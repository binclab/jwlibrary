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
    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *stack = gtk_stack_new();

    GtkWidget *switcher = gtk_stack_switcher_new();
    GtkWidget *sidebar = gtk_stack_sidebar_new();
    GtkWidget *label = gtk_label_new_with_mnemonic("Home");

    gtk_stack_sidebar_set_stack((GtkStackSidebar *)sidebar, (GtkStack *)stack);
    gtk_stack_switcher_set_stack((GtkStackSwitcher *)switcher, (GtkStack *)stack);
    gtk_orientable_set_orientation((GtkOrientable *)switcher, GTK_ORIENTATION_VERTICAL);
    GtkStackPage *page1 = gtk_stack_add_titled((GtkStack *)stack, gtk_button_new_with_label("Child 1"), "Child 1", "Child 1");
    GtkStackPage *page2 = gtk_stack_add_titled((GtkStack *)stack, gtk_button_new_with_label("Child 2"), "Child 2", "Child 2");
    GtkStackPage *page3 = gtk_stack_add_titled((GtkStack *)stack, gtk_button_new(), "Bible", "Bible");

    gtk_stack_page_set_icon_name((GtkStackPage *)page1, "audio-volume-high");
    gtk_stack_page_set_icon_name((GtkStackPage *)page2, "audio-volume-high");
    gtk_stack_page_set_icon_name((GtkStackPage *)page3, "audio-volume-high");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(paned, 128, -1);
    gtk_widget_set_size_request(label, 48, -1);
    gtk_widget_set_size_request(box, 1, -1);
    gtk_paned_set_wide_handle((GtkPaned *)paned, TRUE);
    gtk_paned_set_position((GtkPaned *)paned, 0);
    gtk_paned_set_shrink_start_child((GtkPaned *)paned, TRUE);
    gtk_paned_set_resize_start_child((GtkPaned *)paned, TRUE);
    gtk_paned_set_shrink_end_child((GtkPaned *)paned, TRUE);
    gtk_paned_set_resize_end_child((GtkPaned *)paned, FALSE);

    // gtk_widget_set_vexpand(container, TRUE);
    // gtk_widget_set_hexpand(container, TRUE);

    // gtk_widget_set_size_request(label, 100, -1);
    // gtk_widget_set_size_request(button, 100, -1);

    // gtk_fixed_put((GtkFixed*)container, stack, 48, 48);

    gtk_widget_set_margin_top(switcher, 8);

    double position = 32;
    gtk_fixed_put((GtkFixed *)container, stack, position, 0);
    gtk_fixed_put((GtkFixed *)container, switcher, 0, 0);
    gtk_fixed_put((GtkFixed *)container, paned, position, 0);

    gtk_widget_set_vexpand(paned, TRUE);
    gtk_paned_set_start_child((GtkPaned *)paned, sidebar);
    gtk_paned_set_end_child((GtkPaned *)paned, box);

    gtk_window_set_default_icon_name("jwlibrary");
    gtk_window_set_icon_name((GtkWindow *)window, "jwlibrary");
    gtk_window_set_title((GtkWindow *)window, "JW Library");
    gtk_window_set_default_size((GtkWindow *)window, 854, 480);
    gtk_widget_set_size_request(window, 854, 480);
    gtk_window_set_child((GtkWindow *)window, container);

    /*
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *winbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *winstack = create_winstack();
    GtkWidget *drawer = create_drawer((GtkStack *)winstack);
    GtkIconTheme *theme = gtk_icon_theme_get_for_display(display);
    //gtk_icon_theme_add_resource_path(theme, "/com/binclab/jwlibrary/icons/");
    gtk_box_append((GtkBox *)winbox, create_navbar());
    gtk_box_append((GtkBox *)winbox, paned);
    gtk_window_set_child((GtkWindow *)window, winbox);
    construct_media_window();*/
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
