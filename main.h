#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "gtk/gtk.h"

GApplication *gapp;
GdkDisplay *display;
GdkMonitor *projector;
GdkSurface *surface;

GtkStyleProvider *provider;
GtkStyleContext *context;

gchar *home;

static void init_window(GtkApplication *application, gchar *home);
static void get_managers(), get_devices(), construct_main_window(),
    construct_media_window();
static void monitor_added(), monitor_removed();
static GdkMonitor *get_monitor(int position);
extern void init_winstack(GtkGrid *container);
extern GResource *gresources_get_resource(void);

GtkWidget *window, *mediawindow;
GtkIconTheme *theme;

bool project = true;

#endif // WINDOW_H_INCLUDED
