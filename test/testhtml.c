#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <numeric.h>
#include "gtkpanda.h"
#include "gtkpandahtml.h"
#include "debug.h"

extern	void	*
_xmalloc(
	size_t	size,
	char	*fn,
	int		line)
{
	return	(malloc(size));
}
extern	void
_xfree(
	void	*p,
	char	*fn,
	int		line)
{
	free(p);
}

#if 0
static GtkWidget *
create_window1 (char *uri)
{
  GtkWidget *window1;
  GtkWidget *table1;
  GtkWidget *html1, *html2;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (window1), "window1", window1);
  gtk_window_set_title (GTK_WINDOW (window1), "window1");
  gtk_container_border_width (GTK_CONTAINER(window1),5);

  table1 = gtk_table_new (1, 2, FALSE);

  gtk_container_add (GTK_CONTAINER (window1), table1);
  html1 = gtk_panda_html_new ();
  gtk_panda_html_set_uri (GTK_PANDA_HTML (html1), uri);
  gtk_table_attach_defaults (GTK_TABLE (table1), html1, 0, 1, 0, 1);

  html2 = gtk_panda_html_new ();
  
  gtk_panda_html_set_uri (GTK_PANDA_HTML (html2), 
    "about:config");
  gtk_table_attach_defaults (GTK_TABLE (table1), html2, 0, 1, 1, 2);

  gtk_window_set_default_size(GTK_WINDOW(window1), 300, 400);
  gtk_widget_show_all(window1);

  return window1;
}
#endif

static GtkWidget *
create_window1 (char *uri)
{
  GtkWidget *window1;
  GtkWidget *vbox;
  GtkWidget *html1;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), "window1");

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (window1), vbox);

  html1 = gtk_panda_html_new ();
  gtk_panda_html_set_uri (GTK_PANDA_HTML (html1), uri);

  gtk_box_pack_start(GTK_BOX(vbox),html1,TRUE,TRUE,0);

  gtk_window_set_default_size(GTK_WINDOW(window1), 300, 400);
  gtk_widget_show_all(window1);

  return window1;
}

int
main (int argc, char **argv)
{
  GtkWidget *window;

  if (argc != 2)
    {
      puts ("Usage: testhtml uri");
      exit (1);
    }

  gtk_init (&argc, &argv);

  gtk_panda_init(&argc,&argv);

  window = create_window1 (argv[1]);
  g_signal_connect (G_OBJECT (window), "destroy",
		      G_CALLBACK (gtk_main_quit), NULL);

  gtk_widget_show (window);
  gtk_main ();

  return 0;
}
