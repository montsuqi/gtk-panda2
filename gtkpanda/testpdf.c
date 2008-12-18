#include <gtk/gtk.h>
#include "config.h"
#include "gtkpanda.h"

static  GtkWidget *pdf;
static  GtkWidget *entry;

static void destroy( GtkWidget *widget,
    gpointer   data )
{
  gtk_main_quit ();
}

static void clicked(GtkWidget *widget,
  gpointer p)
{
	GError *error = NULL;
	gchar *data;
	gsize size = sizeof(data);

	if (g_file_get_contents (gtk_entry_get_text(GTK_ENTRY(entry)), 
		&data, &size, &error)) {
		gtk_panda_pdf_set(GTK_PANDA_PDF(pdf), (int)size, data);
		g_free(data);
	} else {
		fprintf(stderr, "error : %s\n", error->message);
		g_error_free (error);
	}
}

int main( int   argc,
    char *argv[] )
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *vbox;
  GtkWidget *hbox;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  gtk_widget_set_size_request (window, 300, 300);

  vbox = gtk_vbox_new(FALSE,1);
  pdf = gtk_panda_pdf_new();

  hbox = gtk_hbox_new(FALSE,1);
  entry = gtk_entry_new();
  button = gtk_button_new_with_label("load");
  gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);

  gtk_box_pack_start(GTK_BOX(vbox), pdf, TRUE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 1);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  g_signal_connect(G_OBJECT(button), "clicked",
    G_CALLBACK(clicked), NULL);
  g_signal_connect(G_OBJECT(entry), "activate",
    G_CALLBACK(clicked), NULL);

  gtk_widget_show_all (window);

  gtk_main();

  return 0;
}
