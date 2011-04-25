#include <glib.h>
#include <gtk/gtk.h>

static void clicked(
    GtkWidget *widget,
    gpointer   data )
{
  GList *glist = NULL;
  GtkWidget *item;

  item = gtk_list_item_new_with_label("ikegam's test code");
  gtk_list_append_items(GTK_LIST(data), g_list_append(glist, item));
  gtk_widget_show(item);
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
  GtkWidget *list;
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

  list = gtk_list_new();
  button = gtk_button_new_with_label("push");

  g_signal_connect (G_OBJECT (button), "clicked",
      G_CALLBACK(clicked), list);

  gtk_box_pack_start(GTK_BOX(box1), list, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box1), button, TRUE, TRUE, 0);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
