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
  GtkWidgetClass *widget_class,*base_widget_class;
  GObjectClass *gobject_class;

  base_widget_class = g_type_class_ref(GTK_TYPE_WIDGET);
  widget_class = (GtkWidgetClass*) klass;

  widget_class->get_preferred_width = base_widget_class->get_preferred_width;
  widget_class->get_preferred_height = base_widget_class->get_preferred_height;
#if 0
  widget_class->get_preferred_height_for_width = 
    base_widget_class->get_preferred_height_for_width;
  widget_class->get_preferred_width_for_height = 
    base_widget_class->get_preferred_width_for_height;
#endif

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
gtk_panda_combo_init (GtkPandaCombo * combo)
{
  combo->case_sensitive = FALSE;
  combo->use_arrows = TRUE;
  combo->loop_selection = FALSE;
  gtk_widget_set_margin_top(GTK_WIDGET(combo),0);
  gtk_widget_set_margin_left(GTK_WIDGET(combo),0);
  gtk_widget_set_margin_right(GTK_WIDGET(combo),0);
  gtk_widget_set_margin_bottom(GTK_WIDGET(combo),0);
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

      combo_type = g_type_register_static( GTK_TYPE_COMBO_BOX_TEXT,
                                           "GtkPandaCombo",
                                           &combo_info,
                                           0);
    }
  return combo_type;
}

gboolean
cb_entry_focus_out(GtkWidget *widget,
  GdkEvent *event,
  gpointer data)
{
  gtk_editable_set_position(GTK_EDITABLE(widget), 0);
  return FALSE;
}

GtkWidget *
gtk_panda_combo_new (void)
{
  GtkWidget *ret;
  GtkEntry *entry;
  GtkEntryCompletion *comp;
  GtkListStore *store;

  store = gtk_list_store_new(1, G_TYPE_STRING);
 
  ret = g_object_new( gtk_panda_combo_get_type(),
    "model", GTK_TREE_MODEL(store),
    "has-entry",TRUE,
    "entry-text-column",0,
    NULL);

  comp = gtk_entry_completion_new();
  gtk_entry_completion_set_model(comp,GTK_TREE_MODEL(store));
  gtk_entry_completion_set_text_column(comp,0);
  entry = gtk_panda_combo_get_entry(GTK_PANDA_COMBO(ret));
  gtk_entry_set_completion(entry,comp);

  g_signal_connect(G_OBJECT(entry),"focus-out-event",
    G_CALLBACK(cb_entry_focus_out),NULL);

  g_object_unref (store);
  gtk_combo_box_set_row_span_column(GTK_COMBO_BOX(ret),0);
  gtk_combo_box_set_wrap_width(GTK_COMBO_BOX(ret),0);

  return ret;
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

GtkEntry *
gtk_panda_combo_get_entry(GtkPandaCombo *combo)
{
  GtkEntry *entry;

  g_return_val_if_fail (combo != NULL,NULL);
  g_return_val_if_fail (GTK_IS_PANDA_COMBO (combo),NULL);

  entry = GTK_ENTRY(gtk_bin_get_child(GTK_BIN(combo)));

  return entry;
}
