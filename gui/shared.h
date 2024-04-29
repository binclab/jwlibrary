#ifndef SHARED_H_INCLUDED
#define SHARED_H_INCLUDED

#include <gtk/gtk.h>

extern void init_window();

extern GApplication *gapp;
extern GdkDisplay *display;
extern GdkMonitor *projector;
extern GdkSurface *surface;

extern void init_window(), init_winstack(), quit();

extern gchar *home;


#endif // SHARED_H_INCLUDED
