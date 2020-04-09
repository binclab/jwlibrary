#include "winstack.h"

void init_winstack()
{
    winstack = gtk_stack_new();
    gtk_widget_set_size_request(winstack, 400 ,-1);
    gtk_paned_pack2((GtkPaned*)paned, winstack, TRUE, FALSE);
}
