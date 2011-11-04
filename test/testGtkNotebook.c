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
  GtkWidget *box1, *box2;
  GtkWidget *label1, *label2;
  GtkWidget *note;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_signal_connect (G_OBJECT (window), "delete_event",
      G_CALLBACK (delete_event), NULL);

  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  note = gtk_notebook_new();
  gtk_container_add (GTK_CONTAINER (window), note);

  box1 = gtk_vbox_new (FALSE, 0);
  box2 = gtk_vbox_new (FALSE, 0);

  label1 = gtk_label_new("ikegam's test code label1");
  label2 = gtk_label_new("ikegam's test code label2");

  gtk_box_pack_start(GTK_BOX(box1), label1, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box2), label2, TRUE, TRUE, 0);

  gtk_notebook_append_page( GTK_NOTEBOOK(note), box1, gtk_label_new("tab1"));
  gtk_notebook_append_page( GTK_NOTEBOOK(note), box2, gtk_label_new("tab2"));


  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
