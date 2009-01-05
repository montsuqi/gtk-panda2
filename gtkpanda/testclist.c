#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <gtk/gtk.h>
#include "gtkpanda.h"

extern	void	*
_xmalloc(
	size_t	size,
	char	*fn,
	int		line)
{
	return	(malloc(size));
}
extern	void
_xfree(
	void	*p,
	char	*fn,
	int		line)
{
	free(p);
}

static void
select_row(
    GtkPandaCList   *clist,
	int				row,
	int				column,
    gpointer    	user_data)
{
  fprintf(stderr, "select row %d\n", row);
  fprintf(stderr, "is row selected? : %d\n", 
    gtk_panda_clist_row_is_selected(clist, row));
  fprintf(stderr, "ncolumns:%d nrows:%d\n", 
    gtk_panda_clist_get_n_columns(clist),
    gtk_panda_clist_get_n_rows(clist));
}

static void
unselect_row(
    GtkPandaCList   *clist,
	int				row,
	int				column,
    gpointer    	user_data)
{
  fprintf(stderr, "unselect row %d\n", row);
  fprintf(stderr, "is row selected? : %d\n", 
    gtk_panda_clist_row_is_selected(clist, row));
}


#define TESTGTK_CLIST_COLUMNS 3

int
main (int argc, char *argv[])
{
  GtkWidget *window1;
  GtkWidget *vbox1;
  GtkWidget *scrolledwindow1;
  GtkWidget *clist1;
  int i;
  static char text[TESTGTK_CLIST_COLUMNS][50];
  static char *texts[TESTGTK_CLIST_COLUMNS];

  gtk_set_locale ();
  gtk_init (&argc, &argv);
  gtk_panda_init (&argc, &argv);

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_show (window1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (window1), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow1);
  gtk_object_set_data_full (
    GTK_OBJECT (window1), 
    "scrolledwindow1", 
    scrolledwindow1,
    (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, TRUE, TRUE, 0);

  clist1 = gtk_panda_clist_new (TESTGTK_CLIST_COLUMNS);
  gtk_panda_clist_clear(GTK_PANDA_CLIST(clist1));

  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (clist1),
      -1,
      "label2",
      gtk_cell_renderer_text_new (),
      "text", 0,
      NULL);
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (clist1),
      -1,
      "label3",
      gtk_cell_renderer_text_new (),
      "text", 1,
      NULL);
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (clist1),
      -1,
      "label4",
      gtk_cell_renderer_text_new (),
      "text", 2,
      NULL);

  gtk_widget_show (clist1);
  gtk_panda_clist_set_column_width (GTK_PANDA_CLIST (clist1), 0, 50);
  gtk_panda_clist_set_column_width (GTK_PANDA_CLIST (clist1), 1, 100);
  gtk_panda_clist_set_column_width (GTK_PANDA_CLIST (clist1), 2, 800);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), clist1);

#if 1
  gtk_panda_clist_set_selection_mode (GTK_PANDA_CLIST (clist1),
				  GTK_SELECTION_MULTIPLE);
#else
  gtk_panda_clist_set_selection_mode (GTK_PANDA_CLIST (clist1),
				  GTK_SELECTION_SINGLE);
#endif

  g_signal_connect (G_OBJECT(clist1), "select_row",
      G_CALLBACK(select_row), clist1);
  g_signal_connect (G_OBJECT(clist1), "unselect_row",
      G_CALLBACK(unselect_row), clist1);

  for (i = 0; i < TESTGTK_CLIST_COLUMNS; i++){
      texts[i] = text[i];
      sprintf (text[i], "Column %d", i);
  }

  for (i = 0; i < 100; i++){
      sprintf (text[0], "CListRow %d", i);
      gtk_panda_clist_append (GTK_PANDA_CLIST (clist1), texts);
  }

  gtk_panda_clist_clear(GTK_PANDA_CLIST(clist1));

  for (i = 0; i < TESTGTK_CLIST_COLUMNS; i++){
      texts[i] = text[i];
      sprintf (text[i], "Column %d", i);
  }

  for (i = 0; i < 10; i++){
      sprintf (text[0], "CListRow %d", i);
      gtk_panda_clist_append (GTK_PANDA_CLIST (clist1), texts);
  }

  gtk_main ();
  return 0;
}

