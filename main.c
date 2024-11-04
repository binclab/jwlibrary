#include "gui/window.h"

int main(int argc, char *argv[]) {
  argv[0] = "jwlibrary";
  home = (gchar *)malloc(strlen(g_get_home_dir()) + 24);
  sprintf(home, "%s/.local/share/%s/", g_get_home_dir(), argv[0]);
  GtkApplication *application = gtk_application_new("org.jw.library", 0);
  g_signal_connect(application, "activate", (GCallback)init_window, home);
  int status = g_application_run((GApplication *)application, argc, argv);
  g_object_unref(application);
  return status;
}

static void show_sidebar(GtkToggleButton *button, GtkWidget *revealer) {
  gboolean toggled = gtk_toggle_button_get_active(button);
  gtk_widget_set_visible(revealer, toggled);
  gtk_revealer_set_reveal_child((GtkRevealer *)revealer, toggled);
}

void init_window(GtkApplication *application, gchar *home) {
  get_managers();
  //for(char *list: gtk_icon_theme_get_search_path(gtk_icon_theme_get_for_display(display)))
  
  GtkWidget *window = gtk_application_window_new(application);
  GtkWidget *container = gtk_grid_new();
  GtkWidget *hamburger = gtk_toggle_button_new();
  GtkWidget *overlay = gtk_overlay_new();
  GtkWidget *stack = gtk_stack_new();
  GtkWidget *revealer = gtk_revealer_new();
  GtkWidget *toggle = gtk_toggle_button_new();
  GtkStackPage *page =
      gtk_stack_add_titled((GtkStack *)stack, toggle, "Child 1", "Child 1");
  GtkStackPage *page1 =
      gtk_stack_add_titled((GtkStack *)stack, gtk_button_new(), "Child 1", "Child 1");
  GtkStackPage *page2 =
      gtk_stack_add_titled((GtkStack *)stack, gtk_button_new(), "Child 1", "Child 1");
  GtkStackPage *page3 =
      gtk_stack_add_titled((GtkStack *)stack, gtk_button_new(), "Child 1", "Child 1");
  // gtk_widget_set_size_request(container, -1, -1);
  init_navigation((GtkGrid*)container, (GtkStack *)stack);
  gtk_widget_set_size_request(revealer, 100, -1);
  // gtk_widget_set_size_request(pane2, 0, -1);
  gtk_revealer_set_transition_type((GtkRevealer *)revealer,
                                   GTK_REVEALER_TRANSITION_TYPE_SLIDE_RIGHT);
  gtk_stack_page_set_icon_name((GtkStackPage *)page, "go-home");
  gtk_stack_page_set_icon_name((GtkStackPage *)page1, "accessories-dictionary-symbolic");
  gtk_stack_page_set_icon_name((GtkStackPage *)page2, "accessories-dictionary-symbolic");
  gtk_stack_page_set_icon_name((GtkStackPage *)page3, "accessories-dictionary-symbolic");
  g_signal_connect(hamburger, "toggled", (GCallback)show_sidebar, revealer);
  gtk_widget_set_vexpand(overlay, TRUE);
  gtk_widget_set_halign(revealer, GTK_ALIGN_START);
  gtk_widget_set_visible(revealer, FALSE);
  gtk_revealer_set_child((GtkRevealer *)revealer,
                         gtk_button_new_with_label("Reveal"));
  gtk_overlay_set_child((GtkOverlay *)overlay, stack);
  gtk_overlay_add_overlay((GtkOverlay *)overlay, revealer);
  gtk_button_set_icon_name((GtkButton *)hamburger, "open-menu");
  gtk_grid_attach((GtkGrid *)container, hamburger, 0, 0, 1, 1);
  gtk_grid_attach((GtkGrid *)container, overlay, 1, 1, 1, 1);
  gtk_widget_set_size_request(window, 854, 480);
  gtk_window_set_child((GtkWindow *)window, container);
  gtk_window_present((GtkWindow *)window);
}

static void get_managers() {
  display = gdk_display_get_default();
  provider = gtk_css_provider_new();
  projector = get_monitor(1);
  /*GtkIconTheme *theme = gtk_icon_theme_get_for_display(display);
  gtk_icon_theme_add_resource_path(theme, "/com/binclab/jwlibrary/icons");*/
  gtk_css_provider_load_from_resource(provider,
                                      "/com/binclab/jwlibrary/css/style.css");
  gtk_style_context_add_provider_for_display(display,
                                             (GtkStyleProvider *)provider, 600);
}

static GdkMonitor *get_monitor(int position) {
  GListModel *list = gdk_display_get_monitors(display);
  printf("Monitors %i\n", g_list_model_get_n_items(list));
  return g_list_model_get_item(list, 1);
}

static void construct_media_window() {
  // gtk_application_window_new(application);
  mediawindow = gtk_window_new();
  GtkWidget *yeartext = gtk_label_new(
      "“Go, therefore, and make disciples \n. . . , baptizing "
      "them.”\n​—MATT. 28:19.");
  gtk_window_set_title((GtkWindow *)mediawindow, "Media");
  if (projector) {
    gtk_widget_set_size_request(mediawindow, 400, 224);
    gtk_window_fullscreen_on_monitor((GtkWindow *)mediawindow, projector);
    gtk_window_set_child((GtkWindow *)mediawindow, yeartext);
    gtk_window_present((GtkWindow *)mediawindow);
  }
  g_signal_connect(display, "seat-added", (GCallback)monitor_added,
                   mediawindow);
  g_signal_connect(display, "seat-removed", (GCallback)monitor_removed, NULL);
}

static void monitor_added() {
  projector = get_monitor(1);
  if (projector && project)
    gtk_window_fullscreen_on_monitor((GtkWindow *)mediawindow, projector);
  else
    printf("Check your projector settings\n");
}

static void monitor_removed() {
  GListModel *list = gdk_display_get_monitors(display);
  printf("%i\n", g_list_model_get_n_items(list));
  if (g_list_model_get_n_items(list) == 1) {
    gtk_window_unfullscreen((GtkWindow *)mediawindow);
    gtk_widget_set_size_request(mediawindow, 1024, 576);
  }
  g_object_unref(projector);
}

void quit() {
  g_application_quit(gapp);
  g_object_unref(projector);
}
