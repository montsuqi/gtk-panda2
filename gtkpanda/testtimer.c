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

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  gtk_panda_init(&argc, &argv);

  window1 = gtk_dialog_new ();

  gtk_signal_connect (GTK_OBJECT (window1), "destroy",
		      GTK_SIGNAL_FUNC(gtk_widget_destroyed),
		      &window1);

  gtk_window_set_title (GTK_WINDOW (window1), "test timer");
  gtk_container_set_border_width (GTK_CONTAINER (window1), 0);
  gtk_window_set_default_size (GTK_WINDOW(window1), 200, 110);

  button = gtk_button_new_with_label ("OK");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (cb_dialog_ok_button),
		      NULL);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window1)->action_area), 
		      button, TRUE, TRUE, 0);
  gtk_widget_grab_default (button);
  gtk_widget_show (button);

  button = gtk_button_new_with_label ("Test");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (cb_dialog_test_button),
		      window1);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window1)->action_area),
			  button, TRUE, TRUE, 0);
  gtk_widget_show (button);

  label = gtk_label_new ("idle");
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window1)->vbox), 
		      label, TRUE, TRUE, 0);

  timer = gtk_panda_timer_new();
  gtk_signal_connect (GTK_OBJECT (timer), "timeout",
              GTK_SIGNAL_FUNC(cb_timer),
              &timer);
  gtk_widget_show (timer);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window1)->vbox), 
		      timer, TRUE, TRUE, 0);
  gtk_widget_show (window1);

  gtk_main ();
  return 0;
}
