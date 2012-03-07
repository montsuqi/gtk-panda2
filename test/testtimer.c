#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <gtk/gtk.h>
#include "gtkpanda.h"

GtkWidget *label;
GtkWidget *timer;

static void
cb_dialog_ok_button(GtkButton *button, gpointer user_data)
{
  gtk_main_quit();
}

static void
cb_dialog_test_button(GtkButton *button, gpointer user_data)
{
  fprintf(stderr,"timer start\n");
  gtk_label_set_text(GTK_LABEL(label), "timout waiting ...");
  gtk_panda_timer_set(GTK_PANDA_TIMER(timer), 5);
  gtk_panda_timer_reset(GTK_PANDA_TIMER(timer));
#if 0
  //gtk_widget_hide_all(GTK_WIDGET(user_data));
  gtk_widget_hide(GTK_WIDGET(user_data));
#endif
}

void
cb_timer(GtkPandaTimer *timer, gpointer user_data)
{
  fprintf(stderr,"timeout\n");
  gtk_label_set_text(GTK_LABEL(label), "timeout");
}

int
main (int argc, char *argv[])
{
  GtkWidget *window1;
  GtkWidget *button;
  GtkWidget *action_area;
  GtkWidget *content_area;

  gtk_init (&argc, &argv);

  gtk_panda_init(&argc, &argv);

  window1 = gtk_dialog_new ();

  g_signal_connect (G_OBJECT (window1), "destroy",
		      G_CALLBACK(gtk_widget_destroyed),
		      &window1);

  gtk_window_set_title (GTK_WINDOW (window1), "test timer");
  gtk_window_set_default_size (GTK_WINDOW(window1), 200, 110);

  action_area = gtk_dialog_get_action_area(GTK_DIALOG(window1));
  content_area = gtk_dialog_get_content_area(GTK_DIALOG(window1));

  button = gtk_button_new_with_label ("OK");
  g_signal_connect (G_OBJECT (button), "clicked",
		      G_CALLBACK(cb_dialog_ok_button),
		      NULL);
  gtk_box_pack_start (GTK_BOX (action_area), 
		      button, TRUE, TRUE, 0);
  gtk_widget_show (button);

  button = gtk_button_new_with_label ("Test");
  g_signal_connect (G_OBJECT (button), "clicked",
		      G_CALLBACK (cb_dialog_test_button),
		      window1);
  gtk_box_pack_start (GTK_BOX (action_area),
			  button, TRUE, TRUE, 0);
  gtk_widget_show (button);

  label = gtk_label_new ("idle");
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (content_area), 
		      label, TRUE, TRUE, 0);

  timer = gtk_panda_timer_new();
  g_signal_connect (G_OBJECT (timer), "timeout",
              G_CALLBACK(cb_timer),
              &timer);
  gtk_widget_show (timer);
  gtk_box_pack_start (GTK_BOX (content_area), 
		      timer, TRUE, TRUE, 0);
  gtk_widget_show (window1);

  gtk_main ();
  return 0;
}
