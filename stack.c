#include "stack.h"
#include "reader/parser.h"

void *init_winstack(GtkGrid *container) {
  GtkWidget *stack = gtk_stack_new();

  GtkWidget *revealer = gtk_revealer_new();
  GtkWidget *overlay = gtk_overlay_new();
  GtkWidget *toggle = gtk_toggle_button_new();
  init_home((GtkStack *)stack);
  init_bible((GtkStack *)stack);
  init_library((GtkStack *)stack);
  init_meetings((GtkStack *)stack);
  // gtk_widget_set_size_request(container, -1, -1);
  init_navigation(container, (GtkStack *)stack);

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
  const char *days[] = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                        "Friday", "Saturday", "Sunday"};
  const char *months[] = {"January",   "February", "March",    "April",
                               "May",       "June",     "July",     "August",
                               "September", "October",  "November", "December"};
  GDateTime *date = g_date_time_new_now_local();
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget *dailytext = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget *textbutton = gtk_button_new();
  GtkWidget *primary = gtk_image_new_from_icon_name("x-office-calendar-symbolic");
  GtkWidget *secondary = gtk_image_new_from_icon_name("go-next-symbolic");
  GtkWidget *label = gtk_label_new(NULL);
  GtkWidget *buttonbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
  GtkWidget *textview = gtk_text_view_new();
  GtkWidget *content = gtk_scrolled_window_new();
  GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *)textview);
  gchar *day = days[g_date_time_get_day_of_week(date) - 1];
  gchar *month = months[g_date_time_get_month(date) - 1];
  gchar text[strlen(day)+ strlen(month)];
  sprintf(text, "%s, %s %i", day, month, g_date_time_get_day_of_month(date));
  gtk_label_set_text((GtkLabel*)label, text);
  gtk_button_set_child((GtkButton *)textbutton, buttonbox);
  page[0] = gtk_stack_add_titled(stack, box, "Home", "Home");
  gtk_stack_page_set_icon_name(page[0], "go-home");
  gtk_widget_set_halign(textbutton, GTK_ALIGN_CENTER);
  gtk_box_append((GtkBox *)buttonbox, primary);
  gtk_box_append((GtkBox *)buttonbox, label);
  gtk_box_append((GtkBox *)buttonbox, secondary);
  gtk_box_append((GtkBox *)dailytext, textbutton);
  gtk_box_append((GtkBox *)dailytext, textview);
  gtk_box_append((GtkBox *)box, dailytext);
  gtk_box_append((GtkBox *)box, content);
  open_file("/home/bret/Downloads/es24_E.jwpub");
}

static void init_bible(GtkStack *stack) {
  GtkWidget *bible = gtk_notebook_new();
  gtk_notebook_append_page((GtkNotebook *)bible, gtk_button_new(),
                           gtk_label_new("INTRODUCTION"));
  gtk_notebook_append_page((GtkNotebook *)bible, gtk_button_new(),
                           gtk_label_new("BOOKS"));
  gtk_notebook_append_page((GtkNotebook *)bible, gtk_button_new(),
                           gtk_label_new("INDEX"));
  gtk_notebook_append_page((GtkNotebook *)bible, gtk_button_new(),
                           gtk_label_new("APPENDIX A"));
  gtk_notebook_append_page((GtkNotebook *)bible, gtk_button_new(),
                           gtk_label_new("APPENDIX B"));
  gtk_notebook_append_page((GtkNotebook *)bible, gtk_button_new(),
                           gtk_label_new("APPENDIX C"));
  gtk_notebook_set_current_page((GtkNotebook *)bible, 1);
  page[1] = gtk_stack_add_titled(stack, bible, "Bible", "Bible");
  gtk_stack_page_set_icon_name(page[1], "accessories-dictionary-symbolic");
}

static void init_library(GtkStack *stack) {
  page[2] = gtk_stack_add_titled(stack, gtk_button_new(), "Library", "Library");
  gtk_stack_page_set_icon_name(page[2], "document-open-symbolic");
}

static void init_meetings(GtkStack *stack) {
  page[3] =
      gtk_stack_add_titled(stack, gtk_button_new(), "Meetings", "Meetings");
  gtk_stack_page_set_icon_name(page[3], "avatar-default-symbolic");
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