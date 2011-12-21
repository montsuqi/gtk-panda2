#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "gtkpanda.h"

static GtkPandaHTML *html1 = NULL;

G_MODULE_EXPORT void
cb_activate(
  GtkEntry *w,
  gpointer data)
{
  printf("load %s\n",gtk_entry_get_text(w));
  gtk_panda_html_set_uri(html1,gtk_entry_get_text(w));
}

int
main (int argc, char **argv)
{
  GtkBuilder *builder;
  GObject *window1;

  gtk_init(&argc,&argv);
  gtk_panda_init(&argc,&argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "pandahtml.ui", NULL);
  gtk_builder_connect_signals(builder, NULL);

  window1 = gtk_builder_get_object(builder, "window1");
  html1 = GTK_PANDA_HTML(gtk_builder_get_object(builder, "pandahtml1"));

  gtk_widget_show_all(GTK_WIDGET(window1));
  gtk_main();
  return 0;
}
