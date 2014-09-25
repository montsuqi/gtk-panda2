/* GtkPandaText - text widget for gtk+
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
 * Based on gtk/gtktextview.c
 */

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#include <string.h>

#include <gtk/gtk.h>
#include <glib-object.h>
#include <gdk/gdkkeysyms.h>

#include "config.h"
#include "gtkpandaintl.h"
#include "gtkpandatext.h"
#include "imcontrol.h"

enum
{   
  ACTIVATE,
  CHANGED,   
  LAST_SIGNAL
};

enum {
  PROP_0,
  PROP_TEXT,
  PROP_ENABLE_XIM
};

static GtkWidgetClass *parent_class = NULL;
static guint signals [LAST_SIGNAL] = { 0 };

static void gtk_panda_text_class_init (GtkPandaTextClass *klass);
static void gtk_panda_text_init (GtkPandaText *text);
static gint gtk_panda_text_focus_in (GtkWidget *widget, GdkEventFocus *event);
static void gtk_panda_text_activate (GtkPandaText *text);
static void gtk_panda_text_changed (GtkTextBuffer *textview, gpointer data);
static void gtk_panda_text_set_property (
  GObject         *object,
  guint           prop_id,
  const GValue    *value,
  GParamSpec      *pspec);
static void gtk_panda_text_get_property (
  GObject         *object,
  guint           prop_id,
  GValue          *value,
  GParamSpec      *psec);

static void
gtk_panda_text_class_init ( GtkPandaTextClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class;
  GtkBindingSet *binding_set;

  widget_class = (GtkWidgetClass *) klass;
  widget_class->focus_in_event = gtk_panda_text_focus_in;
  parent_class = g_type_class_ref(GTK_TYPE_TEXT_VIEW);

  klass->activate = gtk_panda_text_activate;

  gobject_class->set_property = gtk_panda_text_set_property;
  gobject_class->get_property = gtk_panda_text_get_property;

  g_object_class_install_property (gobject_class,
    PROP_TEXT,
    g_param_spec_string ("text",
    _("Text"),
    _("Text"),
    "",
    G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
    PROP_ENABLE_XIM,
    g_param_spec_boolean ("xim_enabled",
                          _("Enable Input Method"),
                          _("Whether enable input method controll"),
                          FALSE,
                          G_PARAM_READWRITE));

  signals[ACTIVATE] =
  g_signal_new ("activate",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaTextClass, activate),
        NULL, NULL,
        gtk_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[CHANGED] =
  g_signal_new ("changed",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST,
        G_STRUCT_OFFSET (GtkPandaTextClass, changed),
        NULL, NULL,
        gtk_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  binding_set = gtk_binding_set_by_class (klass);
  gtk_binding_entry_add_signal (binding_set, GDK_Return, GDK_CONTROL_MASK,
    "activate", 0);
}

static void
gtk_panda_text_init (GtkPandaText * text)
{
  GtkTextBuffer *buffer;

  text->xim_enabled = FALSE;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));

  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text), GTK_WRAP_CHAR);

  g_signal_connect(buffer, "changed",
    G_CALLBACK(gtk_panda_text_changed), text);
}

GType
gtk_panda_text_get_type (void)
{
  static GType text_type = 0;

  if (!text_type)
    {
      static const GTypeInfo text_info =
      {
        sizeof (GtkPandaTextClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_text_class_init,           
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaText),
        0,
        (GInstanceInitFunc) gtk_panda_text_init
      };

      text_type = g_type_register_static( GTK_TYPE_TEXT_VIEW,
                                           "GtkPandaText",
                                           &text_info,
                                           0);
    }
  return text_type;
}

GtkWidget *
gtk_panda_text_new ()
{
  return g_object_new ( GTK_PANDA_TYPE_TEXT, NULL);
}

static void
gtk_panda_text_activate(GtkPandaText *text)
{
  // do nothing;
}

static void 
gtk_panda_text_changed (GtkTextBuffer  *buffer,
  gpointer data)
{
  GtkPandaText *text;

  text = GTK_PANDA_TEXT(data);
  g_signal_emit_by_name(text, "changed");
}

static gint
gtk_panda_text_focus_in (GtkWidget     *widget,
             GdkEventFocus *event)
{
  GtkPandaText *ptext;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_TEXT_VIEW (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  ptext = GTK_PANDA_TEXT(widget);

  if (GTK_WIDGET_CLASS(parent_class)->focus_in_event) {
    (*GTK_WIDGET_CLASS(parent_class)->focus_in_event)(widget,event);
  }

  if (ptext->xim_enabled) {
    set_im(widget);
  } else {
    unset_im(widget);
  }

  return FALSE;
}

static void
gtk_panda_text_set_property (
  GObject      *object,
  guint        prop_id,
  const GValue *value,
  GParamSpec   *pspec)
{
  GtkPandaText  *text;
  GtkTextBuffer *buf;
  gchar *str;

  g_return_if_fail(GTK_IS_PANDA_TEXT(object));
  text = GTK_PANDA_TEXT (object);
  
  switch  (prop_id)  {
    case PROP_TEXT:
      buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
      str = (gchar*)g_value_get_string(value);
      gtk_text_buffer_set_text(buf, str, strlen(str));
      break;
    case PROP_ENABLE_XIM:
      text->xim_enabled = g_value_get_boolean(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gtk_panda_text_get_property (
  GObject      *object,
  guint        prop_id,
  GValue       *value,
  GParamSpec    *pspec)
{
  GtkPandaText  *text;
  GtkTextBuffer *buf;
  GtkTextIter start;
  GtkTextIter end;
  gchar *str;

  g_return_if_fail(GTK_IS_PANDA_TEXT(object));
  text = GTK_PANDA_TEXT(object);
  
  switch (prop_id) {
    case PROP_TEXT:
      buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
	  gtk_text_buffer_get_start_iter(buf, &start);
	  gtk_text_buffer_get_end_iter(buf, &end);
      str = gtk_text_buffer_get_text(buf,&start,&end,FALSE);
      g_value_set_string(value, g_strdup(str));
      break;
    case PROP_ENABLE_XIM:
      g_value_set_boolean(value, text->xim_enabled);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

// public API

void 
gtk_panda_text_set_xim_enabled (
  GtkPandaText *text, 
  gboolean flag)
{
  text->xim_enabled = flag;
}
