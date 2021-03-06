#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int i;
  	fprintf(stderr, "select:");
	for (i=0; i < gtk_panda_clist_get_rows(clist); i++) {
    	if (gtk_panda_clist_row_is_selected(clist, i)) {
  			fprintf(stderr, "%d ", i);
		}
	}
  	fprintf(stderr, "\n");
}

static void
unselect_row(
    GtkPandaCList   *clist,
	int				row,
	int				column,
    gpointer    	user_data)
{
  fprintf(stderr, "---- unselect row %d\n", row);
  fprintf(stderr, "is row selected? : %d\n", 
    gtk_panda_clist_row_is_selected(clist, row));
}

int
main (int argc, char *argv[])
{
  GtkWidget *window1;
  GtkWidget *vbox1;
  GtkWidget *scrolledwindow1;
  GtkWidget *clist1;
  GtkWidget *button;

  char **text;
  char str[256];
  int i, j;

  gtk_set_locale ();
  gtk_init (&argc, &argv);
  gtk_panda_init (&argc, &argv);

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_show (window1);

  vbox1 = gtk_vbox_new (FALSE, 0);
#if 0
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (window1), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
#endif
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

#define COLUMNS 3
  GtkTreeViewColumn *column;
  GtkWidget *label1, *label2;
  label1 = gtk_label_new("label1");
  label2 = gtk_label_new("label2");
  gtk_widget_show(label1);
  gtk_widget_show(label2);

  clist1 = gtk_panda_clist_new ();
  gtk_panda_clist_set_columns(GTK_PANDA_CLIST(clist1),10);


  gtk_panda_clist_set_columns(GTK_PANDA_CLIST(clist1),1);
  gtk_panda_clist_set_columns(GTK_PANDA_CLIST(clist1),5);
  gtk_panda_clist_set_columns(GTK_PANDA_CLIST(clist1),COLUMNS);
  column = gtk_tree_view_get_column(GTK_TREE_VIEW(clist1),0);
  gtk_tree_view_column_set_widget(column, label1);
  column = gtk_tree_view_get_column(GTK_TREE_VIEW(clist1),1);
  gtk_tree_view_column_set_widget(column, label2);

  gtk_widget_show_all (clist1);
  gtk_panda_clist_set_column_widths(GTK_PANDA_CLIST(clist1), "50,50,50,50,50,50,50,50");
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

  gtk_panda_clist_set_rows(GTK_PANDA_CLIST(clist1),50);
  text = malloc(sizeof(char *)*COLUMNS);
  for (i=0; i<50; i++) {
    for (j=0; j<COLUMNS; j++) {
      sprintf(str, "cel_%d_%d", i, j);
      text[j] = strdup(str);
    }
    gtk_panda_clist_set_row(GTK_PANDA_CLIST(clist1),i,text);
  }

  gtk_panda_clist_set_fgcolor(GTK_PANDA_CLIST(clist1),0,"#FF2222");
  gtk_panda_clist_set_fgcolor(GTK_PANDA_CLIST(clist1),1,"#2222FF");
  gtk_panda_clist_set_bgcolor(GTK_PANDA_CLIST(clist1),0,"#FFEEEE");
  gtk_panda_clist_set_bgcolor(GTK_PANDA_CLIST(clist1),1,"#EEEEFF");

  button = gtk_button_new_with_label("toggle sensitive");
  gtk_box_pack_start (GTK_BOX (vbox1), button, FALSE, FALSE, 0);
  gtk_widget_show_all(window1);

  gtk_main ();
  return 0;
}

