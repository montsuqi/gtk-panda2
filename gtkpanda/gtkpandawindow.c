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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib-object.h>
#include <gtk/gtk.h>

#include "config.h"
#include "gtkpandaintl.h"
#include "gtkpandawindow.h"

enum {
  PROP_0,
  PROP_X,
  PROP_Y,
  PROP_WMCLASS_CLASS,
  PROP_WMCLASS_NAME
};

static void gtk_panda_window_class_init    (GtkPandaWindowClass *klass);
static void gtk_panda_window_init          (GtkPandaWindow     *window);
static void  gtk_panda_window_set_property       (GObject         *object,
                       guint            prop_id,
                       const GValue    *value,
                       GParamSpec      *pspec);
static void  gtk_panda_window_get_property       (GObject         *object,
                       guint            prop_id,
                       GValue          *value,
                       GParamSpec      *pspec);


GType
gtk_panda_window_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      static const GTypeInfo info =
      {
        sizeof (GtkPandaWindowClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_window_class_init,
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaWindow),
        0,
        (GInstanceInitFunc) gtk_panda_window_init
      };

      type = g_type_register_static( GTK_TYPE_WINDOW,
                                     "GtkPandaWindow",
                                     &info,
                                     0);
    }

  return type;
}

static void
gtk_panda_window_class_init (GtkPandaWindowClass *klass)
{
  GObjectClass *gobject_class;


  gobject_class = (GObjectClass*) klass;
  gobject_class->set_property = gtk_panda_window_set_property; 
  gobject_class->get_property = gtk_panda_window_get_property; 


  g_object_class_install_property (gobject_class,
    PROP_X,
    g_param_spec_int ("x", "X", "X",
                          0, 5000, 0,
                          G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
    PROP_Y,
    g_param_spec_int ("y", "Y", "Y",
                          0, 5000, 0,
                          G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
    PROP_WMCLASS_CLASS,
    g_param_spec_string ("wmclass_class",
    "WMCLASS_CLASS",
    "WMCLASS_CLASS",
    "",
    G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
    PROP_WMCLASS_NAME,
    g_param_spec_string ("wmclass_name",
    "WMCLASS_NAME",
    "WMCLASS_NAME",
    "",
    G_PARAM_READWRITE));
}

static void
gtk_panda_window_init (GtkPandaWindow *window)
{
  window->x = 0;
  window->y = 0;
  window->wmclass_class = NULL;
  window->wmclass_name = NULL;
}

GtkWidget*
gtk_panda_window_new (GtkWindowType type)
{
  GtkWidget *window;

  window = g_object_new (GTK_PANDA_TYPE_WINDOW, NULL);
  return window;
}

static void 
gtk_panda_window_set_property (GObject         *object,
		      guint            prop_id,
		      const GValue    *value,
		      GParamSpec      *pspec)
{
  GtkPandaWindow *window;

  g_return_if_fail(GTK_IS_PANDA_WINDOW(object));
  window = GTK_PANDA_WINDOW (object);

  switch (prop_id)
    {
    case PROP_X:
      window->x = g_value_get_int(value);
      break;
    case PROP_Y:
      window->y = g_value_get_int(value);
      break;
    case PROP_WMCLASS_CLASS:
      if (window->wmclass_class != NULL) {
        g_free(window->wmclass_class);
      }
      window->wmclass_class = g_strdup(g_value_get_string(value));
      break;
    case PROP_WMCLASS_NAME:
      if (window->wmclass_name != NULL) {
        g_free(window->wmclass_name);
      }
      window->wmclass_name = g_strdup(g_value_get_string(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void gtk_panda_window_get_property (GObject         *object,
				  guint            prop_id,
				  GValue          *value,
				  GParamSpec      *pspec)
{
  GtkPandaWindow *window;

  g_return_if_fail(GTK_IS_PANDA_WINDOW(object));
  window = GTK_PANDA_WINDOW (object);

  switch (prop_id)
    {
    case PROP_X:
      g_value_set_int (value, window->x);
      break;
    case PROP_Y:
      g_value_set_int (value, window->y);
      break;
    case PROP_WMCLASS_CLASS:
      g_value_set_string (value, g_strdup(window->wmclass_class));
      break;
    case PROP_WMCLASS_NAME:
      g_value_set_string (value, g_strdup(window->wmclass_name));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}
