#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "gtkpanda.h"

G_MODULE_EXPORT void
cb_toggled(
  GtkToggleButton *w,
  GtkPandaCList *clist)
{
  if (gtk_toggle_button_get_active(w)) {
    printf("set multi\n");
    gtk_panda_clist_set_selection_mode(GTK_PANDA_CLIST(clist),GTK_SELECTION_MULTIPLE);
  } else {
    printf("set single\n");
    gtk_panda_clist_set_selection_mode(GTK_PANDA_CLIST(clist),GTK_SELECTION_SINGLE);
  }
}

G_MODULE_EXPORT void
cb_select_row(
  GtkPandaCList *clist,
  int row,
  int col,
  gpointer data)
{
  int i;
 
  printf("select %d %d\n",row,col);
  printf("[");
  for (i=0; i < gtk_panda_clist_get_n_rows(clist); i++) {
    printf("%d,",gtk_panda_clist_row_is_selected(clist,i));
  }
  printf("]\n\n");
}

G_MODULE_EXPORT void
cb_unselect_row(
  GtkPandaCList *clist,
  int row,
  int col,
  gpointer data)
{
  int i;
 
  printf("unselect %d %d\n",row,col);
  printf("[");
  for (i=0; i < gtk_panda_clist_get_n_rows(clist); i++) {
    printf("%d,",gtk_panda_clist_row_is_selected(clist,i));
  }
  printf("]\n\n");
}

int
main (int argc, char **argv)
{
  GtkBuilder *builder;
  GObject *window1;
  GObject *pandaclist1;
  char **rowdata;
  int i,j,cols;

  gtk_init(&argc,&argv);
  gtk_panda_init(&argc,&argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "pandaclist.ui", NULL);
  gtk_builder_connect_signals(builder, NULL);

  window1 = gtk_builder_get_object(builder, "window1");
  pandaclist1 = gtk_builder_get_object(builder, "pandaclist1");

  gtk_panda_clist_clear(GTK_PANDA_CLIST(pandaclist1));
  
  cols = GTK_PANDA_CLIST(pandaclist1)->columns;
  rowdata = malloc(sizeof(char *)*cols);
  for (i=0; i<10; i++) {
    for (j=0; j<cols; j++) {
      rowdata[j] = g_strdup_printf("[%d,%d]",i,j);
    }
    gtk_panda_clist_append(GTK_PANDA_CLIST(pandaclist1), rowdata);
  }

  gtk_widget_show_all(GTK_WIDGET(window1));
  gtk_main();
  return 0;
}
