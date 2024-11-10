#ifndef DRAWER_H_INCLUDED
#define DRAWER_H_INCLUDED

#include <gtk/gtk.h>

static GtkWidget *drawer, *drawerlabel;

GtkWidget *create_drawer();
static void instantiate_drawer(), set_drawer_icons(), connect_drawer_signals();
static void populate_drawer(), show_drawer();

#endif // DRAWER_H_INCLUDED