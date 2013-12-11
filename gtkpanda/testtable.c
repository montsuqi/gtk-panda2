#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gtkpanda.h"

void
cb_clicked(GtkWidget *widget,
  gpointer data)
{
}

void
cb_cell_edited(GtkPandaTable *table,
  int row,
  int column,
  gchar *value,
  gpointer data)
{
  fprintf(stderr,"cell_edited[%d,%d][%s]\n",row,column,value);
  gtk_panda_table_set_cell_color(table,row,column,"#FF0000","#FFFF00");
  gtk_panda_table_set_xim_enabled(table,
    !gtk_panda_table_get_xim_enabled(table));
#if 1
  gtk_panda_table_moveto(table,50,1,0.5,0.5);
#endif
}

int
main (int argc, char *argv[])
{
  GtkWidget *window;
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


  table = gtk_panda_table_new();

  g_object_set(G_OBJECT(table),
    "rows",100,
    "columns",4,
    "column_types","label,text,text",
    "column_widths","150,150,150",
    "column_titles","label,text1,text2,text3",
    NULL);
  gtk_panda_table_build(GTK_PANDA_TABLE(table));
  gtk_panda_table_set_cell_text(GTK_PANDA_TABLE(table),0,0,"もげ");
  gtk_panda_table_set_cell_text(GTK_PANDA_TABLE(table),1,1,"ぬふ");
  gtk_panda_table_set_cell_text(GTK_PANDA_TABLE(table),2,2,"ぐぬ");
  gtk_box_pack_start(GTK_BOX(vbox),table,TRUE,TRUE,0);

  g_signal_connect (G_OBJECT(table), "cell-edited",
    G_CALLBACK(cb_cell_edited), NULL);

  button = gtk_button_new_with_label("reset");
  gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);
  g_signal_connect (G_OBJECT(button), "clicked",
    G_CALLBACK(cb_clicked), table);

  gtk_widget_show_all(window);

  gtk_main();
  return 0;
}

