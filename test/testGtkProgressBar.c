#include <glib.h>
#include <gtk/gtk.h>

static void clicked(
    GtkWidget *widget,
    gpointer   data )
{
  gtk_progress_bar_pulse(GTK_PROGRESS_BAR(data));
}

static gboolean delete_event(
    GtkWidget *widget,
    GdkEvent  *event,
    gpointer   data )
{
  g_print ("delete event occurred\n");
  return FALSE;
}

static void destroy(
    GtkWidget *widget,
    gpointer   data )
{
  gtk_main_quit ();
}

int main(
    int   argc,
    char *argv[] )
{
  GtkWidget *window;
  GtkWidget *box1;
  GtkWidget *bar;
  GtkWidget *button;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_signal_connect (G_OBJECT (window), "delete_event",
      G_CALLBACK (delete_event), NULL);

  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  box1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), box1);

  bar = gtk_progress_bar_new();
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(bar), 1);
  button = gtk_button_new_with_label("push");

  g_signal_connect (G_OBJECT (button), "clicked",
      G_CALLBACK(clicked), bar);

  gtk_box_pack_start(GTK_BOX(box1), bar, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box1), button, TRUE, TRUE, 0);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
