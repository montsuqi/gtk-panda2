#include <gtk/gtk.h>

static void destroy( GtkWidget *widget,
    gpointer   data )
{
  gtk_main_quit ();
}

int main( int   argc,
    char *argv[] )
{
  GtkWidget *window;
  GtkWidget *file;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  gtk_widget_set_size_request (window, 300, 300);

  file = gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_OPEN);

  gtk_container_add(GTK_CONTAINER(window), file);

  gtk_widget_show_all (window);

  gtk_main();

  return 0;
}
