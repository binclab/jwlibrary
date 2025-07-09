#include "home.h"
#include "reader/parser.h"

void init_home(GtkStack *stack)
{
    const char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday",
                          "Friday", "Saturday", "Sunday"};
    const char *months[] = {"January", "February", "March", "April",
                            "May", "June", "July", "August",
                            "September", "October", "November", "December"};
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
    GtkWidget *homestack = gtk_stack_new();
    GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *)textview);
    Data data = {.stack = (GtkStack *)homestack, .day = g_date_time_get_day_of_year(date), .buffer = buffer};
    gchar *day = days[g_date_time_get_day_of_week(date) - 1];
    gchar *month = months[g_date_time_get_month(date) - 1];
    gchar text[strlen(day) + strlen(month)];
    sprintf(text, "%s, %s %i", day, month, g_date_time_get_day_of_month(date));
    gtk_label_set_text((GtkLabel *)label, text);
    gtk_button_set_child((GtkButton *)textbutton, buttonbox);
    gtk_widget_set_halign(textbutton, GTK_ALIGN_CENTER);
    gtk_box_append((GtkBox *)buttonbox, primary);
    gtk_box_append((GtkBox *)buttonbox, label);
    gtk_box_append((GtkBox *)buttonbox, secondary);
    gtk_box_append((GtkBox *)dailytext, textbutton);
    gtk_box_append((GtkBox *)dailytext, textview);
    gtk_box_append((GtkBox *)box, dailytext);
    gtk_box_append((GtkBox *)box, content);

    GtkStackPage *homepage = gtk_stack_add_named(data.stack, box, "Home");
    page[0] = gtk_stack_add_titled(stack, homestack, "Home", "Home");
    gtk_stack_page_set_icon_name(page[0], "go-home");
    data.path = malloc(strlen(home) + 24);
    sprintf(data.path, "%slibrary/es24_E/es24_E.db", home);
    if (g_file_query_exists(g_file_new_for_path(data.path), NULL))
    {
        load_dailytext(&data);
    }
    else
    {
        pthread_t thread;
        pthread_create(&thread, NULL, extract_dailytext, (void *)&data);
    }

    g_signal_connect(textbutton, "clicked", (GCallback)show_dailytext, homestack);
}

static void load_dailytext(Data *data)
{
    Publication dailytext;
    if (sqlite3_open(data->path, &dailytext.database) == 0)
    {
        char *sql = malloc(96);
        char *selection = "SELECT MepsLanguageIndex, Symbol, Year, IssueTagNumber";
        sprintf(sql, "%s FROM Publication WHERE PublicationId = 1", selection);
        setup_publication(&dailytext, sql);
        free(sql);
        sql = malloc(55);
        sprintf(sql, "SELECT Content FROM DatedText WHERE DatedTextId = %i;", data->day);
        init_dailytext(data->stack, get_content(&dailytext, sql, data->buffer));
        free(sql);
    }
    free(data->path);
}

static void *extract_dailytext(void *data)
{
    char *prefix = "https://app.jw-cdn.org/catalogs/publications/v5/";
    char uri[62];
    GError *error = NULL;
    sprintf(uri, "%smanifest.json", prefix);
    SoupSession *session = soup_session_new();
    SoupMessage *message = soup_message_new(SOUP_METHOD_GET, uri);
    GInputStream *stream = soup_session_send(session, message, NULL, &error);
    JsonParser *parser = json_parser_new();
    if (stream && (json_parser_load_from_stream(parser, stream, NULL, &error)))
    {
        JsonNode *node = json_parser_get_root(parser);
        JsonObject *object = json_node_get_object(node);
        char *hash = json_object_get_string_member(object, "current");
        char url[strlen(prefix) + strlen(hash) + 15];
        sprintf(url, "%s%s/catalog.db.zip", prefix, hash);
        g_input_stream_close(stream, NULL, NULL);
        message = soup_message_new(SOUP_METHOD_GET, url);
        stream = soup_session_send(session, message, NULL, &error);
        SoupMessageHeaders *headers = soup_message_get_response_headers(message);
        gsize length = soup_message_headers_get_content_length(headers);
        zip_t *archive;
        char *buffer = malloc(length);
        g_input_stream_read_all(stream, buffer, length, &length, NULL, NULL);
        zip_source_t *source = zip_source_buffer_create(buffer, length, 1, NULL);
        if ((archive = zip_open_from_source(source, ZIP_RDONLY, NULL)))
        {
            extract_contents(NULL, 1, archive);
        }
        g_input_stream_close(stream, NULL, NULL);
        free(buffer);
        // stream = soup_session_send(session, message, NULL, &error);
        // message = soup_message_new(SOUP_METHOD_GET, url);
        /// stream soup_session_send_finish ();
        // GBytes *bytes = soup_session_send_and_read(session, message, NULL,
        // &error); gsize length = g_bytes_get_size(bytes); zip_source_t *source =
        // zip_source_buffer_create(bytes, length, 1, &error);
        /*
        gsize length;
        if (g_input_stream_read_all(stream, content, length, &length, NULL, NULL))
        { printf("%s\n", content); char database[strlen(home) + 25];
          sprintf(database, "%slibrary/es24_E/es24_E.db", home);
          GFile *gfile = g_file_new_for_path(database);
          if (!g_file_query_exists(gfile, NULL)) {
            open_file("/home/bret/Downloads/es24_E.jwpub");
          }*/
        // headers = soup_message_get_response_headers(message);
        // goffset length = soup_message_headers_get_content_length(headers);
        // printf("%i\n", length);
    }
    if (error)
        g_error_free(error);
    load_dailytext((Data *)data);
}

static void init_dailytext(GtkStack *stack, const gchar *content)
{
    GtkWidget *webview = webkit_web_view_new();
    GtkStackPage *page = gtk_stack_add_named(stack, webview, "Daily Text");
    webkit_web_view_load_html((WebKitWebView *)webview, content, NULL);
}

static void show_dailytext(GtkButton *button, GtkStack *stack)
{
    gtk_stack_set_visible_child_name(stack, "Daily Text");
}