#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <gtk/gtk.h>
#include <json-glib/json-glib.h>
#include <sqlite3.h>
#include <zip.h>

typedef struct {
  char *filename;
  sqlite3 *database;
} Publication;

extern gchar *home;

Publication open_file(char *path);

static void extract_contents(char *book, zip_int64_t entries, zip_t *archive);

#endif  // PARSER_H_INCLUDED