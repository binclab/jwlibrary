#include "main.h"

int main(int argc, char *argv[]) {
  argv[0] = "jwlibrary";
  home = malloc(strlen(g_get_user_data_dir()) + strlen(argv[0]) + 1);
  sprintf(home, "%s/%s/", g_get_user_data_dir(), argv[0]);
  GtkApplication *application = gtk_application_new("org.jw.library", 0);
  g_signal_connect(application, "activate", (GCallback)init_window, home);
  int status = g_application_run((GApplication *)application, argc, argv);
  g_object_unref(application);
  return status;
}

void init_window(GtkApplication *application, gchar *home) {
  get_managers();
  // for(char *list:
  // gtk_icon_theme_get_search_path(gtk_icon_theme_get_for_display(display)))

  GtkWidget *window = gtk_application_window_new(application);
  GtkWidget *container = gtk_grid_new();
  init_winstack((GtkGrid *)container);
  gtk_widget_set_size_request(window, 854, 480);
  gtk_window_set_child((GtkWindow *)window, container);
  gtk_window_present((GtkWindow *)window);
}

static void get_managers() {
  const char *styles = "/com/binclab/jwlibrary/css/style.css";
  display = gdk_display_get_default();
  provider = (GtkStyleProvider *)gtk_css_provider_new();
  theme = gtk_icon_theme_get_for_display(display);
  projector = get_monitor(1);
  gtk_icon_theme_add_resource_path(theme, "/com/binclab/jwlibrary/icons/96x96");
  gtk_css_provider_load_from_resource((GtkCssProvider *)provider, styles);
  gtk_style_context_add_provider_for_display(display, provider, 600);
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
