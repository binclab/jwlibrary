#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <gtk/gtk.h>
#include <json-glib/json-glib.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <sqlite3.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <zip.h>
#include <zlib.h>

typedef struct
{
    char *name;
    sqlite3 *database;
    unsigned char key[16];
    unsigned char iv[16];
} Publication;

extern gchar *home;

void open_file(char *path);
void extract_contents(char *name, zip_int64_t entries, zip_t *archive);
const gchar *get_content(Publication *publication, char *instruction, GtkTextBuffer *label);
void setup_publication(Publication *publication, const char *instruction);
static void decryptText(const char *name);
static void write_file(char *path, const void *content, gsize content_size);
static void print_(unsigned char *blob, int length);

extern unsigned char *get_text(unsigned char *bArr, int bArr_len, unsigned char *key, unsigned char *iv, int *output_len);
#endif // PARSER_H_INCLUDED