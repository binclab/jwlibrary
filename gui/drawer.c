#include "drawer.h"

GtkWidget *create_drawer(GtkStack *winstack)
{
    drawer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(drawer, 100, -1);
    /*instantiate_drawer();
    set_drawer_icons();
    connect_drawer_signals();
    populate_drawer();
    g_signal_connect(navbtn, "clicked", (GCallback)show_drawer, NULL);*/

    GtkGesture *dragger = gtk_gesture_drag_new();
    g_signal_connect(dragger, "drag-end", (GCallback)move_handle, gtk_widget_get_parent(drawer));
    gtk_widget_add_controller(drawer, (GtkEventController *)dragger);

    return drawer;
}
