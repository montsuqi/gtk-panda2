/* GtkNumberEntry - widget for gtk+
 * Copyright (C) 2001 Masami Ogoshi <ogochan@linux.or.jp>
 *
 * GtkNumberEntry widget by Masami Ogoshi
 * Based on GtkItemEntry widget
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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
#define  DEBUG
#define  TRACE
*/
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <ctype.h>
#include <string.h>
#include <math.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdki18n.h>

#include "config.h"
#include "gtkpandaintl.h"
#include "numeric.h"
#include "imcontrol.h"
#include "gtknumberentry.h"
#include "debug.h"

enum {
  PROP_0,
  PROP_FORMAT
};

static void gtk_number_entry_class_init (GtkNumberEntryClass     *klass);
static void gtk_number_entry_init       (GtkNumberEntry          *entry);
static void gtk_number_entry_set_property (
  GObject         *object,
  guint           prop_id,
  const GValue    *value,
  GParamSpec      *pspec);
static void gtk_number_entry_get_property (
  GObject         *object,
  guint           prop_id,
  GValue          *value,
  GParamSpec      *psec);

static gint gtk_number_entry_key_press (
  GtkWidget         *widget,
  GdkEventKey       *event);

static gint gtk_number_entry_focus_in(GtkWidget *widget,GdkEventFocus *event);
static GtkWidgetClass *parent_class = NULL;

GType
gtk_number_entry_get_type (void)
{
  static GType entry_type = 0;

  if (!entry_type)
    {
      static const GTypeInfo entry_info =
      {
        sizeof (GtkNumberEntryClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_number_entry_class_init,
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkNumberEntry),
        0,
        (GInstanceInitFunc) gtk_number_entry_init
      };

      entry_type = g_type_register_static( GTK_TYPE_ENTRY,
                                           "GtkNumberEntry",
                                           &entry_info,
                                           0);
    }

  return entry_type;
}

static void
gtk_number_entry_class_init (GtkNumberEntryClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class;

dbgmsg(">gtk_number_entry_class_init");
  widget_class = (GtkWidgetClass*) klass;

  widget_class->key_press_event = gtk_number_entry_key_press;
  widget_class->focus_in_event = gtk_number_entry_focus_in;

  parent_class = g_type_class_ref (GTK_TYPE_ENTRY);

  gobject_class->set_property = gtk_number_entry_set_property;
  gobject_class->get_property = gtk_number_entry_get_property;

  g_object_class_install_property (gobject_class,
    PROP_FORMAT,
    g_param_spec_string ("format",
    _("Format"),
    _("The format of the entry"),
    "",
    G_PARAM_READWRITE));
dbgmsg("<gtk_number_entry_class_init");
}

static void
gtk_number_entry_init (GtkNumberEntry *entry)
{
  entry->format = NULL;
  entry->scale = 0;
  entry->value = NumericInput("0.0", 9, 0);
  gtk_entry_set_alignment(GTK_ENTRY(entry), 1);
  gtk_entry_set_completion(GTK_ENTRY(entry), NULL);
}

extern  GtkWidget*
gtk_number_entry_new ()
{
  GtkWidget *number_entry;

  number_entry = g_object_new (GTK_TYPE_NUMBER_ENTRY, NULL);
  return number_entry;
}

static gint
gtk_number_entry_focus_in(
  GtkWidget     *widget,
  GdkEventFocus *event)
{
  GtkEntry *entry;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_ENTRY (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  entry = GTK_ENTRY (widget);

  if (GTK_WIDGET_CLASS (parent_class)->focus_in_event) {
    (* GTK_WIDGET_CLASS (parent_class)->focus_in_event)
      (widget, event);
  }
  unset_im(widget);
  return FALSE;
}

static void
gtk_number_entry_set_property (
  GObject      *object,
  guint        prop_id,
  const GValue *value,
  GParamSpec   *pspec)
{
  GtkNumberEntry  *entry;
  entry = GTK_NUMBER_ENTRY (object);
  
  switch  (prop_id)  {
    case PROP_FORMAT:
      gtk_number_entry_set_format(entry, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gtk_number_entry_get_property (
  GObject      *object,
  guint        prop_id,
  GValue       *value,
  GParamSpec    *pspec)
{
  GtkNumberEntry  *entry;
  entry = GTK_NUMBER_ENTRY (object);
  
  switch (prop_id) {
    case PROP_FORMAT:
      g_value_set_string(value, g_strdup(entry->format));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

extern  GtkWidget*
gtk_number_entry_new_with_max_length (
  guint16 max)
{
  GtkWidget *number_entry;

  number_entry = g_object_new (GTK_TYPE_NUMBER_ENTRY, NULL);
  gtk_entry_set_max_length(GTK_ENTRY(number_entry), max);

  return GTK_WIDGET (number_entry);
}

extern  GtkWidget*
gtk_number_entry_new_with_format(
  char  *fmt)
{
  GtkWidget *number_entry;

  number_entry = g_object_new (GTK_TYPE_NUMBER_ENTRY, NULL);
  gtk_number_entry_set_format(GTK_NUMBER_ENTRY(number_entry), fmt);

  return GTK_WIDGET (number_entry);
}

extern  void
gtk_number_entry_set_format(
  GtkNumberEntry  *entry,
  const gchar    *fmt)
{
  GtkEditable *editable;
  gint pos;
  char buf[64] = "";

  g_return_if_fail (entry != NULL);
  g_return_if_fail (GTK_IS_NUMBER_ENTRY (entry));

  if    (  entry->format  !=  NULL  ) {
    g_free(entry->format);
  }
  entry->format = g_strdup(fmt);

  editable = GTK_EDITABLE(entry);
  pos = 0;
  NumericFormat(buf,entry->format, entry->value);
  gtk_editable_delete_text(editable, 0, -1);
  gtk_editable_insert_text(editable, buf, strlen(buf), &pos);
  gtk_editable_set_position(editable, -1);
}

extern  void
gtk_number_entry_set_value (
  GtkNumberEntry *entry,
  Numeric value)
{
  GtkEditable *editable;
  gint pos;
  char buf[64] = "";
  int pr;
  int sc;

dbgmsg(">gtk_number_entry_set_value");
  g_return_if_fail (entry != NULL);
  g_return_if_fail (GTK_IS_NUMBER_ENTRY (entry));

  if    (  entry->value  !=  value  ) {
    NumericFormatToPrecision(entry->format,&pr,&sc);
    NumericFree(entry->value);
    entry->value = NumericRescale(value,pr+1,sc);
    entry->scale = 0;
    entry->expo = 0;

    editable = GTK_EDITABLE(entry);
    pos = 0;
    NumericFormat(buf,entry->format, entry->value);
    gtk_editable_delete_text(editable, 0, -1);
    gtk_editable_insert_text(editable, buf, strlen(buf), &pos); 
    gtk_editable_set_position(editable, -1);
  } else {
    dbgmsg("not set value");
  }
dbgmsg("<gtk_number_entry_set_value");
}

extern Numeric
gtk_number_entry_get_value (
  GtkNumberEntry  *entry)
{
  int pr;
  int sc;
  
dbgmsg(">gtk_number_entry_get_value");
  g_return_val_if_fail (entry != NULL, 0);
  g_return_val_if_fail (GTK_IS_NUMBER_ENTRY (entry), 0);

  NumericFormatToPrecision(entry->format,&pr,&sc);
  
dbgmsg("<gtk_number_entry_get_value");
  return  (NumericRescale(entry->value,pr,sc));
}

static gint
gtk_number_entry_key_press (
  GtkWidget  *widget,
  GdkEventKey  *event)
{
  GtkNumberEntry *entry;
  GtkEditable *editable;

  gint return_val;
  gint pos;
  char *p;
  char buf[64] = "";
  int i;
  int minus;
  int pr;
  int sc;
  Numeric value;
  Numeric value2;
  Numeric input;
  Numeric small;
  Numeric one;

dbgmsg(">gtk_entry_key_press");
  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_NUMBER_ENTRY (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  entry = GTK_NUMBER_ENTRY (widget);
  g_return_val_if_fail (entry->value != NULL, FALSE);
  editable = GTK_EDITABLE (widget);
  return_val = FALSE;

  if (!gtk_widget_has_focus(widget)) return FALSE;

  if (!gtk_editable_get_editable(editable)) {
    switch(event->keyval){
      case GDK_Tab:
        return FALSE;
        break;
      case GDK_Left:
      case GDK_Right:
      case GDK_Up:
      case GDK_Down:
      case GDK_Return:
      break;
      default:
      return TRUE;     
    }
  }

  NumericFormatToPrecision(entry->format,&pr,&sc);
  switch(event->keyval)  {
    case GDK_BackSpace:
    case GDK_Clear:
    case GDK_Home:
    case GDK_End:
      return_val = TRUE;
      entry->scale = 0;
      entry->expo = 0;
      NumericFree(entry->value);
      entry->value = NumericInput("0",pr+1,sc);
      break;
    case GDK_Insert:
      return_val = TRUE;
      if (event->state & GDK_SHIFT_MASK) {
        gtk_editable_paste_clipboard (editable);
      } else
      if (event->state & GDK_CONTROL_MASK)  {
        gtk_editable_copy_clipboard (editable);
      }
      break;
    case GDK_Delete:
      return_val = TRUE;
      if (event->state & GDK_SHIFT_MASK)  {
        gtk_editable_cut_clipboard (editable);
      }
      break;
    case GDK_Tab:
      return FALSE;
      break;
    case GDK_Left:
    case GDK_Right:
    case GDK_Shift_L:
    case GDK_Shift_R:
    case GDK_Escape:
    case GDK_Up:
    case GDK_Down:
      return_val = TRUE;
      break;
    case GDK_Return:
    case GDK_KP_Enter:
      return_val = TRUE;
      g_signal_emit_by_name (G_OBJECT (entry), "activate");
      break;
    default:
      if    (  event->length  >  0  ) {
        gtk_editable_delete_selection (editable);
        if    (  NumericNegative(entry->value)  ) {
          minus = -1;
          value = NumericUMinus(entry->value);
          NumericFree(entry->value);
          entry->value = value;
        } else {
          minus = 1;
        }
        for  ( p = event->string , i = 0 ; i < event->length ; i ++, p ++ ) {
          if    (  *p  ==  '-'  ) {
            minus = ( minus < 0 ) ? 1 : -1;
          } else
          if    (  *p  ==  '.'  ) {
            if    (  strchr(entry->format,'.')  !=  NULL  ) {
              entry->scale = 1;
            }
          } else
          if    (  (  *p  >=  '0'  )
              &&  (  *p  <=  '9'  ) ) {
            input = NumericInputChar(*p,1,0);
            if    (  entry->scale  ==  0  ) {
              if    (  entry->expo  <  ( pr - sc )  )  {
                value = NumericShift(entry->value,1);
                NumericFree(entry->value);
                entry->value = NumericADD(value,input);
                NumericFree(value);
                entry->expo ++;
              }
            } else {
              if    (  entry->scale  <=  sc  ) {
                one = NumericInput("1",pr,sc);
                small = NumericShift(one,- entry->scale);
                entry->scale ++;
                NumericFree(one);
                value = NumericMUL(small,input);
                NumericFree(small);
                value2 = NumericADD(entry->value,value);
                NumericFree(entry->value);
                NumericFree(value);
                entry->value = value2;
              }
            }
            NumericFree(input);
          }
        }
        if    (  minus  <  0  ) {
          value = NumericUMinus(entry->value);
          NumericFree(entry->value);
          entry->value = value;
        }
        return_val = TRUE;
      }
      break;
  }
  
  pos = 0;
  NumericFormat(buf,entry->format, entry->value);
  gtk_editable_delete_text(editable, 0, -1);
  gtk_editable_insert_text(editable, buf, strlen(buf), &pos); 
  gtk_editable_set_position(editable, -1);

dbgmsg("<gtk_entry_key_press");
  return return_val;
}
