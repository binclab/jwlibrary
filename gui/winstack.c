#include "winstack.h"

void init_winstack()
{
    GtkStringList *model = gtk_string_list_new(NULL);
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    winstack = gtk_stack_new();
    medialibrary = gtk_grid_view_new((GtkSelectionModel*)model, factory);
    gtk_paned_set_end_child((GtkPaned*)paned, winstack);
    gtk_paned_set_shrink_end_child((GtkPaned*)paned, FALSE);
    gtk_paned_set_resize_end_child((GtkPaned*)paned, TRUE);
    gtk_stack_add_child((GtkStack*)winstack, medialibrary);
}
