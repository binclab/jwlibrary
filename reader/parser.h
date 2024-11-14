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
void extract_contents(char *name, zip_int64_t entries, zip_t *archive);
void get_row(sqlite3 *database, int row, int column);
#endif  // PARSER_H_INCLUDED