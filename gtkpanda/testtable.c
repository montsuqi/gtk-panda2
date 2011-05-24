#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gtkpanda.h"

#define ROW_SIZE 100

void
reset_data(GtkPandaTable *table)
{
  static int num = 1;
  gchar *rowdata[5] = {GTK_STOCK_YES,"text","label","T",NULL};
  gchar *colors[ROW_SIZE+1];
  int i;

  for(i=0;i<ROW_SIZE;i++) {
    if (i%2==0) {
      rowdata[0] = GTK_STOCK_YES;
      rowdata[1] = g_strdup_printf("even%d",num);
      rowdata[2] = "label";
      rowdata[3] = "T";
    } else {
      rowdata[0] = GTK_STOCK_NO;
      rowdata[1] = g_strdup_printf("odd%d",num);
      rowdata[2] = "LABEL";
      rowdata[3] = "F";
    }
    gtk_panda_table_set_row(table,i,rowdata);
  }

  colors[ROW_SIZE] = NULL;
  for(i=0;i<ROW_SIZE;i++){
    if (i%2==0) {
      colors[i] = "white";
    } else {
      colors[i] = "azure";
    }
  }
  gtk_panda_table_set_row_colors(table,colors);
  num++;
}

void
cb_clicked(GtkWidget *widget,
  gpointer data)
{
  reset_data(GTK_PANDA_TABLE(data));
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

  gtk_panda_table_set_columns(GTK_PANDA_TABLE(table),4);
  gtk_panda_table_set_types(GTK_PANDA_TABLE(table),
    "icon,text,label,check");
  gtk_panda_table_set_titles(GTK_PANDA_TABLE(table),
    "icon1,text1,label1,check1");
  gtk_panda_table_set_column_widths(GTK_PANDA_TABLE(table),
    "50,100,100,50");
  gtk_panda_table_set_rows(GTK_PANDA_TABLE(table),ROW_SIZE);
  g_signal_connect (G_OBJECT(table), "cell-edited",
    G_CALLBACK(cb_cell_edited), NULL);
  reset_data(GTK_PANDA_TABLE(table));

  button = gtk_button_new_with_label("reset");
  gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);
  g_signal_connect (G_OBJECT(button), "clicked",
    G_CALLBACK(cb_clicked), table);

  gtk_widget_show_all(window);

  gtk_main ();
  return 0;
}

