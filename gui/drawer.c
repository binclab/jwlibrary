#include "drawer.h"

GtkWidget *create_drawer()
{
    drawer = gtk_center_box_new();
    /*GtkWidget *switcher = gtk_stack_switcher_new();
    gtk_stack_switcher_set_stack((GtkStackSwitcher*)switcher, (GtkStack*)winstack);
    gtk_center_box_set_start_widget((GtkCenterBox*)drawer, switcher);
    gtk_orientable_set_orientation((GtkOrientable*)drawer, GTK_ORIENTATION_VERTICAL);
    gtk_widget_set_size_request(drawer, 100, -1);
    instantiate_drawer();
    set_drawer_icons();
    connect_drawer_signals();
    populate_drawer();
    g_signal_connect(navbtn, "clicked", (GCallback)show_drawer, NULL);*/
    return drawer;
}
