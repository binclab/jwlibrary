#include "window.h"

void init_window()
{
    get_managers();
    get_devices();
    construct_main_window();
    construct_media_window();
    gtk_window_present((GtkWindow*)window);
}

static void get_managers()
{

}

static void get_devices()
{
    display = gdk_display_get_default();
    projector = get_monitor(1);
}

static GdkMonitor *get_monitor(int position){
    GListModel *list = gdk_display_get_monitors(display);
    printf("Monitors %i\n", g_list_model_get_n_items(list));
    return g_list_model_get_item(list, 1);
}

static void construct_main_window()
{
    window = gtk_window_new();
    winbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_window_set_application((GtkWindow*)window, jwlibrary);
    gtk_window_set_default_size((GtkWindow*)window, 640, 240);
    gtk_widget_set_size_request(window, 640, 240);
    gtk_widget_set_valign(window, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(window, GTK_ALIGN_CENTER);
    gtk_paned_set_wide_handle((GtkPaned*)paned, TRUE);
    gtk_paned_set_position((GtkPaned*)paned, 0);
    init_navbar();
    init_drawer();
    init_winstack();
    gtk_box_append((GtkBox*)winbox, paned);
    gtk_window_set_child((GtkWindow*)window, winbox);
}

static void construct_media_window()
{
    mediawindow = gtk_window_new();
    GtkWidget *yeartext = gtk_label_new("“Go, therefore, and make disciples \n. . . , baptizing them.”\n​—MATT. 28:19.");
    gtk_window_set_title((GtkWindow*)mediawindow, "Media");
    if(projector)
    {
        gtk_window_fullscreen_on_monitor((GtkWindow*)mediawindow, projector);
    }
    gtk_widget_set_size_request(mediawindow, 400, 224);
    gtk_window_set_child((GtkWindow*)mediawindow, yeartext);
    gtk_window_present((GtkWindow*)mediawindow);
    g_signal_connect(display, "monitor-added", (GCallback)monitor_added, NULL);
    g_signal_connect(display, "monitor-removed", (GCallback)monitor_removed, NULL);
}

static void monitor_added()
{
    projector = get_monitor(1);
    if(projector && project)
        gtk_window_fullscreen_on_monitor((GtkWindow*)mediawindow, projector);
    else
        printf("Check your projector settings\n");
}

static void monitor_removed()
{
    GListModel *list = gdk_display_get_monitors(display);
    printf("%i\n", g_list_model_get_n_items(list));
    if(g_list_model_get_n_items(list) == 1)
    {
        gtk_window_unfullscreen((GtkWindow*)mediawindow);
        gtk_widget_set_size_request(mediawindow, 1024, 576);
    }
    g_object_unref(projector);
}


