#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "gtkpanda.h"
#include "debug.h"

static GtkPandaPixmap *pixmap1 = NULL;

G_MODULE_EXPORT void
cb_file_set(
  GtkFileChooserButton *w,
  gpointer data)
{
  gchar *buf;
  gsize size;

  if (g_file_get_contents(
    gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(w)),
    &buf,&size,NULL))
  {
    gtk_panda_pixmap_set_image(pixmap1,buf,size);
  }
  g_free(buf);
}

int
main (int argc, char **argv)
{
  GtkBuilder *builder;
  GObject *window1;
  GObject *filefilter1;

  gtk_init(&argc,&argv);
  gtk_panda_init(&argc,&argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "pandapixmap.ui", NULL);
  gtk_builder_connect_signals(builder, NULL);


  window1 = gtk_builder_get_object(builder, "window1");
  filefilter1 = gtk_builder_get_object(builder, "filefilter1");
  pixmap1 = GTK_PANDA_PIXMAP(gtk_builder_get_object(builder,"pixmap1"));

  gtk_file_filter_add_pattern(GTK_FILE_FILTER(filefilter1),"*.png");

  gtk_widget_show_all(GTK_WIDGET(window1));
  gtk_main();
  return 0;
}
