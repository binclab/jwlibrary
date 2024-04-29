#ifndef WINSTACK_H_INCLUDED
#define WINSTACK_H_INCLUDED

#include "shared.h"
#include <webkit/webkit.h>

GtkWidget *winstack, *navbar;
GtkStackPage *page[8];

GtkWidget *get_navbar(GtkWidget* paned);
static void init_home(), init_bible(), init_publications(), init_media();
static void init_meetings(), init_wol(), init_player(), init_settings();

#endif // WINSTACK_H_INCLUDED
