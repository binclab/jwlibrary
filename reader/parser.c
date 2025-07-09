#include "parser.h"

// Function to process the manifest file
void process_manifest(char *buffer, char *filename) {
    GError *gerror = NULL;
    JsonParser *parser = json_parser_new();
    json_parser_load_from_data(parser, buffer, -1, &gerror);
    JsonNode *node = json_parser_get_root(parser);
    JsonObject *object = json_node_get_object(node);
    node = json_object_get_member(object, "publication");
    object = json_node_get_object(node);
    filename = (char *)json_object_get_string_member(object, "fileName");
    char *result = strstr(filename, ".db");
    if (result != NULL) {
        *result = '\0';
    }
    g_object_unref(parser);
}

// Function to open a file and process its contents
void open_file(char *path) {
    zip_error_t error;
    zip_source_t *source = zip_source_file_create(path, 0, -1, &error);
    if (!source) {
        fprintf(stderr, "Failed to create zip source: %s\n", zip_error_strerror(&error));
        return;
    }

    zip_t *archive = zip_open_from_source(source, ZIP_RDONLY, &error);
    if (!archive) {
        fprintf(stderr, "Failed to open zip archive: %s\n", zip_error_strerror(&error));
        zip_source_free(source);
        return;
    }

    char *filename = "manifest.json";
    zip_int64_t file_index = zip_name_locate(archive, filename, ZIP_FL_NOCASE);
    if (file_index >= 0) {
        zip_file_t *file = zip_fopen_index(archive, file_index, 0);
        if (file) {
            struct zip_stat stat;
            zip_stat_index(archive, file_index, 0, &stat);
            char *buffer = malloc(stat.size);
            if (buffer && zip_fread(file, buffer, stat.size) > 0) {
                process_manifest(buffer, filename);
            }
            free(buffer);
            zip_fclose(file);
        }
    }

    file_index = zip_name_locate(archive, "contents", ZIP_FL_NOCASE);
    if (file_index >= 0) {
        zip_file_t *file = zip_fopen_index(archive, file_index, 0);
        if (file) {
            struct zip_stat stat;
            zip_stat_index(archive, file_index, 0, &stat);
            char *buffer = malloc(stat.size);
            if (buffer && zip_fread(file, buffer, stat.size) > 0) {
                zip_close(archive);
                source = zip_source_buffer_create(buffer, stat.size, 1, &error);
                if ((archive = zip_open_from_source(source, ZIP_RDONLY, &error))) {
                    extract_contents(filename, zip_get_num_entries(archive, 0), archive);
                }
            }
            free(buffer);
            zip_fclose(file);
        }
    }

    zip_close(archive);
}


// Function to extract contents from the archive
void extract_contents(char *name, zip_int64_t entries, zip_t *archive) {
    for (zip_int64_t i = 0; i < entries; i++) {
        GError *error = NULL;
        const char *filename = zip_get_name(archive, i, ZIP_FL_ENC_GUESS);
        struct zip_stat stat;
        zip_stat_index(archive, i, 0, &stat);

        GFile *gfile;
        char directory[strlen(home) + 9 + (name ? strlen(name) + 1 : 0) + strlen(filename)];
        sprintf(directory, "%slibrary/%s%s", home, name ? name : "", name ? "/" : "");
        gfile = g_file_new_for_path(directory);

        g_file_make_directory_with_parents(gfile, NULL, NULL);

        char filepath[strlen(g_file_get_path(gfile)) + strlen(filename) + 1];
        sprintf(filepath, "%s/%s", g_file_get_path(gfile), filename);
        gfile = g_file_new_for_path(filepath);

        zip_file_t *file = zip_fopen_index(archive, i, ZIP_FL_ENC_GUESS);
        GFileOutputStream *stream = g_file_replace(gfile, NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL);

        char *buffer = malloc(stat.size);
        if (buffer && zip_fread(file, buffer, stat.size) > 0) {
            g_output_stream_write_all((GOutputStream*)stream, buffer, stat.size, NULL, NULL, &error);
        }

        free(buffer);
        zip_fclose(file);
    }
}

const gchar *get_content(Publication *publication, char *instruction, GtkTextBuffer *label) {
    gchar *text = NULL;
    sqlite3_stmt *stmt = NULL;
    EVP_CIPHER_CTX *context = EVP_CIPHER_CTX_new();
    struct Sizes {
        int encrypted;
        int decrypted;
        int update;
        uLong compressed;
        uLong decompressed;
        int length;
    } size;

    if (sqlite3_prepare_v2(publication->database, instruction, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare SQL statement\n");
        EVP_CIPHER_CTX_free(context);
        return NULL;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        fprintf(stderr, "No row returned by SQL query\n");
        sqlite3_finalize(stmt);
        EVP_CIPHER_CTX_free(context);
        return NULL;
    }

    size.encrypted = sqlite3_column_bytes(stmt, 0);
    const unsigned char *blob = (const unsigned char *)sqlite3_column_blob(stmt, 0);

    unsigned char *compressed = (unsigned char *)malloc(size.encrypted);
    if (!compressed) {
        fprintf(stderr, "Memory allocation failed\n");
        sqlite3_finalize(stmt);
        EVP_CIPHER_CTX_free(context);
        return NULL;
    }

    if (!context || !EVP_DecryptInit_ex(context, EVP_aes_128_cbc(), NULL, publication->key, publication->iv)) {
        fprintf(stderr, "Decryption context initialization failed\n");
        free(compressed);
        sqlite3_finalize(stmt);
        EVP_CIPHER_CTX_free(context);
        return NULL;
    }

    if (!EVP_DecryptUpdate(context, compressed, &size.update, blob, size.encrypted)) {
        fprintf(stderr, "Decryption update failed\n");
        free(compressed);
        sqlite3_finalize(stmt);
        EVP_CIPHER_CTX_free(context);
        return NULL;
    }

    if (!EVP_DecryptFinal_ex(context, compressed + size.update, &size.decrypted)) {
        fprintf(stderr, "Decryption finalization failed\n");
        free(compressed);
        sqlite3_finalize(stmt);
        EVP_CIPHER_CTX_free(context);
        return NULL;
    }

    size.compressed = size.decrypted + size.update;
    unsigned char *buffer = (unsigned char *)malloc(size.compressed * 3);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        free(compressed);
        sqlite3_finalize(stmt);
        EVP_CIPHER_CTX_free(context);
        return NULL;
    }

    if (uncompress(buffer, &size.decompressed, compressed, size.compressed) != Z_OK) {
        fprintf(stderr, "Decompression failed\n");
        free(buffer);
        free(compressed);
        sqlite3_finalize(stmt);
        EVP_CIPHER_CTX_free(context);
        return NULL;
    }

    buffer[size.decompressed] = '\0';
    size.length = strrchr((char *)buffer, '>') - (char *)buffer;
    buffer[size.length] = '\0';
    text = (char *)malloc(size.length + 1);
    if (!text) {
        fprintf(stderr, "Memory allocation failed\n");
        free(buffer);
        free(compressed);
        sqlite3_finalize(stmt);
        EVP_CIPHER_CTX_free(context);
        return NULL;
    }
    strcpy(text, (char *)buffer);

    xmlInitParser();
    LIBXML_TEST_VERSION
    int options = HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING;
    htmlDocPtr doc = htmlReadMemory((const char *)text, size.decompressed, NULL, NULL, options);
    if (doc) {
        xmlChar *snippet = xmlNodeGetContent(doc->children->next->children->children->next->next);
        gtk_text_buffer_set_text(label, (const gchar *)snippet, -1);
        xmlFree(snippet);
        xmlFreeDoc(doc);
    } else {
        fprintf(stderr, "HTML parsing failed\n");
    }

    free(buffer);
    free(compressed);
    sqlite3_finalize(stmt);
    EVP_CIPHER_CTX_free(context);

    return text;
}

// Function to set up the publication
void setup_publication(Publication *publication, const char *instruction) {
    sqlite3_stmt *stmt = NULL;

    if (sqlite3_prepare_v2(publication->database, instruction, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare SQL statement: %s\n", sqlite3_errmsg(publication->database));
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *index = (const char *)sqlite3_column_text(stmt, 0);
        const char *symbol = (const char *)sqlite3_column_text(stmt, 1);
        const char *year = (const char *)sqlite3_column_text(stmt, 2);
        const char *issue = (const char *)sqlite3_column_text(stmt, 3);

        int length = strlen(index) + strlen(symbol) + strlen(year) + strlen(issue) + 4; // for separators and null-terminator
        char *card = (char *)malloc(length + 1);
        if (!card) {
            fprintf(stderr, "Memory allocation failed\n");
            sqlite3_finalize(stmt);
            return;
        }

        sprintf(card, "%s_%s_%s", index, symbol, year);
        if (strcmp("0", issue) != 0) {
            snprintf(card, length, "%s_%s_%s_%s", index, symbol, year, issue);
        }

        unsigned char digest[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char *)card, strlen(card), digest);
        free(card);

        gsize size;
        GBytes *data = g_resources_lookup_data("/com/binclab/jwlibrary/constant", 0, NULL);
        const unsigned char *constant = (const unsigned char *)g_bytes_get_data(data, &size);

        for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            if (i < SHA256_DIGEST_LENGTH / 2) {
                publication->key[i] = constant[i] ^ digest[i];
            } else {
                publication->iv[i - SHA256_DIGEST_LENGTH / 2] = constant[i] ^ digest[i];
            }
        }
    } else {
        fprintf(stderr, "No data found for the given instruction\n");
    }

    sqlite3_finalize(stmt);
}

// Function to write content to a file
static void write_file(char *path, const void *content, gsize content_size) {
    GFile *file = g_file_new_for_path(path);
    GFileOutputStream *output_stream = g_file_create(file, G_FILE_CREATE_NONE, NULL, NULL);
    if (output_stream) {
        gssize bytes_written = g_output_stream_write(G_OUTPUT_STREAM(output_stream), content, content_size, NULL, NULL);
        if (bytes_written != content_size) {
            fprintf(stderr, "Failed to write the complete content to the file\n");
        }
        g_output_stream_close(G_OUTPUT_STREAM(output_stream), NULL, NULL);
        g_object_unref(output_stream);
    } else {
        fprintf(stderr, "Failed to create the output file stream\n");
    }
    g_object_unref(file);
}
