#include "navbar.h"

GtkWidget *create_navbar()
{
    navbar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    instantiate_navbar();
    set_navbar_icons();
    connect_navbar_signals();
    populate_navbar();
    return navbar;
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
    playerbtn = gtk_button_new();
    settingsbtn = gtk_button_new();
}

static void set_navbar_icons()
{
    gtk_button_set_child((GtkButton *)navbtn, navopenimg);
}

static void connect_navbar_signals()
{
    gtk_widget_set_vexpand(space, TRUE);
}

static void populate_navbar()
{
    gtk_box_append((GtkBox *)navbar, navbtn);
    gtk_box_append((GtkBox *)navbar, homebtn);
    gtk_box_append((GtkBox *)navbar, biblebtn);
    gtk_box_append((GtkBox *)navbar, publicbtn);
    gtk_box_append((GtkBox *)navbar, mediabtn);
    gtk_box_append((GtkBox *)navbar, meetingsbtn);
    gtk_box_append((GtkBox *)navbar, wolbtn);
    gtk_box_append((GtkBox *)navbar, playerbtn);
    gtk_box_append((GtkBox *)navbar, space);
    gtk_box_append((GtkBox *)navbar, settingsbtn);
}