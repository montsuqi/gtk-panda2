#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "config.h"
#include "gtkpanda.h"

static void destroy( GtkWidget *widget,
    gpointer   data )
{
  gtk_main_quit ();
}

int main( int   argc,
    char *argv[] )
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *fc1;
  GtkWidget *fc2;

  char buf[256];

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (G_OBJECT (window), "destroy",
      G_CALLBACK (destroy), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  gtk_widget_set_size_request (window, 300, 300);

  vbox = gtk_vbox_new(FALSE,1);

  fc1 = gtk_panda_fileentry_new();
  gtk_panda_fileentry_set_mode(GTK_PANDA_FILEENTRY(fc1),
    GTK_FILE_CHOOSER_ACTION_OPEN);

  fc2 = gtk_panda_fileentry_new();
  gtk_panda_fileentry_set_mode(GTK_PANDA_FILEENTRY(fc2),
    GTK_FILE_CHOOSER_ACTION_SAVE);
  sprintf(buf, "this is test.");
  gtk_entry_set_text(GTK_ENTRY(GTK_PANDA_FILEENTRY(fc2)->entry), 
    "test.txt");
  gtk_panda_fileentry_set_data(GTK_PANDA_FILEENTRY(fc2), 
    strlen(buf), buf);

  gtk_box_pack_start(GTK_BOX(vbox), fc1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), fc2, FALSE, FALSE, 1);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  gtk_widget_show_all (window);

  gtk_main();

  return 0;
}
