#include "winstack.h"

void init_winstack()
{
    winstack = gtk_stack_new();
    gtk_widget_set_size_request(winstack, 400 ,-1);
    gtk_paned_set_end_child((GtkPaned*)paned, winstack);
    gtk_paned_set_shrink_end_child((GtkPaned*)paned, FALSE);
    gtk_paned_set_resize_end_child((GtkPaned*)paned, TRUE);
}
