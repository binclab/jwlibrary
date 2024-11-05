#ifndef WINSTACK_H_INCLUDED
#define WINSTACK_H_INCLUDED

#include <webkit/webkit.h>

static GtkWidget *stack;
static GtkStackPage *page[8];

GtkWidget *create_winstack();
static void init_home(), init_bible(), init_publications(), init_media();
static void init_meetings(), init_wol(), init_player(), init_settings();
static void show_sidebar(GtkToggleButton *button, GtkWidget *revealer);
extern gchar *home;
extern void init_navigation(GtkGrid *grid, GtkStack *stack);

#endif  // WINSTACK_H_INCLUDED