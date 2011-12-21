#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "gtkpanda.h"

static GtkPandaTimer *timer1 = NULL;
static GtkButton *button1 = NULL;

G_MODULE_EXPORT void
cb_timeout(
  GtkPandaTimer *timer,
  gpointer data)
{
  printf("timeout\n");
  gtk_button_set_label(button1,"idle");
}

G_MODULE_EXPORT void
cb_clicked(
  GtkButton *w,
  gpointer data)
{
  printf("timer start\n");
  gtk_panda_timer_set(timer1,5);
  gtk_panda_timer_reset(timer1);
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
  timer1 = GTK_PANDA_TIMER(gtk_builder_get_object(builder, "pandatimer1"));
  button1 = GTK_BUTTON(gtk_builder_get_object(builder, "button1"));

  gtk_widget_show_all(GTK_WIDGET(window1));
  gtk_main();
  return 0;
}
