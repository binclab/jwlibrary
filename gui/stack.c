#include "stack.h"

void *init_winstack(GtkGrid *container) {
  GtkWidget *stack = gtk_stack_new();

  GtkWidget *revealer = gtk_revealer_new();
  GtkWidget *overlay = gtk_overlay_new();
  GtkWidget *toggle = gtk_toggle_button_new();
  init_home((GtkStack *)stack);
  GtkStackPage *page1 = gtk_stack_add_titled(
      (GtkStack *)stack, gtk_button_new(), "Bible", "Bible");
  GtkStackPage *page2 = gtk_stack_add_titled(
      (GtkStack *)stack, gtk_button_new(), "Child 1", "Child 1");
  GtkStackPage *page3 = gtk_stack_add_titled(
      (GtkStack *)stack, gtk_button_new(), "Child 1", "Child 1");
  // gtk_widget_set_size_request(container, -1, -1);
  init_navigation(container, (GtkStack *)stack);
  gtk_stack_page_set_icon_name((GtkStackPage *)page1,
                               "accessories-dictionary-symbolic");
  gtk_stack_page_set_icon_name((GtkStackPage *)page2, "document-open-symbolic");
  gtk_stack_page_set_icon_name((GtkStackPage *)page3, "user-trash-symbolic");

  gtk_overlay_set_child((GtkOverlay *)overlay, stack);
  GtkWidget *hamburger = gtk_toggle_button_new();
  // gtk_widget_set_size_request(pane2, 0, -1);

  gtk_widget_set_size_request(revealer, 100, -1);
  gtk_revealer_set_transition_type((GtkRevealer *)revealer,
                                   GTK_REVEALER_TRANSITION_TYPE_SLIDE_RIGHT);

  g_signal_connect(hamburger, "toggled", (GCallback)show_sidebar, revealer);
  gtk_widget_set_halign(revealer, GTK_ALIGN_START);
  gtk_widget_set_visible(revealer, FALSE);
  gtk_revealer_set_child((GtkRevealer *)revealer,
                         gtk_button_new_with_label("Reveal"));
  gtk_button_set_icon_name((GtkButton *)hamburger, "open-menu");
  gtk_grid_attach((GtkGrid *)container, hamburger, 0, 0, 1, 1);

  gtk_widget_set_vexpand(overlay, TRUE);
  gtk_overlay_add_overlay((GtkOverlay *)overlay, revealer);
  gtk_grid_attach((GtkGrid *)container, overlay, 1, 1, 1, 1);
}

static void show_sidebar(GtkToggleButton *button, GtkWidget *revealer) {
  gboolean toggled = gtk_toggle_button_get_active(button);
  gtk_widget_set_visible(revealer, toggled);
  gtk_revealer_set_reveal_child((GtkRevealer *)revealer, toggled);
}

static void init_home(GtkStack *stack) {
  page[0] = gtk_stack_add_titled(stack, gtk_button_new(), "Home", "Home");
  gtk_stack_page_set_icon_name(page[0], "nav-home");
}

static void init_bible(GtkStack *stack) {
  page[1] = gtk_stack_add_titled((GtkStack *)stack, gtk_button_new(), "Bible",
                                 "Bible");
}

static void init_publications(GtkStack *stack) {
  page[2] = gtk_stack_add_titled((GtkStack *)stack, gtk_button_new(),
                                 "Publications", "Publications");
}

static void init_media(GtkStack *stack) {
  page[3] = gtk_stack_add_titled((GtkStack *)stack, gtk_button_new(), "Media",
                                 "Media");
}

static void init_meetings(GtkStack *stack) {
  page[4] = gtk_stack_add_titled((GtkStack *)stack, gtk_button_new(),
                                 "Meetings", "Meetings");
}

static void init_wol(GtkStack *stack) {
  GDateTime *date = g_date_time_new_now_local();
  // gchar *week = g_date_time_format(date, "%V");
  gint week = g_date_time_get_week_of_year(date);
  gint year = g_date_time_get_year(date);
  gchar *uri = malloc(sizeof(char) * 50);
  sprintf(uri, "https://wol.jw.org/en/wol/meetings/r1/lp-e/%i/%i", year, week);
  char *storage = (char *)malloc(strlen(home) + 10);
  printf("Time : %s\n", uri);
  GtkWidget *webview = webkit_web_view_new();
  gtk_stack_add_child((GtkStack *)stack, webview);
  sprintf(storage, "%ssession.db", home);
  WebKitNetworkSession *session =
      webkit_web_view_get_network_session((WebKitWebView *)webview);
  WebKitCookieManager *cookiejar =
      webkit_network_session_get_cookie_manager(session);
  webkit_cookie_manager_set_persistent_storage(cookiejar, storage, 1);
  webkit_web_view_load_uri((WebKitWebView *)webview, uri);
  page[5] = gtk_stack_add_child((GtkStack *)stack, webview);
  gtk_stack_add_titled((GtkStack *)stack, webview, "WOL", "WOL");
}

static void init_player(GtkStack *stack) {
  page[6] = gtk_stack_add_titled((GtkStack *)stack, gtk_button_new(), "Player",
                                 "Player");
}

static void init_settings(GtkStack *stack) {
  page[7] = gtk_stack_add_titled((GtkStack *)stack, gtk_button_new(),
                                 "Settings", "Settings");
}