#ifndef WINSTACK_H_INCLUDED
#define WINSTACK_H_INCLUDED

#include <webkit/webkit.h>

static GtkWidget *winstack;
static GtkStackPage *page[8];

GtkWidget *create_winstack();
static void init_home(), init_bible(), init_publications(), init_media();
static void init_meetings(), init_wol(), init_player(), init_settings();

extern gchar *home;

#endif // WINSTACK_H_INCLUDED