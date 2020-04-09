#include "navbar.h"

void init_navbar()
{
    navbar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    instantiate_navbar();
    set_navbar_icons();
    connect_navbar_signals();
    populate_navbar();
    gtk_container_add((GtkContainer*)winbox, navbar);
}

static void instantiate_navbar()
{
    space = gtk_label_new(NULL);
    navopenimg = gtk_image_new();
    navcloseimg = gtk_image_new();

    navbtn = gtk_button_new();
    homebtn = gtk_button_new();
    biblebtn = gtk_button_new();
    publicbtn = gtk_button_new();
    mediabtn = gtk_button_new();
    meetingsbtn = gtk_button_new();
    wolbtn = gtk_button_new();
    wofbtn = gtk_button_new();
    playerbtn = gtk_button_new();
    settingsbtn = gtk_button_new();
}

static void set_navbar_icons()
{
    gtk_container_add((GtkContainer*)navbtn, navopenimg);
}

static void connect_navbar_signals()
{
    gtk_widget_set_vexpand(space, TRUE);

}

static void populate_navbar()
{
    gtk_container_add((GtkContainer*)navbar, navbtn);
    gtk_container_add((GtkContainer*)navbar, homebtn);
    gtk_container_add((GtkContainer*)navbar, biblebtn);
    gtk_container_add((GtkContainer*)navbar, publicbtn);
    gtk_container_add((GtkContainer*)navbar, mediabtn);
    gtk_container_add((GtkContainer*)navbar, meetingsbtn);
    gtk_container_add((GtkContainer*)navbar, wolbtn);
    gtk_container_add((GtkContainer*)navbar, wofbtn);
    gtk_container_add((GtkContainer*)navbar, playerbtn);
    gtk_container_add((GtkContainer*)navbar, space);
    gtk_container_add((GtkContainer*)navbar, settingsbtn);
}
