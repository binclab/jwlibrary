#include "navigation.h"

void *init_navigation(GtkGrid *grid, GtkStack *stack) {
  GtkWidget *header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget *historybtn = gtk_button_new_from_icon_name("document-open-recent-symbolic");
  GtkWidget *syncbtn = gtk_button_new_from_icon_name("emblem-synchronizing-symbolic");
  GtkWidget *settingsbtn = gtk_button_new_from_icon_name("emblem-system-symbolic");
  GtkWidget *morebtn = gtk_button_new_from_icon_name("view-more-symbolic");
  GtkWidget *navinfo = gtk_label_new("Home");
  switcher = gtk_stack_switcher_new();

  /*navbar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  instantiate_navbar();
  set_navbar_icons();
  connect_navbar_signals();
  populate_navbar();*/

  gtk_orientable_set_orientation((GtkOrientable *)switcher,
                                 GTK_ORIENTATION_VERTICAL);

  gtk_stack_switcher_set_stack((GtkStackSwitcher *)switcher, stack);
  gtk_widget_set_valign(historybtn, GTK_ALIGN_END);
  gtk_widget_set_halign(syncbtn, GTK_ALIGN_END);
  gtk_widget_set_halign(settingsbtn, GTK_ALIGN_END);
  gtk_widget_set_halign(morebtn, GTK_ALIGN_END);
  gtk_box_append((GtkBox*)header, navinfo);
  gtk_box_append((GtkBox*)header, historybtn);
  gtk_box_append((GtkBox*)header, syncbtn);
  gtk_box_append((GtkBox*)header, settingsbtn);
  gtk_box_append((GtkBox*)header, morebtn);
  gtk_widget_set_vexpand(header, TRUE);
  gtk_widget_set_valign(switcher, GTK_ALIGN_START);
  gtk_grid_attach(grid, switcher, 0, 1, 1, 1);
  gtk_grid_attach(grid, header, 1, 0, 1, 1);
}

static void instantiate_navbar() {
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

static void set_navbar_icons() {
  gtk_button_set_child((GtkButton *)navbtn, navopenimg);
}

static void connect_navbar_signals() { gtk_widget_set_vexpand(space, TRUE); }

static void populate_navbar() {
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