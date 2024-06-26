#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "shared.h"

void init_window(GtkApplication *application, gchar *home);
static void get_managers(), get_devices(), construct_main_window(), construct_media_window();
static void monitor_added(), monitor_removed();
static GdkMonitor *get_monitor(int position);
extern void init_navbar(), init_drawer(), init_winstack();

bool project = true;

#endif // WINDOW_H_INCLUDED
