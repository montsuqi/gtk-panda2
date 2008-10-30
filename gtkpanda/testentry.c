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

static gint timer = 0;

static void on_changed (GtkPandaEntry *entry);

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

static gint
print_text (gpointer entry)
{
  const gchar *text = gtk_entry_get_text (GTK_ENTRY (entry));
  int len = strlen (text);
  if (text[len - 1] >= 0x80)
    puts (text);
  timer = 0;
  return FALSE;
}

static void
on_changed (GtkPandaEntry *entry)
{
  if (timer)
    gtk_timeout_remove (timer);
  timer = gtk_timeout_add (500, print_text, entry);
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
  gtk_object_set_data (GTK_OBJECT (window1), "window1", window1);
  gtk_window_set_title (GTK_WINDOW (window1), "window1");

  table1 = gtk_table_new (3, 2, FALSE);
  gtk_widget_ref (table1);
  gtk_object_set_data_full (GTK_OBJECT (window1), "table1", table1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (window1), table1);
  gtk_container_set_border_width (GTK_CONTAINER (table1), 4);
  gtk_table_set_row_spacings (GTK_TABLE (table1), 4);
  gtk_table_set_col_spacings (GTK_TABLE (table1), 4);

  label1 = gtk_label_new ("ASCII");
  gtk_widget_ref (label1);
  gtk_object_set_data_full (GTK_OBJECT (window1), "label1", label1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label1);
  gtk_table_attach (GTK_TABLE (table1), label1, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label1), 0, 0.5);

  label2 = gtk_label_new ("KANA");
  gtk_widget_ref (label2);
  gtk_object_set_data_full (GTK_OBJECT (window1), "label2", label2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label2);
  gtk_table_attach (GTK_TABLE (table1), label2, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label2), 0, 0.5);

  label3 = gtk_label_new ("XIM");
  gtk_widget_ref (label3);
  gtk_object_set_data_full (GTK_OBJECT (window1), "label3", label3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label3);
  gtk_table_attach (GTK_TABLE (table1), label3, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label3), 0, 0.5);

  entry1 = gtk_panda_entry_new ();
  gtk_panda_entry_set_input_mode (GTK_PANDA_ENTRY (entry1),
				  GTK_PANDA_ENTRY_ASCII_MODE);
  gtk_widget_ref (entry1);
  gtk_object_set_data_full (GTK_OBJECT (window1), "entry1", entry1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry1);
  gtk_table_attach (GTK_TABLE (table1), entry1, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  entry2 = gtk_panda_entry_new ();
  gtk_panda_entry_set_input_mode (GTK_PANDA_ENTRY (entry2),
				  GTK_PANDA_ENTRY_KANA_MODE);
  gtk_signal_connect (GTK_OBJECT (entry2), "changed",
                      GTK_SIGNAL_FUNC (on_changed),
                      NULL);
  gtk_widget_ref (entry2);
  gtk_object_set_data_full (GTK_OBJECT (window1), "entry2", entry2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry2);
  gtk_table_attach (GTK_TABLE (table1), entry2, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  entry3 = gtk_panda_entry_new ();
  gtk_panda_entry_set_input_mode (GTK_PANDA_ENTRY (entry3),
				  GTK_PANDA_ENTRY_XIM_MODE);
  gtk_panda_entry_set_xim_enabled (GTK_PANDA_ENTRY (entry3), TRUE);
  gtk_widget_ref (entry3);
  gtk_object_set_data_full (GTK_OBJECT (window1), "entry3", entry3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry3);
  gtk_table_attach (GTK_TABLE (table1), entry3, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  return window1;
}

int
main (int argc, char **argv)
{
  GtkWidget *window;

#ifdef ENABLE_GNOME
  gnome_init ("test", VERSION, argc, argv);
#else
  gtk_set_locale ();
  gtk_init (&argc, &argv);
#endif

  window = create_window1 ();
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC (gtk_main_quit), NULL);

  gtk_widget_show (window);
  gtk_main ();

  return 0;
}
