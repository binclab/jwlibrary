#ifndef HOME_H_INCLUDED
#define HOME_H_INCLUDED

#include <webkit/webkit.h>

typedef struct
{
    char *path;
    gint day;
    GtkStack *stack;
    GtkTextBuffer *buffer;
} Data;

void init_home(GtkStack *stack);
static void init_dailytext(GtkStack *stack, const gchar* content);
static void show_dailytext(GtkButton *button, GtkStack *stack);
static void *extract_dailytext(void *path), load_dailytext(Data *data);
extern GtkStackPage *page[8];
#endif // HOME_H_INCLUDED