#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "gtkpanda.h"
#include "debug.h"

static GObject *numberentry1;

G_MODULE_EXPORT void
cb_format_change(GtkEntry *w,gpointer data)
{
  gtk_number_entry_set_format(
    GTK_NUMBER_ENTRY(numberentry1),
    gtk_entry_get_text(w));
}

G_MODULE_EXPORT void
cb_activate(GtkEntry *w,gpointer data)
{
  Numeric value;
  char *str;

  value = gtk_number_entry_get_value(GTK_NUMBER_ENTRY(w));
  str =  NumericOutput(value);
  printf("activate [%s]\n",str);
  NumericFree(value);
  free(str);
}

G_MODULE_EXPORT void
cb_changed(GtkEditable *w,
  gpointer data)
{
  fprintf(stderr,"changed %s[%s]\n",
    gtk_buildable_get_name(GTK_BUILDABLE(w)),
    gtk_editable_get_chars(w,0,-1)
  );
}


int
main (int argc, char **argv)
{
  GtkBuilder *builder;
  GObject *window1;

  gtk_init(&argc,&argv);
  gtk_panda_init(&argc,&argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "numberentry.ui", NULL);
  gtk_builder_connect_signals(builder, NULL);

  window1 = gtk_builder_get_object(builder, "window1");
  numberentry1 = gtk_builder_get_object(builder, "numberentry1");

  gtk_widget_show_all(GTK_WIDGET(window1));
  gtk_main();
  return 0;
}
