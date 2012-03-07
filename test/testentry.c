#define	DEBUG
#define	TRACE

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "gtkpanda.h"
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

static gboolean focus_in(GtkWidget *w, GdkEvent *e, gpointer data)
{
  fprintf(stderr,"focus_in __IM_STATE:%s\n", getenv("__IM_STATE"));
  return FALSE;
}
static gboolean focus_out(GtkWidget *w, GdkEvent *e, gpointer data)
{
  fprintf(stderr,"focus_out __IM_STATE:%s\n", getenv("__IM_STATE"));
  return FALSE;
}

static gboolean key_press(GtkWidget *w, GdkEventKey *e, gpointer data)
{

  fprintf(stderr, "type:%d\n", e->type);
  fprintf(stderr, "window:%p\n", e->window);
  fprintf(stderr, "send_event:%d\n", e->send_event);
  fprintf(stderr, "time:%d\n", e->time);
  fprintf(stderr, "state:%d\n", e->state);
  fprintf(stderr, "keyval:%d\n", e->keyval);
  fprintf(stderr, "length:%d\n", e->length);
  fprintf(stderr, "string:%s\n", e->string);
  fprintf(stderr, "hardware_keycode:%d\n", e->hardware_keycode);
  fprintf(stderr, "group:%d\n", e->group);
  return FALSE;
}

static GtkWidget *
create_window1 ()
{
  GtkWidget *window1;
  GtkWidget *table1;
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), "window1");

  table1 = gtk_grid_new();
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (window1), table1);
  gtk_container_set_border_width (GTK_CONTAINER (table1), 4);

  label1 = gtk_label_new ("ASCII");
  gtk_widget_show (label1);
  gtk_grid_attach (GTK_GRID (table1), label1, 0, 0, 1, 1);
  gtk_misc_set_alignment (GTK_MISC (label1), 0, 0.5);

  label2 = gtk_label_new ("KANA");
  gtk_widget_show (label2);
  gtk_grid_attach (GTK_GRID (table1), label2, 0, 1, 1, 1);
  gtk_misc_set_alignment (GTK_MISC (label2), 0, 0.5);

  label3 = gtk_label_new ("XIM");
  gtk_widget_show (label3);
  gtk_grid_attach (GTK_GRID (table1), label3, 0, 2, 1, 1);
  gtk_misc_set_alignment (GTK_MISC (label3), 0, 0.5);

  entry1 = gtk_panda_entry_new ();
  gtk_panda_entry_set_input_mode (GTK_PANDA_ENTRY (entry1),
				  GTK_PANDA_ENTRY_ASCII);
  gtk_widget_show (entry1);
  gtk_grid_attach (GTK_GRID (table1), entry1, 1, 0, 1, 1);

  entry2 = gtk_panda_entry_new ();
  gtk_panda_entry_set_input_mode (GTK_PANDA_ENTRY (entry2),
				  GTK_PANDA_ENTRY_KANA);
#if 1
  g_signal_connect (G_OBJECT(GTK_ENTRY(entry2)), "focus-in-event",
                      G_CALLBACK (focus_in),
                      NULL);
  g_signal_connect (G_OBJECT(GTK_ENTRY(entry2)), "focus-out-event",
                      G_CALLBACK (focus_out),
                      NULL);
  g_signal_connect (G_OBJECT(GTK_ENTRY(entry2)), "key-press-event",
                      G_CALLBACK (key_press),
                      NULL);
#endif
  gtk_widget_show (entry2);
  gtk_grid_attach (GTK_GRID (table1), entry2, 1, 1, 1, 1);

  entry3 = gtk_panda_entry_new ();
  gtk_panda_entry_set_input_mode (GTK_PANDA_ENTRY (entry3),
				  GTK_PANDA_ENTRY_XIM);
  gtk_panda_entry_set_xim_enabled (GTK_PANDA_ENTRY (entry3), TRUE);
  gtk_widget_show (entry3);
  gtk_grid_attach (GTK_GRID (table1), entry3, 1, 2, 1, 1);

  return window1;
}

int
main (int argc, char **argv)
{
  GtkWidget *window;

  gtk_init (&argc, &argv);

  window = create_window1 ();
  g_signal_connect (G_OBJECT (window), "destroy",
		      G_CALLBACK (gtk_main_quit), NULL);
#if 0
  gtk_panda_entry_force_feature_off();
#endif

  gtk_widget_show (window);
  gtk_main ();

  return 0;
}
