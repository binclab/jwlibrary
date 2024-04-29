#ifndef NAVBAR_H_INCLUDED
#define NAVBAR_H_INCLUDED

#include "shared.h"

GtkWidget *navbar, *navcloseimg, *navopenimg, *space;
GtkWidget *navbtn, *homebtn, *biblebtn, *publicbtn, *mediabtn;
GtkWidget *meetingsbtn, *wolbtn, *playerbtn, *settingsbtn;

void init_navbar();
static void instantiate_navbar(), set_navbar_icons();
static void connect_navbar_signals(), populate_navbar();

#endif // NAVBAR_H_INCLUDED
