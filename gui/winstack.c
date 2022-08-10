#include "winstack.h"

void init_winstack()
{
    GListModel *model = gtk_map_list_model_new
    GtkSingleSelection * selection = gtk_single_selection_new(model);
    winstack = gtk_stack_new();
    medialibrary = gtk_grid_view_new(); //gtk_grid_view_new();
    gtk_paned_set_end_child((GtkPaned*)paned, winstack);
    gtk_paned_set_shrink_end_child((GtkPaned*)paned, FALSE);
    gtk_paned_set_resize_end_child((GtkPaned*)paned, TRUE);
    gtk_stack_add_child((GtkStack*)winstack, medialibrary);
}
