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
    printf("on_combo_entry1_activate() : combo1(0x%08x) = %s\n",
       (int)GTK_WIDGET(editable)->parent, text);
}

static void
on_combo_entry1_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
    gchar *text;

    text = gtk_editable_get_chars(editable, 0, -1);
    printf("on_combo_entry1_changed()  : combo1(0x%08x) = %s\n",
       (int)GTK_WIDGET(editable)->parent, text);
}

static void
on_combo_entry2_activate               (GtkEditable     *editable,
                                        gpointer         user_data)
{
    gchar *text;

    text = gtk_editable_get_chars(editable, 0, -1);
    printf("on_combo_entry2_activate() : combo2(0x%08x) = %s\n",
       (int)GTK_WIDGET(editable)->parent, text);
}

static void
on_combo_entry2_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
    gchar *text;

    text = gtk_editable_get_chars(editable, 0, -1);
    printf("on_combo_entry2_changed()  : combo2(0x%08x) = %s\n",
       (int)GTK_WIDGET(editable)->parent, text);
}

static void
cb_dialog_ok_button(GtkButton *button, gpointer user_data)
{
    gchar *text;

    printf("cb_dialog_ok_button():\n");

    text = gtk_editable_get_chars(
      GTK_EDITABLE(GTK_PANDA_COMBO(combo1)->entry), 0, -1);
    printf(" combo1 = %s\n", text);
    text = gtk_editable_get_chars(
      GTK_EDITABLE(GTK_PANDA_COMBO(combo2)->entry), 0, -1);
    printf(" combo2 = %s\n", text);
    gtk_main_quit();
}

static void
cb_dialog_test_button(GtkButton *button, gpointer user_data)
{
    gchar *text;

    printf("cb_dialog_test_button():\n");

    text = gtk_editable_get_chars(
      GTK_EDITABLE(GTK_PANDA_COMBO(combo1)->entry), 0, -1);
    printf(" combo1 = %s\n", text);
    text = gtk_editable_get_chars(
      GTK_EDITABLE(GTK_PANDA_COMBO(combo2)->entry), 0, -1);
    printf(" combo2 = %s\n", text);
}

int
main (int argc, char *argv[])
{
  GtkWidget *window1;
  GList *combo1_items = NULL;
  GList *combo2_items = NULL;

  GtkWidget *button;

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  gtk_panda_init(&argc, &argv);

  window1 = gtk_dialog_new ();

  gtk_signal_connect (GTK_OBJECT (window1), "destroy",
		      GTK_SIGNAL_FUNC(gtk_widget_destroyed),
		      &window1);

  gtk_window_set_title (GTK_WINDOW (window1), "GtkDialog");
  gtk_container_set_border_width (GTK_CONTAINER (window1), 0);
  gtk_widget_set_usize (window1, 200, 110);

  button = gtk_button_new_with_label ("OK");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (cb_dialog_ok_button),
		      NULL);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window1)->action_area), 
		      button, TRUE, TRUE, 0);
  gtk_widget_grab_default (button);
  gtk_widget_show (button);

  button = gtk_button_new_with_label ("Test");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (cb_dialog_test_button),
		      NULL);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window1)->action_area),
			  button, TRUE, TRUE, 0);
  gtk_widget_show (button);

  combo1 = gtk_panda_combo_new ();
  gtk_widget_ref (combo1);
  gtk_widget_show (combo1);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window1)->vbox), 
		      combo1, TRUE, TRUE, 0);
  combo1_items = g_list_append (combo1_items, "hoge1");
  combo1_items = g_list_append (combo1_items, "hoge2");
  combo1_items = g_list_append (combo1_items, "hoge3");
  gtk_panda_combo_set_popdown_strings (GTK_PANDA_COMBO (combo1), combo1_items);
  g_list_free (combo1_items);
  g_signal_connect(GTK_PANDA_COMBO(combo1)->entry, "activate",
    G_CALLBACK(on_combo_entry1_activate), NULL);
  g_signal_connect(GTK_PANDA_COMBO(combo1)->entry, "changed",
    G_CALLBACK(on_combo_entry1_changed), NULL);

  combo2 = gtk_panda_combo_new ();
  gtk_widget_ref (combo2);
  gtk_widget_show (combo2);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window1)->vbox), 
		      combo2, TRUE, TRUE, 0);
  combo2_items = g_list_append (combo2_items, "hoge21");
  combo2_items = g_list_append (combo2_items, "hoge22");
  combo2_items = g_list_append (combo2_items, "hoge23");
  gtk_panda_combo_set_popdown_strings (GTK_PANDA_COMBO (combo2), combo2_items);
  combo2_items = g_list_append (combo2_items, "hoge24");
  combo2_items = g_list_append (combo2_items, "hoge25");
  combo2_items = g_list_append (combo2_items, "hoge26");
  gtk_panda_combo_set_popdown_strings (GTK_PANDA_COMBO (combo2), combo2_items);
  g_list_free (combo2_items);
  g_signal_connect(GTK_PANDA_COMBO(combo2)->entry, "activate",
    G_CALLBACK(on_combo_entry2_activate), NULL);
  g_signal_connect(GTK_PANDA_COMBO(combo2)->entry, "changed",
    G_CALLBACK(on_combo_entry2_changed), NULL);

  gtk_widget_show (window1);

  gtk_main ();
  return 0;
}
