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
cb_activate(GtkEntry *w,gpointer data)
{
  fprintf(stderr,"activate %s[%s]\n",
    gtk_buildable_get_name(GTK_BUILDABLE(w)),
    gtk_entry_get_text(GTK_ENTRY(w))
  );
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
  gtk_builder_add_from_file(builder, "pandaentry.ui", NULL);
  gtk_builder_connect_signals(builder, NULL);

  window1 = gtk_builder_get_object(builder, "window1");

  gtk_widget_show_all(GTK_WIDGET(window1));
  gtk_main();
  return 0;
}
