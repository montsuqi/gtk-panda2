#include <gtk/gtk.h>

static void selected(
    GtkWidget *widget,
    gpointer   data )
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget)) == 0) {
    return;
  }
  if (GTK_BIN (widget)->child)
  {
    GtkWidget *child = GTK_BIN (widget)->child;

    if (GTK_IS_LABEL (child))
    {
      gchar *text;

      gtk_label_get (GTK_LABEL (child), &text);
      g_print ("label: %s\n", text);
    }
  }
}

static gboolean delete_event(
    GtkWidget *widget,
    GdkEvent  *event,
    gpointer   data )
{
  g_print ("delete event occurred\n");
  return FALSE;
}

static void destroy( GtkWidget *widget,
    gpointer   data )
{
  gtk_main_quit ();
}

int main( int   argc,
    char *argv[] )
{
  GtkWidget *window;
  GtkWidget *box1;
  GtkWidget *button1;
  GtkWidget *button2;
  GtkWidget *button3;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_signal_connect (G_OBJECT (window), "delete_event",
      G_CALLBACK (delete_event), NULL);

  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  box1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), box1);

  button1 = gtk_radio_button_new_with_label_from_widget (
      NULL, "button1");
  button2 = gtk_radio_button_new_with_label_from_widget (
      GTK_RADIO_BUTTON(button1), "button2");
  button3 = gtk_radio_button_new_with_label_from_widget (
      GTK_RADIO_BUTTON(button2), "button3");

  g_signal_connect (G_OBJECT (button1), "clicked",
      G_CALLBACK (selected), NULL);
  g_signal_connect (G_OBJECT (button2), "clicked",
      G_CALLBACK (selected), NULL);
  g_signal_connect (G_OBJECT (button3), "clicked",
      G_CALLBACK (selected), NULL);

  gtk_box_pack_start(GTK_BOX(box1), button1, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box1), button2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box1), button3, TRUE, TRUE, 0);

  gtk_widget_show (box1);
  gtk_widget_show (button1);
  gtk_widget_show (button2);
  gtk_widget_show (button3);
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}

