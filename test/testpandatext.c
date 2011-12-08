#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "gtkpanda.h"
#include "debug.h"

G_MODULE_EXPORT void
cb_activate(GtkPandaText *w,gpointer data)
{
  GtkTextBuffer *buffer;
  GtkTextIter start,end;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  fprintf(stderr,"\n==== text1 activate\n");
  fprintf(stderr,"%s", gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
}

G_MODULE_EXPORT void
cb_changed(GtkPandaText *w,
  gpointer data)
{
  GtkTextBuffer *buffer;
  GtkTextIter start,end;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  fprintf(stderr,"\n==== text1 changed\n");
  fprintf(stderr,"%s", gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
}

int
main (int argc, char **argv)
{
  GtkBuilder *builder;
  GObject *window1;

  gtk_init(&argc,&argv);
  gtk_panda_init(&argc,&argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "pandatext.ui", NULL);
  gtk_builder_connect_signals(builder, NULL);

  window1 = gtk_builder_get_object(builder, "window1");

  gtk_widget_show_all(GTK_WIDGET(window1));
  gtk_main();
  return 0;
}
