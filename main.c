#include "main.h"

void activate()
{
    init_window();
}

int main(int argc, char *argv[])
{
    int status;
    jwlibrary = gtk_application_new("org.jw.library.linux", 0);
    gapp = (GApplication*)jwlibrary;
    g_signal_connect(jwlibrary, "activate", (GCallback)activate, NULL);
    status = g_application_run(gapp, argc, argv);
    g_object_unref(jwlibrary);
    return status;
}

void quit()
{
    g_application_quit(gapp);
    g_object_unref(projector);
}
