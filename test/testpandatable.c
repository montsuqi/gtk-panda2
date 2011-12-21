#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "gtkpanda.h"

G_MODULE_EXPORT void
cb_cell_edited(
  GtkPandaTable *table,
  int row,
  int col,
  gchar *value,
  gpointer data)
{
  printf("cell edited %d %d %s\n",row,col,value);
}

int
main (int argc, char **argv)
{
  GtkBuilder *builder;
  GObject *window1;
  GObject *pandatable1;
  char *rowdata[5] = {"","label","text","T",NULL};
  char *colors[100];
  int i,rows;

  gtk_init(&argc,&argv);
  gtk_panda_init(&argc,&argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "pandatable.ui", NULL);
  gtk_builder_connect_signals(builder, NULL);

  window1 = gtk_builder_get_object(builder, "window1");
  pandatable1 = gtk_builder_get_object(builder, "pandatable1");

  rows = GTK_PANDA_TABLE(pandatable1)->rows;

  colors[rows] = NULL;
  for (i=0; i<rows; i++) {
    if (i%2 == 0) {
      rowdata[0] = GTK_STOCK_YES;
      colors[i] = "";
    } else {
      rowdata[0] = GTK_STOCK_NO;
      colors[i] = "RoyalBlue";
    }
    gtk_panda_table_set_row(GTK_PANDA_TABLE(pandatable1),i,rowdata);
  }
  gtk_panda_table_set_bgcolors(GTK_PANDA_TABLE(pandatable1),colors);

  gtk_widget_show_all(GTK_WIDGET(window1));
  gtk_main();
  return 0;
}
