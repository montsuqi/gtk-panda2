#include <gtk/gtk.h>

#define  COL_NAME  0
#define  COL_AGE   1
#define  NUM_COLS  2

static void clicked (
    GtkWidget *widget,
    gpointer data)
{
  GtkListStore  *store;
  GtkTreeIter    iter;

  store =  GTK_LIST_STORE(gtk_tree_view_get_model (data));

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter,
      COL_NAME, "novice",
      COL_AGE, 21,
      -1);
}

static GtkTreeModel *create_and_fill_model (void)
{
  GtkListStore  *store;
  GtkTreeIter    iter;

  store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter,
      COL_NAME, "Boss",
      COL_AGE, 35,
      -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter,
      COL_NAME, "Leader",
      COL_AGE, 28,
      -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter,
      COL_NAME, "senior",
      COL_AGE, 25,
      -1);

  return GTK_TREE_MODEL (store);
}

static GtkWidget *create_view_and_model (void)
{
  GtkCellRenderer     *renderer;
  GtkTreeModel        *model;
  GtkWidget           *view;

  view = gtk_tree_view_new ();

  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
      -1,      
      "Name",  
      renderer,
      "text", COL_NAME,
      NULL);

  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
      -1,
      "Age",  
      renderer,
      "text", COL_AGE,
      NULL);

  model = create_and_fill_model ();

  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  g_object_unref (model);

  return view;
}

static void columns_changed(GtkTreeView *tree_view,
                            gpointer     user_data)
{
  printf("columns-changed\n");
}

static void cursor_changed(GtkTreeView *tree_view,
                           GtkTreeIter *iter,
                           GtkTreePath *path,
                           gpointer     user_data)
{
  printf("cursor-changed\n");
}

static gboolean expand_collapse_cursor_row(GtkTreeView *tree_view,
                                       gboolean     arg1,
                                       gboolean     arg2,
                                       gboolean     arg3,
                                       gpointer     user_data)
{
  printf("expand-collapse-cursor-row\n");
  return TRUE;
}

static gboolean move_cursor(GtkTreeView *tree_view,
                        GtkMovementStep arg1,
                        gint            arg2,
                        gpointer        user_data)
{
  printf("move-cursor\n");
  return TRUE;
}

static void row_activated(GtkTreeView *tree_view,
                          GtkTreePath       *path,
                          GtkTreeViewColumn *column,
                          gpointer           user_data)
{
  printf("row-activated\n");
}

static void row_collapsed(GtkTreeView *tree_view,
                          GtkTreeIter *iter,
                          GtkTreePath *path,
                          gpointer     user_data)
{
  printf("row-collapsed\n");
}

static void row_expanded(GtkTreeView *tree_view,
                         GtkTreeIter *iter,
                         GtkTreePath *path,
                         gpointer     user_data) 
{
  printf("row-expanded\n");
}

static gboolean select_all(GtkTreeView *tree_view,
                       gpointer     user_data)
{
  printf("select-all\n");
  return TRUE;
}

static gboolean select_cursor_parent(GtkTreeView *tree_view,
                                 gpointer     user_data)
{
  printf("select-cursor-parent\n");
  return TRUE;
}

static gboolean select_cursor_row(GtkTreeView *tree_view,
                              gboolean     arg1,
                              gpointer     user_data) 
{
  printf("select-cursor-row\n");
  return TRUE;
}

static void set_scroll_adjustments(GtkTreeView *tree_view,
                                   GtkAdjustment *arg1,
                                   GtkAdjustment *arg2,
                                   gpointer       user_data)
{
  printf("set-scroll-adjustments\n");
}

static gboolean start_interactive_search(GtkTreeView *tree_view,
                                 gpointer     user_data)
{
  printf("start-interactive-search\n");
  return TRUE;
}

static gboolean test_collapse_row(GtkTreeView *tree_view,
                              GtkTreeIter *iter,
                              GtkTreePath *path,
                              gpointer     user_data) 
{
  printf("test-collapse-row\n");
  return TRUE;
}


static gboolean test_expand_row(GtkTreeView *tree_view,
                            GtkTreeIter *iter,
                            GtkTreePath *path,
                            gpointer     user_data)
{
  printf("test-expand-row\n");
  return TRUE;
}

static gboolean toggle_cursor_row(GtkTreeView *tree_view,
                              gpointer     user_data) 
{
  printf("toggle-cursor-row\n");
  return TRUE;
}

static gboolean unselect_all(GtkTreeView *tree_view,
                         gpointer     user_data) 
{
  printf("unselect-all\n");
  return TRUE;
}

static GList *prev_selection = NULL;

static void preserve_selection(GList *new)
{
  if (prev_selection != NULL) {
    g_list_foreach (prev_selection, (GFunc)gtk_tree_path_free, NULL);
    g_list_free (prev_selection);
  }
  prev_selection = new;
}

static void selection_changed(GtkTreeSelection *selection,
  gpointer user_data)
{
  GList *current_selection;
  guint curlen;
  guint prelen;
  GtkTreePath *cpath, *ppath;
  int i,j;
  gboolean emit_select;
  gboolean emit_unselect;

  emit_select = emit_unselect = FALSE;

  current_selection = gtk_tree_selection_get_selected_rows (selection, NULL);
  if(current_selection == NULL) {
    emit_unselect = TRUE;
  } else {
    if(prev_selection != NULL) {
      prelen = g_list_length(prev_selection);
      curlen = g_list_length(current_selection);
      if (curlen == prelen){
        for(i = 0; i < curlen;  i++) {
          cpath = (GtkTreePath *)g_list_nth_data(current_selection, i);
          for( j = 0; j < prelen; j++) {
            ppath = (GtkTreePath *)g_list_nth_data(prev_selection, j);
            if(!gtk_tree_path_compare(cpath,ppath))
              break;
          }
          if (j == prelen)
            emit_select = TRUE;
        }
      } else if(curlen > prelen) {
        emit_select = TRUE;
      } else {
        emit_unselect = TRUE;
      }
    } else {
      emit_select = TRUE;
    }
  }
  preserve_selection(current_selection);
  if (emit_select) {
    printf("select\n");
    return;
  } else if(emit_unselect) {
    printf("unselect\n");
  }
}

int main (
    int argc,
    char **argv)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *box;
  GtkWidget *view;
  GtkTreeSelection *selection;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  box = gtk_vbox_new(FALSE,3);
  button = gtk_button_new_with_label("push!");
  view = create_view_and_model ();
  selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(view));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
  //gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);

  g_signal_connect (window, "delete_event",
      gtk_main_quit, NULL);
  g_signal_connect (GTK_OBJECT(button), "clicked",
      G_CALLBACK(clicked), view);

  // tree view signal
  g_signal_connect (G_OBJECT(view), "columns-changed",
      G_CALLBACK(columns_changed), NULL);
  g_signal_connect (G_OBJECT(view), "cursor-changed",
      G_CALLBACK(cursor_changed), NULL);
  g_signal_connect (G_OBJECT(view), "expand-collapse-cursor-row",
      G_CALLBACK(  expand_collapse_cursor_row ), NULL);
  g_signal_connect (G_OBJECT(view),"move-cursor" ,
      G_CALLBACK( move_cursor  ), NULL);
  g_signal_connect (G_OBJECT(view),"row-activated"     ,
      G_CALLBACK( row_activated  ), NULL);
  g_signal_connect (G_OBJECT(view),"row-collapsed" ,
      G_CALLBACK( row_collapsed  ), NULL);
  g_signal_connect (G_OBJECT(view),"row-expanded"     ,
      G_CALLBACK(  row_expanded ), NULL);
  g_signal_connect (G_OBJECT(view), "select-all"    ,
      G_CALLBACK(  select_all ), NULL);
  g_signal_connect (G_OBJECT(view),"select-cursor-parent" ,
      G_CALLBACK(select_cursor_parent   ), NULL);
  g_signal_connect (G_OBJECT(view), "select-cursor-row"    ,
      G_CALLBACK( select_cursor_row  ), NULL);
  g_signal_connect (G_OBJECT(view), "set-scroll-adjustments"   ,
      G_CALLBACK( set_scroll_adjustments  ), NULL);
  g_signal_connect (G_OBJECT(view),"start-interactive-search"    ,
      G_CALLBACK(start_interactive_search), NULL);
  g_signal_connect (G_OBJECT(view),"test-collapse-row"    ,
      G_CALLBACK( test_collapse_row  ), NULL);
  g_signal_connect (G_OBJECT(view), "test-expand-row"    ,
      G_CALLBACK(test_expand_row   ), NULL);
  g_signal_connect (G_OBJECT(view), "toggle-cursor-row"       ,
      G_CALLBACK( toggle_cursor_row  ), NULL);
  g_signal_connect (G_OBJECT(view),  "unselect-all" ,
      G_CALLBACK(  unselect_all ), NULL);

  // tree view selection signal
  g_signal_connect (G_OBJECT(selection),  "changed" ,
      G_CALLBACK(selection_changed), view);

  gtk_box_pack_start (GTK_BOX(box), view, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX(box), button, TRUE, TRUE, 0);

  gtk_container_add (GTK_CONTAINER (window), box);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}

