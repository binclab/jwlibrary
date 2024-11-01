#include "gui/window.h"

int main(int argc, char *argv[])
{
    argv[0] = "jwlibrary";
    home = (gchar *)malloc(strlen(g_get_home_dir()) + 24);
    sprintf(home, "%s/.local/share/%s/", g_get_home_dir(), argv[0]);
    GtkApplication *application = gtk_application_new("org.jw.library", 0);
    g_signal_connect(application, "activate", (GCallback)init_window, home);
    int status = g_application_run((GApplication *)application, argc, argv);
    g_object_unref(application);
    return status;
}

void init_window(GtkApplication *application, gchar *home)
{
    get_managers();
    get_devices();
    GtkWidget *window = gtk_application_window_new(application);
    GtkWidget *container = gtk_grid_new();
    GtkWidget *switcher = gtk_stack_switcher_new();
    GtkWidget *header = gtk_button_new_with_label("title");
    GtkWidget *overlay = gtk_overlay_new();
    GtkWidget *stack = gtk_stack_new();
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    GtkStackPage *page = gtk_stack_add_titled((GtkStack *)stack, gtk_button_new_with_label("Child 1"), "Child 1", "Child 1");

    gtk_widget_set_size_request(container, -1, -1);
    gtk_overlay_set_child((GtkOverlay *)overlay, stack);
    gtk_overlay_add_overlay((GtkOverlay *)overlay, paned);
    gtk_orientable_set_orientation((GtkOrientable *)switcher, GTK_ORIENTATION_VERTICAL);
    gtk_stack_page_set_icon_name((GtkStackPage *)page, "audio-volume-high");
    gtk_stack_switcher_set_stack((GtkStackSwitcher *)switcher, (GtkStack *)stack);

    gtk_grid_set_column_homogeneous((GtkGrid *)container, FALSE);
    gtk_grid_set_row_homogeneous((GtkGrid *)container, FALSE);
    gtk_widget_set_hexpand(header, TRUE);
    gtk_widget_set_vexpand(overlay, TRUE);
    gtk_widget_set_hexpand(switcher, FALSE);
    gtk_grid_attach((GtkGrid *)container, switcher, 0, 0, 1, 2);
    gtk_grid_attach((GtkGrid *)container, header, 1, 0, 1, 1);
    gtk_grid_attach((GtkGrid *)container, overlay, 1, 1, 1, 1);
    gtk_widget_set_size_request(window, 854, 480);
    gtk_window_set_child((GtkWindow *)window, container);
    gtk_window_present((GtkWindow *)window);
}

static void get_managers()
{
}

static void get_devices()
{
    display = gdk_display_get_default();
    projector = get_monitor(1);
}

static GdkMonitor *get_monitor(int position)
{
    GListModel *list = gdk_display_get_monitors(display);
    printf("Monitors %i\n", g_list_model_get_n_items(list));
    return g_list_model_get_item(list, 1);
}

static void construct_media_window()
{
    // gtk_application_window_new(application);
    mediawindow = gtk_window_new();
    GtkWidget *yeartext = gtk_label_new("“Go, therefore, and make disciples \n. . . , baptizing them.”\n​—MATT. 28:19.");
    gtk_window_set_title((GtkWindow *)mediawindow, "Media");
    if (projector)
    {
        gtk_widget_set_size_request(mediawindow, 400, 224);
        gtk_window_fullscreen_on_monitor((GtkWindow *)mediawindow, projector);
        gtk_window_set_child((GtkWindow *)mediawindow, yeartext);
        gtk_window_present((GtkWindow *)mediawindow);
    }
    g_signal_connect(display, "seat-added", (GCallback)monitor_added, mediawindow);
    g_signal_connect(display, "seat-removed", (GCallback)monitor_removed, NULL);
}

static void monitor_added()
{
    projector = get_monitor(1);
    if (projector && project)
        gtk_window_fullscreen_on_monitor((GtkWindow *)mediawindow, projector);
    else
        printf("Check your projector settings\n");
}

static void monitor_removed()
{
    GListModel *list = gdk_display_get_monitors(display);
    printf("%i\n", g_list_model_get_n_items(list));
    if (g_list_model_get_n_items(list) == 1)
    {
        gtk_window_unfullscreen((GtkWindow *)mediawindow);
        gtk_widget_set_size_request(mediawindow, 1024, 576);
    }
    g_object_unref(projector);
}

void quit()
{
    g_application_quit(gapp);
    g_object_unref(projector);
}
