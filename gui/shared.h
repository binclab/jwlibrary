#ifndef SHARED_H_INCLUDED
#define SHARED_H_INCLUDED

#include <gtk/gtk.h>

extern void init_window();

extern GApplication *gapp;
extern GdkDisplay *display;
extern GdkMonitor *projector;
extern GdkSurface *surface;
extern GtkApplication *jwlibrary;
extern GtkWidget *window, *navbar, *drawer, *winstack, *paned, *winbox;
extern GtkWidget *navbtn, *navcloseimg, *navopenimg;

extern void init_window(), init_navbar(), init_drawer(), init_winstack(), quit();


#endif // SHARED_H_INCLUDED
