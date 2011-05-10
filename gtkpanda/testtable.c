#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gtkpanda.h"


void
reset_table(GtkPandaTable *table)
{
  gchar *data[6] = { "white","icon", "text","label","check","white" };
  int i;

  gtk_panda_table_clear(table);
  for(i=0;i<100;i++) {
    if (i %2 ==0) {
      data[1] = (gchar*)GTK_STOCK_BOLD;
      data[4] = "True";
      data[5] = "white";
    } else {
      data[1] = (gchar*)GTK_STOCK_CONVERT;
      data[4] = "False";
      data[5] = "lightgray";
    }
    gtk_panda_table_append(table,data);
  }
}

void
cb_clicked(GtkWidget *widget,
  gpointer data)
{
  reset_table(GTK_PANDA_TABLE(data));
}

void
cb_cell_edited(GtkPandaTable *table,
  int row,
  int column,
  gchar *value,
  gpointer data)
{
  fprintf(stderr,"[%d,%d][%s]\n",row,column,value);
}

int
main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *scroll;
  GtkWidget *vbox;
  GtkWidget *table;
  GtkWidget *button;

  gtk_set_locale ();
  gtk_init (&argc, &argv);
  gtk_panda_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request (window, 300, 500);

  vbox = gtk_vbox_new(FALSE,0);
  gtk_container_add(GTK_CONTAINER(window),vbox);

  scroll = gtk_scrolled_window_new(NULL,NULL);
  gtk_box_pack_start(GTK_BOX(vbox),scroll,TRUE,TRUE,0);

  table = gtk_panda_table_new();
  gtk_container_add(GTK_CONTAINER(scroll),table);

  gtk_panda_table_set_columns(GTK_PANDA_TABLE(table),6);
  gtk_panda_table_set_titles(GTK_PANDA_TABLE(table),
    "icon1,text1,label1,check1");
  gtk_panda_table_set_types(GTK_PANDA_TABLE(table),
    "color,icon,text,label,check,color");
  reset_table(GTK_PANDA_TABLE(table));
  g_signal_connect (G_OBJECT(table), "cell-edited",
    G_CALLBACK(cb_cell_edited), NULL);

  button = gtk_button_new_with_label("reset");
  gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);
  g_signal_connect (G_OBJECT(button), "clicked",
    G_CALLBACK(cb_clicked), table);

  gtk_widget_show_all(window);

  gtk_main ();
  return 0;
}

