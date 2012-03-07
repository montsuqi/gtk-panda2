#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <gtk/gtk.h>
#include "gtkpanda.h"

GtkWidget *text1;
GtkWidget *text2;

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

static void
cb_dialog_ok_button(GtkButton *button, gpointer user_data)
{
  GtkTextBuffer *buffer;
  GtkTextIter start,end;  

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text1));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  fprintf(stderr,"\n==== text1\n");
  fprintf(stderr,"%s", gtk_text_buffer_get_text(buffer, &start, &end, FALSE));

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text2));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  fprintf(stderr,"\n==== text2\n");
  fprintf(stderr,"%s", gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
}

static void
cb_text1_activate(GtkPandaText *text, gpointer data)
{
  GtkTextBuffer *buffer;
  GtkTextIter start,end;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  fprintf(stderr,"\n==== text1 activate\n");
  fprintf(stderr,"%s", gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
}

static void
cb_text1_changed(GtkPandaText *text, gpointer data)
{
  GtkTextBuffer *buffer;
  GtkTextIter start,end;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  fprintf(stderr,"\n==== text1 changed\n");
  fprintf(stderr,"%s", gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
}

static void
cb_text2_activate(GtkPandaText *text, gpointer data)
{
  GtkTextBuffer *buffer;
  GtkTextIter start,end;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  fprintf(stderr,"\n==== text2 activate\n");
  fprintf(stderr,"%s", gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
}

static void
cb_text2_changed(GtkPandaText *text, gpointer data)
{
  GtkTextBuffer *buffer;
  GtkTextIter start,end;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  fprintf(stderr,"\n==== text2 changed\n");
  fprintf(stderr,"%s", gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
}


int
main (int argc, char *argv[])
{
  GtkWidget *window1;
  GtkWidget *button;
  GtkWidget *scrolledwindow1;
  GtkWidget *scrolledwindow2;

  gtk_init (&argc, &argv);
  gtk_panda_init(&argc, &argv);

  window1 = gtk_dialog_new ();

  g_signal_connect (G_OBJECT (window1), "destroy",
		      G_CALLBACK(gtk_widget_destroyed),
		      &window1);

  gtk_window_set_title (GTK_WINDOW (window1), "testtext");
  gtk_container_set_border_width (GTK_CONTAINER (window1), 0);
  gtk_window_set_default_size (GTK_WINDOW(window1), 200, 400);

  button = gtk_button_new_with_label ("OK");
  g_signal_connect (G_OBJECT (button), "clicked",
		      G_CALLBACK (cb_dialog_ok_button),
		      NULL);
  gtk_box_pack_start (
    GTK_BOX(gtk_dialog_get_action_area(GTK_DIALOG (window1))), 
		      button, TRUE, TRUE, 0);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_box_pack_start (
    GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG (window1))), 
		      scrolledwindow1, TRUE, TRUE, 0);
  text1 = gtk_panda_text_new();
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), text1);
  g_signal_connect(G_OBJECT(text1), "activate",
    G_CALLBACK(cb_text1_activate), NULL);
  g_signal_connect(G_OBJECT(text1), "changed",
    G_CALLBACK(cb_text1_changed), NULL);

  scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
  gtk_box_pack_start (
    GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG (window1))), 
		      scrolledwindow2, TRUE, TRUE, 0);
  text2 = gtk_panda_text_new();
  gtk_panda_text_set_xim_enabled(GTK_PANDA_TEXT(text2), TRUE);
  gtk_container_add (GTK_CONTAINER (scrolledwindow2), text2);
  g_signal_connect(G_OBJECT(text2), "activate",
    G_CALLBACK(cb_text2_activate), NULL);
  g_signal_connect(G_OBJECT(text2), "changed",
    G_CALLBACK(cb_text2_changed), NULL);

  gtk_widget_show_all (window1);

  gtk_main ();
  return 0;
}
