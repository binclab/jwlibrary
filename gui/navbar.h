#ifndef NAVBAR_H_INCLUDED
#define NAVBAR_H_INCLUDED

#include <gtk/gtk.h>

GtkWidget *navbar, *navcloseimg, *navopenimg, *space;
GtkWidget *navbtn, *homebtn, *biblebtn, *publicbtn, *mediabtn;
GtkWidget *meetingsbtn, *wolbtn, *playerbtn, *settingsbtn;

GtkWidget *create_navbar();
static void instantiate_navbar(), set_navbar_icons();
static void connect_navbar_signals(), populate_navbar();

#endif // NAVBAR_H_INCLUDED
