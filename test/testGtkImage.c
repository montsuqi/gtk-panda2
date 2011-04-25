#include <gtk/gtk.h>
#include <libgnomeui/libgnomeui.h>

static void destroy( GtkWidget *widget,
    gpointer   data )
{
  gtk_main_quit ();
}

static void clicked(GtkWidget *widget,
  gpointer data)
{
  GtkImage *image;

  image = GTK_IMAGE(data);
  gtk_image_set_from_file(image, "./testGtkImage2.png");
}

int main( int   argc,
    char *argv[] )
{
  GtkWidget *window;
  GtkWidget *image;
  GtkWidget *button;
  GtkWidget *vbox;
  GtkWidget *scroll;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  gtk_widget_set_size_request (window, 300, 300);

  vbox = gtk_vbox_new(FALSE,1);
  image = gtk_image_new_from_file("./testGtkImage.png");
  scroll = gtk_scrolled_window_new(NULL,NULL);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll), image);

  button = gtk_button_new_with_label("change");
  //gtk_box_pack_start_defaults(GTK_BOX(vbox), scroll);
  gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 1);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  g_signal_connect(G_OBJECT(button), "clicked",
    G_CALLBACK(clicked), image);

  gtk_widget_show_all (window);
  fprintf(stderr,"size:[%d,%d]\n", 
    (int)((gtk_scrolled_window_get_hadjustment (GTK_SCROLLED_WINDOW(scroll)))->page_size),
    (int)((gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW(scroll)))->page_size));

  gtk_main();

  return 0;
}
