#include "winstack.h"

GtkWidget *get_navbar(GtkPaned *paned)
{
    navbar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    winstack = gtk_stack_new();
    gtk_paned_set_end_child((GtkPaned *)paned, winstack);
    init_home();
    init_bible();
    init_publications();
    init_media();
    init_meetings();
    init_wol();
    init_player();
    init_settings();
    return navbar;
}

static void init_home()
{
    page[0] = gtk_stack_add_titled((GtkStack *)winstack, gtk_button_new(), "Home", "Home");
}

static void init_bible()
{
    page[1] = gtk_stack_add_titled((GtkStack *)winstack, gtk_button_new(), "Bible", "Bible");
}

static void init_publications()
{
    page[2] = gtk_stack_add_titled((GtkStack *)winstack, gtk_button_new(), "Publications", "Publications");
}

static void init_media()
{
    page[3] = gtk_stack_add_titled((GtkStack *)winstack, gtk_button_new(), "Media", "Media");
}

static void init_meetings()
{
    page[4] = gtk_stack_add_titled((GtkStack *)winstack, gtk_button_new(), "Meetings", "Meetings");
}

static void init_wol()
{
    gchar *uri = (char *)malloc(66);
    time_t rawtime = time(NULL);
    struct tm *date = localtime(&rawtime);
    int week = (date->tm_yday - date->tm_wday + 7) / 7;
    sprintf(uri, "https://wol.jw.org/en/wol/meetings/r1/lp-e/%i/%i", date->tm_year + 1900, week);
    char *storage = (char *)malloc(strlen(home) + 10);
    printf("Time : %s\n", uri);
    GtkWidget *webview = webkit_web_view_new();
    gtk_stack_add_child((GtkStack *)winstack, webview);
    sprintf(storage, "%ssession.db", home);
    WebKitNetworkSession *session = webkit_web_view_get_network_session((WebKitWebView *)webview);
    WebKitCookieManager *cookiejar = webkit_network_session_get_cookie_manager(session);
    webkit_cookie_manager_set_persistent_storage(cookiejar, storage, 1);
    webkit_web_view_load_uri((WebKitWebView *)webview, uri);
    page[5] = gtk_stack_add_child((GtkStack *)winstack, medialibrary);
    gtk_stack_add_titled((GtkStack *)winstack, medialibrary, "WOL", "WOL");
}

static void init_player()
{
    page[6] = gtk_stack_add_titled((GtkStack *)winstack, gtk_button_new(), "Player", "Player");
}

static void init_settings()
{
    page[7] = gtk_stack_add_titled((GtkStack *)winstack, gtk_button_new(), "Settings", "Settings");
}