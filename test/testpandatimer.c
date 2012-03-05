#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "gtkpanda.h"

G_MODULE_EXPORT void
cb_timeout(
  GtkPandaTimer *timer,
  GtkButton *button)
{
  printf("timeout\n");
  gtk_button_set_label(button,"idle");
}

G_MODULE_EXPORT void
cb_clicked(
  GtkButton *w,
  GtkPandaTimer *timer)
{
  printf("timer start\n");
  gtk_panda_timer_set(timer,5);
  gtk_panda_timer_reset(timer);
  gtk_button_set_label(w,"timeout waiting ...");
}

int
main (int argc, char **argv)
{
  GtkBuilder *builder;
  GObject *window1;

  gtk_init(&argc,&argv);
  gtk_panda_init(&argc,&argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "pandatimer.ui", NULL);
  gtk_builder_connect_signals(builder, NULL);

  window1 = gtk_builder_get_object(builder, "window1");

  gtk_widget_show_all(GTK_WIDGET(window1));
  gtk_main();
  return 0;
}
