#include "gui/window.h"

int main(int argc, char *argv[])
{
    int status;
    home = (gchar *)malloc(strlen(g_get_home_dir()) + 24);
    sprintf(home, "%s/.local/share/jwlibrary/", g_get_home_dir());
    GtkApplication *application = gtk_application_new("org.jw.library", 0);
    g_signal_connect(application, "activate", (GCallback)init_window, home);
    status = g_application_run((GApplication *)application, argc, argv);
    g_object_unref(application);
    return status;
}

void init_window(GtkApplication *application, gchar *home)
{
    get_managers();
    get_devices();
    GtkWidget *window = gtk_application_window_new(application);
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *winbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *winstack = create_winstack();
    GtkWidget *drawer = create_drawer((GtkStack *)winstack);
    GtkIconTheme *icon_theme = gtk_icon_theme_get_for_display(display);
    gtk_icon_theme_add_resource_path(icon_theme, "/com/binclab/jwlibrary");

    if (gtk_icon_theme_has_icon(icon_theme, "my-icon") != 1)
    {
        printf("No Icon Found\n");
    }

    gtk_window_set_default_icon_name("ic_jwlibrary");
    gtk_window_set_title((GtkWindow*)window, "JW Library");
    gtk_window_set_icon_name((GtkWindow*)window, "ic_jwlibrary");
    gtk_window_set_default_size((GtkWindow *)window, 854, 480);
    gtk_widget_set_size_request(window, 854, 480);
    gtk_widget_set_hexpand(paned, TRUE);
    gtk_paned_set_wide_handle((GtkPaned *)paned, TRUE);
    gtk_paned_set_position((GtkPaned *)paned, 0);
    gtk_paned_set_shrink_start_child((GtkPaned *)paned, TRUE);
    gtk_paned_set_resize_start_child((GtkPaned *)paned, FALSE);
    gtk_paned_set_shrink_end_child((GtkPaned *)paned, FALSE);
    gtk_paned_set_resize_end_child((GtkPaned *)paned, TRUE);
    gtk_paned_set_start_child((GtkPaned *)paned, drawer);
    gtk_paned_set_end_child((GtkPaned *)paned, winstack);
    gtk_box_append((GtkBox *)winbox, create_navbar());
    gtk_box_append((GtkBox *)winbox, paned);
    gtk_window_set_child((GtkWindow *)window, winbox);
    construct_media_window();
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
