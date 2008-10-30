#include <gtk/gtk.h>

static void select(
    GtkWidget *widget,
    gpointer   data )
{
  guint year, month, day;
  gtk_calendar_get_date (GTK_CALENDAR (widget),
    &year, &month, &day);
  g_print ("%d %d %d\n", year, month+1, day);
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
  GtkWidget *calendar;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_signal_connect (G_OBJECT (window), "delete_event",
      G_CALLBACK (delete_event), NULL);

  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  box1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), box1);

  calendar = gtk_calendar_new();

  g_signal_connect (G_OBJECT (calendar), "day_selected",
      G_CALLBACK(select), NULL);

  gtk_box_pack_start(GTK_BOX(box1), calendar, TRUE, TRUE, 0);

  gtk_widget_show (box1);
  gtk_widget_show (calendar);
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}

