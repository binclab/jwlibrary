#include "drawer.h"

void init_drawer()
{
    drawer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(drawer, -1, 100);
    instantiate_drawer();
    set_drawer_icons();
    connect_drawer_signals();
    populate_drawer();
    g_signal_connect(navbtn, "clicked", (GCallback)show_drawer, NULL);
    gtk_paned_set_start_child((GtkPaned*)paned, drawer);
    gtk_paned_set_resize_start_child((GtkPaned*)paned, TRUE);
    gtk_paned_set_shrink_start_child((GtkPaned*)paned, TRUE);
}

static void instantiate_drawer()
{
    drawerlabel = gtk_label_new("Navigation");
}

static void set_drawer_icons()
{

}

static void connect_drawer_signals()
{

}

static void populate_drawer()
{
    gtk_box_append((GtkBox*)drawer, drawerlabel);
}

static void show_drawer()
{
    if(gtk_paned_get_position((GtkPaned*)paned) > 0)
    {
        //gtk_container_remove((GtkContainer*)navbtn, navcloseimg);
        //gtk_container_add((GtkContainer*)navbtn, navopenimg);
        gtk_paned_set_position((GtkPaned*)paned, 0);
    }
    else
    {
        //gtk_container_remove((GtkContainer*)navbtn, navopenimg);
        //gtk_container_add((GtkContainer*)navbtn, navcloseimg);
        gtk_paned_set_position((GtkPaned*)paned, 104);
    }
}
