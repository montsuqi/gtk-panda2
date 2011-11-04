#include <gtk/gtk.h>
#include <poppler.h>

#include <libgen.h>
#include <unistd.h>

#include <limits.h>
#include <stdlib.h>


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
  GtkWidget *img;
  PopplerDocument *doc;
  PopplerPage *page;
  GdkPixbuf *buf;
  GError *error = NULL;
  double doc_w, doc_h;
  char *filename = NULL;

  filename = dirname(realpath (argv[0], filename));
  sprintf(filename, "%s/%s", filename, "testPoppler.pdf");
  filename = g_filename_to_uri (filename, NULL, &error);

  g_print("%s\n", filename);

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_signal_connect (G_OBJECT (window), "delete_event",
      G_CALLBACK (delete_event), NULL);
  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  doc  = poppler_document_new_from_file(filename,NULL,&error);
  page = poppler_document_get_page(doc, 0);
  poppler_page_get_size(page, &doc_w, &doc_h);

  buf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, (int)doc_w, (int)doc_h );
  poppler_page_render_to_pixbuf(page, 0, 0, (int)doc_w, (int)doc_h, 1.0, 0, buf);
  img = gtk_image_new_from_pixbuf(buf);

  gtk_container_add (GTK_CONTAINER (window), img);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
