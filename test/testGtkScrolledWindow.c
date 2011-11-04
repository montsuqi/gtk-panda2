#include <gtk/gtk.h>

static void destroy( GtkWidget *widget,
    gpointer   data )
{
  gtk_main_quit ();
}

int main( int   argc,
    char *argv[] )
{
  static GtkWidget *window;
  GtkWidget *scrolled_window;
  GtkWidget *table;
  GtkWidget *button;
  int i, j;

  gtk_init (&argc, &argv);

  window = gtk_dialog_new ();
  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  gtk_widget_set_size_request (window, 300, 300);

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 10);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
      GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_box_pack_start (GTK_BOX (GTK_DIALOG(window)->vbox), scrolled_window, 
      TRUE, TRUE, 0);
  gtk_widget_show (scrolled_window);

  table = gtk_table_new (10, 10, FALSE);

  gtk_table_set_row_spacings (GTK_TABLE (table), 10);
  gtk_table_set_col_spacings (GTK_TABLE (table), 10);

  gtk_scrolled_window_add_with_viewport (
      GTK_SCROLLED_WINDOW (scrolled_window), table);
  gtk_widget_show (table);

  for (i = 0; i < 10; i++)
    for (j = 0; j < 10; j++) {
      button = gtk_toggle_button_new_with_label ("ikegam's test code");
      gtk_table_attach_defaults (GTK_TABLE (table), button,
          i, i+1, j, j+1);
      gtk_widget_show (button);
    }

  gtk_widget_show_all (window);

  gtk_main();

  return 0;
}
