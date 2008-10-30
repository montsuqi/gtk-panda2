/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
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
 */

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "gtkpandaintl.h"
#include "gtkpandatimer.h"

static void gtk_panda_timer_class_init    (GtkPandaTimerClass *klass);
static void gtk_panda_timer_init          (GtkPandaTimer      *timer);

static GtkWidgetClass *parent_class = NULL;

enum {
  TIMEOUT,
  LAST_SIGNAL
};
static guint timer_signals[LAST_SIGNAL] = {0,};

GType
gtk_panda_timer_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      static const GTypeInfo info =
      {
        sizeof (GtkPandaTimerClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_timer_class_init,
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaTimer),
        0,
        (GInstanceInitFunc) gtk_panda_timer_init
      };

      type = g_type_register_static( GTK_TYPE_WIDGET,
                                     "GtkPandaTimer",
                                     &info,
                                     0);
    }

  return type;
}

static void
gtk_panda_timer_class_init (GtkPandaTimerClass *class)
{
  GtkObjectClass *object_class;
  GtkWidgetClass *widget_class;

  object_class = (GtkObjectClass*) class;
  widget_class = (GtkWidgetClass*) class;
  parent_class = gtk_type_class (GTK_TYPE_WIDGET);

  timer_signals[TIMEOUT] =
    g_signal_new ("timeout",
                  G_OBJECT_CLASS_TYPE (class),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (GtkPandaTimerClass, timeout),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);
}

static void
gtk_panda_timer_init (GtkPandaTimer *timer)
{
  GTK_WIDGET_SET_FLAGS (timer, GTK_NO_WINDOW);
}

GtkWidget*
gtk_panda_timer_new (void)
{
  GtkWidget *timer;

  timer = gtk_type_new (GTK_PANDA_TYPE_TIMER);
  GTK_PANDA_TIMER(timer)->tag = 0;
  gtk_panda_timer_set(GTK_PANDA_TIMER(timer), 0);

  return timer;
}

void
gtk_panda_timer_set (GtkPandaTimer *timer, guint32 duration)
{
  timer->duration = duration * 1000;
}

gboolean
timeout_handler(gpointer object)
{
  GtkPandaTimer *timer = object;
  GtkWidget *toplevel = gtk_widget_get_toplevel(GTK_WIDGET(timer));
  if (GTK_WIDGET_VISIBLE(toplevel))
    g_signal_emit(timer, timer_signals[TIMEOUT], 0);
  return FALSE;
}

void
gtk_panda_timer_reset (GtkPandaTimer *timer)
{
  if (timer->tag != 0)
    g_source_remove (timer->tag);

  if (timer->duration > 0)
    timer->tag = g_timeout_add (timer->duration, timeout_handler, timer);
  else
    timer->tag = 0; 
}
