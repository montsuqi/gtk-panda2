#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <gtk/gtk.h>
#include "gtkpanda.h"

GtkWidget *combo1;
GtkWidget *combo2;

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
on_combo_entry1_activate               (GtkEditable     *editable,
                                        gpointer         user_data)
{
    gchar *text;

    text = gtk_editable_get_chars(editable, 0, -1);
    printf("on_combo_entry1_activate() : combo1(%p) = %s\n",
       gtk_widget_get_parent(GTK_WIDGET(editable)), text);
}

static void
on_combo_entry1_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
    gchar *text;

    text = gtk_editable_get_chars(editable, 0, -1);
    printf("on_combo_entry1_changed()  : combo1(%p) = %s\n",
       gtk_widget_get_parent(GTK_WIDGET(editable)), text);
}

static void
on_combo_entry2_activate               (GtkEditable     *editable,
                                        gpointer         user_data)
{
    gchar *text;

    text = gtk_editable_get_chars(editable, 0, -1);
    printf("on_combo_entry2_activate() : combo2(%p) = %s\n",
       gtk_widget_get_parent(GTK_WIDGET(editable)), text);
}

static void
on_combo_entry2_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
    gchar *text;

    text = gtk_editable_get_chars(editable, 0, -1);
    printf("on_combo_entry2_changed()  : combo2(%p) = %s\n",
       gtk_widget_get_parent(GTK_WIDGET(editable)), text);
}

static void
cb_dialog_ok_button(GtkButton *button, gpointer user_data)
{
  GtkEntry *entry;
  gchar *text;

  printf("cb_dialog_ok_button():\n");

  entry = gtk_panda_combo_get_entry(GTK_PANDA_COMBO(combo1));
  text = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
  printf(" combo1 = %s\n", text);

  entry = gtk_panda_combo_get_entry(GTK_PANDA_COMBO(combo2));
  text = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
  printf(" combo2 = %s\n", text);
  gtk_main_quit();
}

static void
cb_dialog_test_button(GtkButton *button, gpointer user_data)
{
  GtkEntry *entry;
  gchar *text;

  printf("cb_dialog_test_button():\n");
  entry = gtk_panda_combo_get_entry(GTK_PANDA_COMBO(combo1));
  text = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
  printf(" combo1 = %s\n", text);

  entry = gtk_panda_combo_get_entry(GTK_PANDA_COMBO(combo2));
  text = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
  printf(" combo2 = %s\n", text);
}

int
main (int argc, char *argv[])
{
  GtkWidget *window1;
  GtkEntry *entry;
  gchar *items1[10];
  gchar *items2[10];

  GtkWidget *button;

  gtk_init (&argc, &argv);

  gtk_panda_init(&argc, &argv);

  window1 = gtk_dialog_new ();

  g_signal_connect (G_OBJECT (window1), "destroy",
		      G_CALLBACK(gtk_widget_destroyed),
		      &window1);

  gtk_window_set_title (GTK_WINDOW (window1), "GtkDialog");
  gtk_container_set_border_width (GTK_CONTAINER (window1), 0);
  gtk_window_set_default_size(GTK_WINDOW(window1), 200, 110);

  button = gtk_button_new_with_label ("OK");
  g_signal_connect (G_OBJECT (button), "clicked",
		      G_CALLBACK (cb_dialog_ok_button),
		      NULL);
  gtk_widget_set_can_default(button,TRUE);
  gtk_box_pack_start (
    GTK_BOX(gtk_dialog_get_action_area(GTK_DIALOG(window1))), 
    button, TRUE, TRUE, 0);
  gtk_widget_grab_default (button);
  gtk_widget_show (button);

  button = gtk_button_new_with_label ("Test");
  g_signal_connect (G_OBJECT (button), "clicked",
		      G_CALLBACK (cb_dialog_test_button),
		      NULL);
  gtk_widget_set_can_default(button,TRUE);
  gtk_box_pack_start (
    GTK_BOX(gtk_dialog_get_action_area(GTK_DIALOG(window1))), 
    button, TRUE, TRUE, 0);
  gtk_widget_show (button);

  combo1 = gtk_panda_combo_new ();
  gtk_widget_show (combo1);
  gtk_box_pack_start (
    GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(window1))), 
    combo1, TRUE, TRUE, 0);
  items1[0] = "item1";
  items1[1] = "item2_____________________________________________";
  items1[2] = "item3";
  items1[3] = NULL;
  gtk_panda_combo_set_popdown_strings (GTK_PANDA_COMBO (combo1), items1);
  entry = gtk_panda_combo_get_entry(GTK_PANDA_COMBO(combo1));
  g_signal_connect(entry, "activate",
    G_CALLBACK(on_combo_entry1_activate), NULL);
  g_signal_connect(entry, "changed",
    G_CALLBACK(on_combo_entry1_changed), NULL);

  combo2 = gtk_panda_combo_new ();
  gtk_widget_show (combo2);
  gtk_box_pack_start (
    GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(window1))), 
    combo2, TRUE, TRUE, 0);
  items2[0] = "item21";
  items2[1] = "item22";
  items2[2] = "item23";
  items2[3] = "item24";
  items2[4] = "item25";
  items2[5] = NULL;
  gtk_panda_combo_set_popdown_strings (GTK_PANDA_COMBO (combo2), items2);
  entry = gtk_panda_combo_get_entry(GTK_PANDA_COMBO(combo2));
  g_signal_connect(entry, "activate",
    G_CALLBACK(on_combo_entry2_activate), NULL);
  g_signal_connect(entry, "changed",
    G_CALLBACK(on_combo_entry2_changed), NULL);

  gtk_widget_show (window1);

  gtk_main ();
  return 0;
}
