#include "parser.h"

/*1. Determine the publication card hash

   1. Query the SQLite `Publication` table
   2. Create a list with the `MepsLanguageIndex`, `Symbol`, `Year` fields
   3. If the `IssueTagNumber` field is not zero, add it to the end of the list
   4. Join the list with underscores to one string, for example for
w_S_202206.jwpub, this would be `1_w22_2022_20220600`
   5. Calculate the SHA 256 hash of that string
   6. Calculate the bitwise XOR with
`11cbb5587e32846d4c26790c633da289f66fe5842a3a585ce1bc3a294af5ada7` [CyberChef
example
1](https://gchq.github.io/CyberChef/#recipe=SHA2('256',64,160)From_Hex('Auto')XOR(%7B'option':'Hex','string':'11cbb5587e32846d4c26790c633da289f66fe5842a3a585ce1bc3a294af5ada7'%7D,'Standard',false)To_Hex('None',0)&input=MV93MjJfMjAyMl8yMDIyMDYwMA)

2. Decrypt the text

   1. Query a row from the `Document`, `BibleChapter` or `BibleVerse` table
   2. Read the encoded `Content` field
   3. Run AES-128-CBC, use the first 16 bytes of the hash as AES Key, and the
last 16 bytes as Initialization Vector (IV)
   4. Run Zlib Inflate
      [CyberChef example
2](https://gchq.github.io/CyberChef/#recipe=AES_Decrypt(%7B'option':'Hex','string':'909fd5b41ddd8a75ac39c69604828a7d'%7D,%7B'option':'Hex','string':'3bc2c616d0ca2cff6dc4c0d7263a2327'%7D,'CBC','Hex','Raw',%7B'option':'Hex','string':''%7D,%7B'option':'Hex','string':''%7D)Zlib_Inflate(0,0,'Adaptive',false,false)&input=ZjZhZmMwMTEzZmRiMzY4MDE4ZmEzYmEwZDUwNjJlZWFmNGJlNzVhY2Q2NDJkNzM0YTQ2N2M2OTNjODIyMWM2NzM4ODMwYzE0NDQwMjVkZWQ2ZTZmNGZiNjBjZjgyYzcwYWUyYTY5M2EyYTg3NjQ5NWVlMGMxZTU5MDE1NDcyODM0OTMyMGI1OWY2NDAwNzRjMDgzMzYxN2RhYzI5N2ZjZmI1NTY4ODhhMDgzOTAyZGZhYTgyZDNmNmY1MjZhMjI2NTJkZTlhYmEzZjU4NjRkM2RlOTQzMGU2N2Q4NmViNzQwYmUzM2MyNmFjMWVjMmZmNDg0MGE1ODRkYjdhMmMyM2I3Nzc5Y2FlZDNhZGZhZjQ5MzJhZWJhMjM4MDUxNTgzNjRkMjBhNGIxMGU3Njg3YjU4NzBjNDMzZWZmOTk2ZmJjMGI5NGRkYzRkMTI0YjA0NjU2NTUxMjVkZGNmNzI4ZjNlYTgxMDk5YjZkNGM1YjM5NTczM2U5OTZjNjE0YzgyMTA4MTZkNDc3MzViMjI4YzU0MWJiNGJhMTBkMDBlODVkZmVlODlmN2YzOTQ0Y2NmOTk2YWJmMDJkY2QyZTdkMzJlOTU0MGY3ZTVkMDUzMjE2OWFjZTM5ODk5MWI2YzMzYTlmZDdhYmUzMjMyYTU4ODIwYWZhNDFmOTdmMTU2YWI4YTExMzEzMjI5MjhkMDNhOWM4OTE4NGEzOTE5YTQ3NGM2YjM1MWM4MDg2NDkwYWM0ZDYyYTI3MDU2YWMzN2M2NTFmZjYxYmExMmJmZDRhNzk0YmQxNjNlNmM3MjRhYmY2ZjMxMTc1OTQyMDE4OWM3NjNkZGJjMGQxMzQ2Y2Q1ZDUyYjJiNzExYTg1MGY0NWI3MTk5NDIzNDIwNDRhZDVhYjMwMmY1ZGQzMWJjZjlhMjcwZjM4YTlmNmJlMWFjZDlkYzMxYTczYjE1YzBlNDcwNmYyYThjNDE3NmFiNzg1ODI4NDMyMjUwNzM0MzIxMzY1MWRhMTk5ODg5NTMyYzJiOTQyYTkwOTRjMDVlOWY2OGRjM2ExNmU0NTUzNTE4MTA4MzQzODk2YmRhZTA2ZTVmYjIxZDYzMzRjYjk4NmVmMzlkN2Q1MzQ3YTQwNWU3YzkzMGYzYTVlN2U1ZWM1MWY5MDk4OTNkYjg1ZGZlZjUzYTM3ODBhZWQ3OGNkNjRjM2EzMTdjNWM5MDkwZjYyYjNhMGVkMjZiODBlYWMxNzMxMmEzMDkxMzA0MTIxYmM1Mzc4ZTU3MGQ5MjVjMjcwMmYwNmY3ZWJlNTYwNGRmNDRmMjQ0YjA5MjYxODkyYTE0OTIyYzkxMTBhZTIyY2FkMzg1NmM3MDQ1NDlmMmYwNTVmNGI1Mjg0MDQwYzk5Y2I4ZGY2Y2Q4N2Q5NjA1MmIzYTJjY2IxYmE1ZWMxNTMyOWY2ZThlNmM2NjhjMjkyMjViMWMxN2YxYTVhN2MzZmQ0YmVmODRiMzYyYjJkOTg3NGUyMTIyNDUzYzUyMDA3OTFkOTFhYmZjMzU0YzkxMWJlMTY4NmFmNmIzYTJmMjBjZTc2MzBlZjRiMzJjYWExYzcyZTA2NzhlNDcwMTk2NTYzZTZlNTgxZTNjYjAwOTRlOGYyMWIyZTUxZWZjYTNlNDdkZDExN2YzNGFmZWJhNmI2ODJjY2FkZjhkM2Q2ZjE5MDVlNzIxN2JkZTVjMTU3ZThiMmExOWYyYWFiZGEwZTM3OGZkODA3MjgwNGVmNWJhN2ZjMTQzOTg1NmFjNDVkYjY1MDY1OTBkMDI0Zjc5YjY0Njk1ZWJiYTYyN2U2YTdmOTkzYzZlMmY3NDdhZGQ0MmYyOTQyMGVjMzc5NmJjNWU5M2QzNzAwNTg0NzM4ZDI2YmE3ODUzZTVlNjgzMmU0YzQ5NDM1MGI5MThjMTlmZTI1MmY1)

*/

Publication open_file(char *path) {
  Publication publication = {.database = NULL, .filename = NULL};
  zip_error_t error;
  zip_source_t *source = zip_source_file_create(path, 0, -1, &error);
  zip_t *archive;
  if (source && (archive = zip_open_from_source(source, ZIP_RDONLY, &error))) {
    char *filename = "manifest.json";
    zip_int64_t file_index = zip_name_locate(archive, filename, ZIP_FL_NOCASE);
    zip_file_t *file;
    if ((file_index >= 0) && (file = zip_fopen_index(archive, file_index, 0))) {
      struct zip_stat stat;
      zip_stat_index(archive, file_index, 0, &stat);
      char *buffer = malloc(stat.size);
      if ((buffer != NULL) && (zip_fread(file, buffer, stat.size) > 0)) {
        GError *gerror = NULL;
        JsonParser *parser = json_parser_new();
        json_parser_load_from_data(parser, buffer, -1, &gerror);
        JsonNode *node = json_parser_get_root(parser);
        JsonObject *object = json_node_get_object(node);
        node = json_object_get_member(object, "publication");
        object = json_node_get_object(node);
        filename = (char *)json_object_get_string_member(object, "fileName");
        char *result = strstr(filename, ".db");
        if (result != NULL) *result = '\0';
      }
      free(buffer);
    }
    zip_fclose(file);
    file_index = zip_name_locate(archive, "contents", ZIP_FL_NOCASE);
    if ((file_index >= 0) && (file = zip_fopen_index(archive, file_index, 0))) {
      struct zip_stat stat;
      zip_stat_index(archive, file_index, 0, &stat);
      char *buffer = malloc(stat.size);
      if ((buffer != NULL) && (zip_fread(file, buffer, stat.size) > 0)) {
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
  return publication;
}

void extract_contents(char *name, zip_int64_t entries, zip_t *archive) {
  for (zip_int64_t i = 0; i < entries; i++) {
    GError *error = NULL;
    const char *filename = zip_get_name(archive, i, ZIP_FL_ENC_GUESS);
    struct zip_stat stat;
    zip_stat_index(archive, i, 0, &stat);
    GFile *gfile;
    if (name) {
      int length = strlen(home) + 9 + strlen(name) + strlen(filename);
      char directory[length];
      sprintf(directory, "%slibrary/%s", home, name);
      gfile = g_file_new_for_path(directory);
    } else {
      int length = strlen(home) + 8 + strlen(filename);
      char directory[length];
      sprintf(directory, "%slibrary", home);
      gfile = g_file_new_for_path(directory);
    }
    g_file_make_directory_with_parents(gfile, NULL, NULL);
    char filepath[strlen(g_file_get_path(gfile)) + strlen(filename)];
    sprintf(filepath, "%s/%s", g_file_get_path(gfile), filename);
    gfile = g_file_new_for_path(filepath);
    zip_file_t *file = zip_fopen_index(archive, i, ZIP_FL_ENC_GUESS);
    GOutputStream *stream = (GOutputStream *)g_file_replace(
        gfile, NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL);
    char *buffer = malloc(stat.size);
    if ((buffer != NULL) && (zip_fread(file, buffer, stat.size) > 0)) {
      g_output_stream_write_all(stream, buffer, stat.size, NULL, NULL, &error);
    }
    free(buffer);
    zip_fclose(file);
  }
}

void get_row(sqlite3 *database, int row, int column) {
  char *sql = malloc(43);
  sqlite3_stmt *stmt;
  sprintf(sql, "SELECT * FROM DatedText LIMIT 1 OFFSET %i;", row);
  // const char *sql = "SELECT blob_column FROM DatedText WHERE id = 000;";
  sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    // sqlite3_bind_int(stmt, 1, column);
    const void *buffer = sqlite3_column_blob(stmt, column);
    printf("%s", (char *)buffer);
  }
  free(sql);
}