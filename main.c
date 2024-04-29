#include "main.h"

int main(int argc, char *argv[])
{
    int status;
    home = (gchar *)malloc(strlen(g_get_home_dir()) + 24);
    sprintf(home, "%s/.local/share/jwlibrary/", g_get_home_dir());
    application = gtk_application_new("org.jw.library", 0);
    g_signal_connect(application, "activate", (GCallback)init_window, home);
    status = g_application_run((GApplication *)application, argc, argv);
    g_object_unref(application);
    return status;
}

void quit()
{
    g_application_quit(gapp);
    g_object_unref(projector);
}
