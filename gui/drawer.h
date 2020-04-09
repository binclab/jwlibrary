#ifndef DRAWER_H_INCLUDED
#define DRAWER_H_INCLUDED

#include "shared.h"

GtkWidget *drawer, *drawerlabel;

void init_drawer();
static void instantiate_drawer(), set_drawer_icons(), connect_drawer_signals();
static void populate_drawer(), show_drawer();

#endif // DRAWER_H_INCLUDED
