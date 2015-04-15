/* gtkpandacombo - combo widget for gtk+
 * Copyright 1997 Paolo Molaro
 * Copyright 2001 AXE, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Based on gtk/gtkcombo.c
 */

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib-object.h>

#include "config.h"
#include "gtkpandaintl.h"
#include "gtkpandacombo.h"

enum {
	PROP_0,
	PROP_CASE_SENSITIVE,
	PROP_USE_ARROWS,
	PROP_LOOP_SELECTION
};

static void gtk_panda_combo_class_init(GtkPandaComboClass *klass);
static void gtk_panda_combo_init(GtkPandaCombo *combo);

static void gtk_panda_combo_contents_changed (GtkEntry *entry,
  gpointer user_data);
static void gtk_panda_combo_active_changed (GtkComboBox *combo_box,
  gpointer user_data);
static gboolean gtk_panda_combo_mnemonic_activate(
  GtkWidget *entry,
  gboolean   group_cycling);
static void gtk_panda_combo_grab_focus(GtkWidget *widget);
static void has_frame_changed(GtkPandaCombo *combo,
  GParamSpec *pspec,
  gpointer data);
static void gtk_panda_combo_entry_insert_text(GtkEntry *entry, 
  GdkEventKey *event, GtkPandaCombo *combo);
static gboolean gtk_panda_combo_entry_key_press(GtkEntry * entry,
  GdkEventKey * event, GtkPandaCombo * combo);
static void gtk_panda_combo_completion(GtkPandaCombo *combo);
static void gtk_panda_combo_select_previous(GtkPandaCombo *combo);
static void gtk_panda_combo_select_next(GtkPandaCombo *combo);

static void  gtk_panda_combo_set_property       (GObject         *object,
                       guint            prop_id,
                       const GValue    *value,
                       GParamSpec      *pspec);
static void  gtk_panda_combo_get_property       (GObject         *object,
                       guint            prop_id,
                       GValue          *value,
                       GParamSpec      *pspec);

static void
gtk_panda_combo_class_init (GtkPandaComboClass * klass)
{
  GObjectClass *gobject_class;
  GtkWidgetClass *widget_class;

  widget_class = (GtkWidgetClass *) klass;
  widget_class->mnemonic_activate = gtk_panda_combo_mnemonic_activate;
  widget_class->grab_focus = gtk_panda_combo_grab_focus;

  gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->set_property = gtk_panda_combo_set_property; 
  gobject_class->get_property = gtk_panda_combo_get_property; 

  g_object_class_install_property (gobject_class,
    PROP_CASE_SENSITIVE,
    g_param_spec_boolean ("case-sensitive",
                          _("Case Sensitive"),
                          _("Case Sensitive"),
                          TRUE,
                          G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
    PROP_USE_ARROWS,
    g_param_spec_boolean ("use-arrows",
                          _("Use Arrows"),
                          _("Use Arrows"),
                          TRUE,
                          G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
    PROP_LOOP_SELECTION,
    g_param_spec_boolean ("loop-selection",
                          _("Loop Selection"),
                          _("Loop Selection"),
                          TRUE,
                          G_PARAM_READWRITE));
}

static void
set_focusable(GtkWidget *widget, gpointer data)
{
  if (GTK_IS_BUTTON(widget)) {
    gtk_widget_set_can_focus(widget,FALSE);
  }
}

static void
gtk_panda_combo_init (GtkPandaCombo * combo)
{
  GtkCellRenderer *renderer;
  combo->entry = gtk_panda_entry_new();
#if 0
  GTK_ENTRY (combo->entry)->is_cell_renderer = TRUE;
#endif
  gtk_container_add(GTK_CONTAINER(combo), combo->entry);
  gtk_widget_show (combo->entry);

  renderer = gtk_cell_renderer_text_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combo), renderer, TRUE);
  gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combo),
                                  renderer,
                                  "text", 0,
                                  NULL);
  gtk_combo_box_set_active (GTK_COMBO_BOX (combo), -1);
  gtk_combo_box_set_button_sensitivity (GTK_COMBO_BOX (combo),
    GTK_SENSITIVITY_ON);

  // for set focusable false to popup button
  gtk_container_forall(GTK_CONTAINER(combo), set_focusable, NULL);
 
  combo->case_sensitive = FALSE;
  combo->use_arrows = TRUE;
  combo->loop_selection = FALSE;

  g_signal_connect(combo->entry, "changed",
    G_CALLBACK(gtk_panda_combo_contents_changed), combo);
  g_signal_connect(combo, "changed",
    G_CALLBACK(gtk_panda_combo_active_changed), combo);
  has_frame_changed(combo, NULL, NULL);   
  g_signal_connect(combo, "notify::has-frame", 
    G_CALLBACK (has_frame_changed), NULL);
  g_signal_connect (combo->entry, "insert_text",
    G_CALLBACK(gtk_panda_combo_entry_insert_text), combo);
  g_signal_connect (combo->entry, "key_press_event",
    G_CALLBACK(gtk_panda_combo_entry_key_press), combo);
}

GType
gtk_panda_combo_get_type (void)
{
  static GType combo_type = 0;

  if (!combo_type)
    {
      static const GTypeInfo combo_info =
      {
        sizeof (GtkPandaComboClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_combo_class_init,           
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaCombo),
        0,
        (GInstanceInitFunc) gtk_panda_combo_init
      };

      combo_type = g_type_register_static( GTK_TYPE_COMBO_BOX,
                                           "GtkPandaCombo",
                                           &combo_info,
                                           0);
    }
  return combo_type;
}

GtkWidget *
gtk_panda_combo_new (void)
{
  GtkWidget *ret;
  GtkListStore *store;

  store = gtk_list_store_new(1, G_TYPE_STRING);
 
  ret = g_object_new( gtk_panda_combo_get_type(),
                      "model", GTK_TREE_MODEL(store),
                      NULL);
  g_object_unref (store);

  return ret;
}

static void
gtk_panda_combo_active_changed (GtkComboBox *combo_box,
  gpointer     user_data)
{
  GtkPandaCombo *panda_combo = GTK_PANDA_COMBO (combo_box);
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *str = NULL;

  if (gtk_combo_box_get_active_iter (combo_box, &iter))
    {
      g_signal_handlers_block_by_func (
         panda_combo->entry, gtk_panda_combo_contents_changed, combo_box);

      model = gtk_combo_box_get_model (combo_box);

      gtk_tree_model_get (model, &iter, 0, &str, -1);
      gtk_entry_set_text (GTK_ENTRY (panda_combo->entry), str);
      g_signal_emit_by_name (GTK_ENTRY(panda_combo->entry), "activate");
      g_free (str);

      g_signal_handlers_unblock_by_func (panda_combo->entry, 
        gtk_panda_combo_contents_changed, combo_box);
    }
}

static void
gtk_panda_combo_contents_changed (GtkEntry *entry,
  gpointer user_data)
{
  GtkComboBox *combo_box = GTK_COMBO_BOX (user_data);

  g_signal_handlers_block_by_func (combo_box,
    gtk_panda_combo_active_changed,
    NULL);
  gtk_combo_box_set_active (combo_box, -1);
  g_signal_handlers_unblock_by_func (combo_box,
    gtk_panda_combo_active_changed,
    NULL);
}

static gboolean
gtk_panda_combo_mnemonic_activate (GtkWidget *widget,
  gboolean   group_cycling)
{
  GtkPandaCombo *combo = GTK_PANDA_COMBO(widget);

  gtk_widget_grab_focus(combo->entry);

  return TRUE;
}

static void
gtk_panda_combo_grab_focus (GtkWidget *widget)
{
  GtkPandaCombo *combo = GTK_PANDA_COMBO(widget);

  gtk_widget_grab_focus (combo->entry);
}

static void
has_frame_changed (GtkPandaCombo *combo,
  GParamSpec *pspec,
  gpointer data)
{
  gboolean has_frame;

  g_object_get (combo, "has-frame", &has_frame, NULL);

  gtk_entry_set_has_frame (GTK_ENTRY (combo->entry), has_frame);
}

static gint
complete_combo_entry (gpointer data)
{
  GtkEditable *editable;
  GtkPandaCombo *combo;
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *prefix;
  gchar *label;
  gint prefixlen;
  gint labellen;
  gint pos;
  gint startpos;

  editable = GTK_EDITABLE (data);
  combo = GTK_PANDA_COMBO(gtk_widget_get_parent(GTK_WIDGET(editable)));
  model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo));
  g_return_val_if_fail(
    gtk_tree_model_get_iter(model, &iter, gtk_tree_path_new_first()),
    0); 

  prefix = gtk_editable_get_chars(editable, 0, -1);
  prefixlen = strlen(prefix);
  startpos = gtk_editable_get_position(editable);
  pos = -1;

  do {
    gtk_tree_model_get(model, &iter, 0, &label, -1);
    labellen = strlen(label);
    if (strncmp (prefix, label, prefixlen) == 0) {
      /* insert the rest as a selected text */
      g_signal_handlers_block_by_func (combo->entry, 
        gtk_panda_combo_entry_insert_text, combo);

      gtk_editable_insert_text(editable, 
        label + prefixlen, labellen - prefixlen, &pos);
      if (startpos != 0)
      	gtk_editable_select_region(editable, prefixlen, -1);

      g_signal_handlers_unblock_by_func (combo->entry, 
        gtk_panda_combo_entry_insert_text, combo);

      g_free(label); 
      break;
    }
    g_free(label); 
  } while(gtk_tree_model_iter_next(model, &iter));  
  g_free(prefix);

  if (!gtk_widget_has_focus(combo->entry)
      && (gtk_editable_get_position (GTK_EDITABLE(combo->entry)) != 0)) {
    gtk_editable_set_position (GTK_EDITABLE(combo->entry), 0);
  }

  return 0;
}

static void
gtk_panda_combo_entry_insert_text(GtkEntry *entry, 
  GdkEventKey *event, 
  GtkPandaCombo *combo)
{
  gchar *text;
  gint pos;
 
  text = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
  pos = gtk_editable_get_position(GTK_EDITABLE(entry));
  if (pos == strlen(text))
    gtk_idle_add (complete_combo_entry, entry);
  g_free(text);
}

static gboolean
gtk_panda_combo_entry_key_press(GtkEntry * entry, 
  GdkEventKey * event, 
  GtkPandaCombo * combo) {   

  if ((event->keyval == GDK_Tab) && (event->state & GDK_MOD1_MASK)) {     
    /* completion */
    gtk_panda_combo_completion(combo);
    return TRUE;

  } else if(((event->state & GDK_MOD1_MASK) && 
    ((event->keyval == GDK_Down) || (event->keyval == GDK_KP_Down)))) {
    /* open popdown */
    gtk_combo_box_popup(GTK_COMBO_BOX(combo));
    return TRUE;

  } else if ((event->keyval == GDK_Up)
    || (event->keyval == GDK_KP_Up)
    || ((event->state & GDK_MOD1_MASK) && ((event->keyval == 'p') 
       || (event->keyval == 'P')))) {
    /* select previous */
    if (combo->use_arrows) {
      gtk_panda_combo_select_previous(combo);
    }
    return TRUE;

  } else if ((event->keyval == GDK_Down)
    || (event->keyval == GDK_KP_Down)
    || ((event->state & GDK_MOD1_MASK) && ((event->keyval == 'n') 
       || (event->keyval == 'N')))) {
    /* select next */
    if (combo->use_arrows) {
      gtk_panda_combo_select_next(combo);
     }
    return TRUE;

  }
  return FALSE;
}

static GList *
gtk_panda_combo_get_items(GtkPandaCombo *combo)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GList *list;
  gchar *label;

  list = NULL;
  model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo));
  if (!gtk_tree_model_get_iter(model, &iter, gtk_tree_path_new_first())) {
    return NULL;
  }

  do {
    gtk_tree_model_get(model, &iter, 0, &label, -1);
    list = g_list_append(list, label);
  } while(gtk_tree_model_iter_next(model, &iter));
  return list;
}

static void
gtk_panda_combo_completion(GtkPandaCombo *combo)
{
  GCompletion *cmpl;
  GtkEditable *editable;
  GList *list;
  gchar *prefix;
  gchar *nprefix;
  gint prefixlen;
  gint pos;
  int i;

  g_signal_stop_emission_by_name (combo->entry, "key_press_event");

  editable = GTK_EDITABLE (combo->entry);
  nprefix = NULL;
  list = gtk_panda_combo_get_items(combo);
  g_return_if_fail(list);

  cmpl = g_completion_new (NULL);
  g_completion_add_items (cmpl, list);
  if (combo->case_sensitive) {
    g_completion_set_compare(cmpl, strncmp); 
  } else {
    g_completion_set_compare(cmpl, strncasecmp); 
  }

  prefix = gtk_editable_get_chars (editable, 0, -1); 
  prefixlen = strlen(prefix);
  g_completion_complete_utf8(cmpl, prefix, &nprefix);

  if (nprefix && strlen (nprefix) > strlen (prefix)) {
      gtk_editable_insert_text (editable, 
        nprefix + prefixlen, 
        strlen(nprefix) - prefixlen, &pos);
      gtk_editable_select_region(editable, 0, pos);
  }
  if (nprefix)
    g_free (nprefix);

  for(i = 0; i < g_list_length(list); i++)
    g_free(g_list_nth_data(list, i));
  g_list_free(list);
  g_free(prefix);
  g_completion_free (cmpl);
}

static void
gtk_panda_combo_select_previous(GtkPandaCombo *combo)
{
  GList *list;
  GtkEditable *editable;
  gchar *item;
  gchar *text;
  gint pos;
  int i, p;

  g_signal_stop_emission_by_name (combo->entry, "key_press_event");
  editable = GTK_EDITABLE(combo->entry);
  text = gtk_editable_get_chars (editable, 0, -1); 

  list = gtk_panda_combo_get_items(combo);

  p = 0;
  for(i = 0; i < g_list_length(list); i++) {
    item = g_list_nth_data(list, i);
    if (strlen(item) > 0) {
      if (!strncmp(text, item, strlen(item))) {
		p = i;
        break;
      } 
    } else {
      if (strlen(text) == 0) {
        p = i;
        break;
      }
    }
  }
  if (p == 0) {
    if (combo->loop_selection)
      item = g_list_nth_data(list, g_list_length(list) - 1);
    else
      item = NULL;
  } else {
    item = g_list_nth_data(list, p - 1);
  }
  if (item != NULL) {
    gtk_editable_delete_text(editable, 0, -1);
    pos = 0;
    gtk_editable_insert_text(editable, item, strlen(item), &pos);
  }

  for(i = 0; i < g_list_length(list); i++)
    g_free(g_list_nth_data(list, i));
  g_list_free(list);
  g_free(text);
}

static void
gtk_panda_combo_select_next(GtkPandaCombo *combo)
{
  GList *list;
  GtkEditable *editable;
  gchar *item;
  gchar *text;
  gint pos;
  int i, p;

  g_signal_stop_emission_by_name (combo->entry, "key_press_event");
  editable = GTK_EDITABLE(combo->entry);
  text = gtk_editable_get_chars (editable, 0, -1);

  list = gtk_panda_combo_get_items(combo);

  for(i = 0; i < g_list_length(list); i++) {
    item = g_list_nth_data(list, i);
	if (strlen(item) > 0) {
      if (!strncmp(text, item, strlen(item))) {
        if (i == g_list_length(list) - 1) {
          if (combo->loop_selection)
            item = g_list_nth_data(list, 0);
          else
            break;
        } else {
          item = g_list_nth_data(list, i + 1);
        }
        gtk_editable_delete_text(editable, 0, -1);
        pos = 0;
        gtk_editable_insert_text(editable, item, strlen(item), &pos);
        break;
      }
    }
  }

  p = 0;
  for(i = 0; i < g_list_length(list); i++) {
    item = g_list_nth_data(list, i);
    if (strlen(item) > 0) {
      if (!strncmp(text, item, strlen(item))) {
		p = i;
        break;
      } 
    } else {
      if (strlen(text) == 0) {
        p = i;
        break;
      }
    }
  }
  if (p == g_list_length(list) - 1) {
    if (combo->loop_selection)
      item = g_list_nth_data(list, 0);
    else
      item = NULL;
  } else {
    item = g_list_nth_data(list, p + 1);
  }
  if (item != NULL) {
    gtk_editable_delete_text(editable, 0, -1);
    pos = 0;
    gtk_editable_insert_text(editable, item, strlen(item), &pos);
  }

  for(i = 0; i < g_list_length(list); i++)
    g_free(g_list_nth_data(list, i));
  g_list_free(list);
  g_free(text);
}

static void 
gtk_panda_combo_set_property (GObject         *object,
		      guint            prop_id,
		      const GValue    *value,
		      GParamSpec      *pspec)
{
  GtkPandaCombo *combo;

  g_return_if_fail(GTK_IS_PANDA_COMBO(object));
  combo = GTK_PANDA_COMBO (object);

  switch (prop_id)
    {
    case PROP_CASE_SENSITIVE:
      combo->case_sensitive = g_value_get_boolean(value);
      break;
    case PROP_USE_ARROWS:
      combo->use_arrows = g_value_get_boolean(value);
      break;
    case PROP_LOOP_SELECTION:
      combo->loop_selection = g_value_get_boolean(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void gtk_panda_combo_get_property (GObject         *object,
				  guint            prop_id,
				  GValue          *value,
				  GParamSpec      *pspec)
{
  GtkPandaCombo *combo;

  g_return_if_fail(GTK_IS_PANDA_COMBO(object));
  combo = GTK_PANDA_COMBO (object);

  switch (prop_id)
    {
    case PROP_CASE_SENSITIVE:
      g_value_set_boolean (value, combo->case_sensitive);
      break;
    case PROP_USE_ARROWS:
      g_value_set_boolean (value, combo->use_arrows);
      break;
    case PROP_LOOP_SELECTION:
      g_value_set_boolean (value, combo->loop_selection);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

// public API

void
gtk_panda_combo_set_case_sensitive (GtkPandaCombo * combo, gboolean val)
{
  g_return_if_fail (combo != NULL);
  g_return_if_fail (GTK_IS_PANDA_COMBO (combo));

  combo->case_sensitive = val;
}

void
gtk_panda_combo_set_use_arrows (GtkPandaCombo * combo, gboolean val)
{
  g_return_if_fail (combo != NULL);
  g_return_if_fail (GTK_IS_PANDA_COMBO (combo));

  combo->use_arrows = val;
}

void
gtk_panda_combo_set_loop_selection (GtkPandaCombo * combo, gboolean val)
{
  g_return_if_fail (combo != NULL);
  g_return_if_fail (GTK_IS_PANDA_COMBO (combo));

  combo->loop_selection = val;
  combo->use_arrows = TRUE;
}

void
gtk_panda_combo_set_popdown_strings (GtkPandaCombo * combo, gchar **strs)
{
  GtkListStore *model;
  GtkTreeIter iter;
  int i;
  
  g_return_if_fail (combo != NULL);
  g_return_if_fail (GTK_IS_PANDA_COMBO (combo));
  g_return_if_fail (strs != NULL);
  model = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(combo)));
  gtk_list_store_clear(model);

  for(i=0;strs[i]!=NULL;i++) {
    gtk_list_store_append(model,&iter);
    gtk_list_store_set(model,&iter,0,strs[i],-1);
  }
  gtk_combo_box_set_active (GTK_COMBO_BOX (combo), 0);
}
