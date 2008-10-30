#include <gtk/gtk.h>

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
  GtkWidget *fixed;
  GtkWidget *frame;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_signal_connect (G_OBJECT (window), "delete_event",
      G_CALLBACK (delete_event), NULL);

  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  fixed = gtk_fixed_new();
  gtk_container_add (GTK_CONTAINER (window), fixed);

  frame = gtk_frame_new("ikegam's test code");
  gtk_fixed_put (GTK_FIXED(fixed), frame, 10, 10);


  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}

