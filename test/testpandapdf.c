#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "gtkpanda.h"

G_MODULE_EXPORT void
cb_file_set(
  GtkFileChooserButton *w,
  GtkPandaPDF *pdf)
{
  gchar *buf;
  gsize size;

  if (g_file_get_contents(
    gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(w)),
    &buf,&size,NULL))
  {
    gtk_panda_pdf_set(pdf,size,buf);
  }
  g_free(buf);
}

int
main (int argc, char **argv)
{
  GtkBuilder *builder;
  GObject *window1;

  gtk_init(&argc,&argv);
  gtk_panda_init(&argc,&argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "pandapdf.ui", NULL);
  gtk_builder_connect_signals(builder, NULL);

  window1 = gtk_builder_get_object(builder, "window1");

  gtk_widget_show_all(GTK_WIDGET(window1));
  gtk_main();
  return 0;
}
