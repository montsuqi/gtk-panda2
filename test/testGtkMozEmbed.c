#include <gtk/gtk.h>
#include <gtkmozembed.h>

static void destroy( GtkWidget *widget,
    gpointer   data )
{
  gtk_main_quit ();
}

int main( int   argc,
    char *argv[] )
{
  GtkWidget *window;
  GtkWidget *moz;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  gtk_widget_set_size_request (window, 300, 300);

  moz = gtk_moz_embed_new();
  gtk_moz_embed_load_url(GTK_MOZ_EMBED(moz), argv[1]);

  gtk_container_add(GTK_CONTAINER(window), moz);

  gtk_widget_show_all (window);

  gtk_main();

  return 0;
}

