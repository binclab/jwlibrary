#ifndef WINSTACK_H_INCLUDED
#define WINSTACK_H_INCLUDED

#include <webkit/webkit.h>

static GtkStackPage *page[8];
static void init_home(GtkStack *stack), init_bible(GtkStack *stack),
    init_library(GtkStack *stack), init_media(GtkStack *stack);
static void init_meetings(), init_wol(), init_player(), init_settings();
static void show_sidebar(GtkToggleButton *button, GtkWidget *revealer);
extern gchar *home;
extern void init_navigation(GtkGrid *grid, GtkStack *stack);

enum { COL_NAME, N_COLUMNS };

G_DECLARE_FINAL_TYPE(BibleBook, bible_book, BIBLE, BOOK, GObject)
G_DEFINE_FINAL_TYPE (BibleBook, bible_book, G_TYPE_OBJECT)
#endif  // WINSTACK_H_INCLUDED