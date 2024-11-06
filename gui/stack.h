#ifndef WINSTACK_H_INCLUDED
#define WINSTACK_H_INCLUDED

#include <webkit/webkit.h>

static GtkStackPage *page[8];
static void init_home(GtkStack *stack), init_bible(GtkStack *stack), init_publications(GtkStack *stack), init_media(GtkStack *stack);
static void init_meetings(), init_wol(), init_player(), init_settings();
static void show_sidebar(GtkToggleButton *button, GtkWidget *revealer);
extern gchar *home;
extern void init_navigation(GtkGrid *grid, GtkStack *stack);

#endif  // WINSTACK_H_INCLUDED