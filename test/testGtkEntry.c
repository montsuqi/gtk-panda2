#include <gtk/gtk.h>

static void activate(
    GtkWidget *widget,
    gpointer   data )
{
  g_print ("%s\n", gtk_entry_get_text(GTK_ENTRY(widget)));
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
  GtkWidget *entry;
  GtkWidget *label;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_signal_connect (G_OBJECT (window), "delete_event",
      G_CALLBACK (delete_event), NULL);

  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  box1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), box1);

  entry = gtk_entry_new();
  label = gtk_label_new("press enter");

  g_signal_connect (G_OBJECT (entry), "activate",
      G_CALLBACK(activate), NULL);

  gtk_box_pack_start(GTK_BOX(box1), label, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box1), entry, TRUE, TRUE, 0);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}

