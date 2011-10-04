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
#include "gtkpandaprogressbar.h"

static void gtk_panda_progress_bar_class_init(GtkPandaProgressBarClass *klass);
static void gtk_panda_progress_bar_init(GtkPandaProgressBar *pbar);

static void  gtk_panda_progress_bar_set_property(GObject         *object,
                       guint            prop_id,
                       const GValue    *value,
                       GParamSpec      *pspec);
static void  gtk_panda_progress_bar_get_property(GObject         *object,
                       guint            prop_id,
                       GValue          *value,
                       GParamSpec      *pspec);

enum {
  PROP_0,
  PROP_LOWER,
  PROP_UPPER,
  PROP_VALUE
};

static GtkWidgetClass *parent_class = NULL;

GType
gtk_panda_progress_bar_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      static const GTypeInfo info =
      {
        sizeof (GtkPandaProgressBarClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_progress_bar_class_init,
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaProgressBar),
        0,
        (GInstanceInitFunc) gtk_panda_progress_bar_init
      };

      type = g_type_register_static( GTK_TYPE_PROGRESS_BAR,
                                     "GtkPandaProgressBar",
                                     &info,
                                     0);
    }

  return type;
}

static void
gtk_panda_progress_bar_class_init (GtkPandaProgressBarClass *klass)
{
  GObjectClass *gobject_class;

  parent_class = g_type_class_ref (GTK_TYPE_PROGRESS_BAR);

  gobject_class = (GObjectClass*) klass;
  gobject_class->set_property = gtk_panda_progress_bar_set_property; 
  gobject_class->get_property = gtk_panda_progress_bar_get_property; 

  g_object_class_install_property (gobject_class,
    PROP_LOWER,
    g_param_spec_int ("lower",
      _("progress bar value lower"),
      _("progress bar value lower"),
      0,
      100000,
      0,
      G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
    PROP_UPPER,
    g_param_spec_int ("upper",
      _("progress bar value upper"),
      _("progress bar value upper"),
      0,
      100000,
      100,
      G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
    PROP_VALUE,
    g_param_spec_int ("value",
      _("progress bar value"),
      _("progress bar value"),
      0,
      100000,
      0,
      G_PARAM_READWRITE));
}

static void
gtk_panda_progress_bar_init (GtkPandaProgressBar *pbar)
{
  pbar->lower = 0;
  pbar->upper = 100;
  pbar->value = 0;
  gtk_panda_progress_bar_set_value(pbar,0);
}

GtkWidget*
gtk_panda_progress_bar_new (void)
{
  return g_object_new (GTK_PANDA_TYPE_PROGRESS_BAR, NULL);
}

static void 
gtk_panda_progress_bar_set_property (GObject         *object,
		      guint            prop_id,
		      const GValue    *value,
		      GParamSpec      *pspec)
{
  GtkPandaProgressBar *pbar;

  g_return_if_fail(GTK_IS_PANDA_PROGRESS_BAR(object));
  pbar = GTK_PANDA_PROGRESS_BAR (object);

  switch (prop_id)
    {
    case PROP_LOWER:
      gtk_panda_progress_bar_set_lower(pbar,g_value_get_int(value));
      break;
    case PROP_UPPER:
      gtk_panda_progress_bar_set_upper(pbar,g_value_get_int(value));
      break;
    case PROP_VALUE:
      gtk_panda_progress_bar_set_value(pbar,g_value_get_int(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void gtk_panda_progress_bar_get_property (GObject         *object,
				  guint            prop_id,
				  GValue          *value,
				  GParamSpec      *pspec)
{
  GtkPandaProgressBar *pbar;

  g_return_if_fail(GTK_IS_PANDA_PROGRESS_BAR(object));
  pbar = GTK_PANDA_PROGRESS_BAR (object);

  switch (prop_id)
    {
    case PROP_LOWER:
      g_value_set_int (value, gtk_panda_progress_bar_get_lower(pbar));
      break;
    case PROP_UPPER:
      g_value_set_int (value, gtk_panda_progress_bar_get_upper(pbar));
      break;
    case PROP_VALUE:
      g_value_set_int (value, gtk_panda_progress_bar_get_value(pbar));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

// public API
void 
gtk_panda_progress_bar_set_lower(
  GtkPandaProgressBar *pbar,
  gint lower)
{
  pbar->lower = lower;
  gtk_panda_progress_bar_set_value(pbar,pbar->value);
}

void 
gtk_panda_progress_bar_set_upper(
  GtkPandaProgressBar *pbar,
  gint upper)
{
  pbar->upper = upper;
  gtk_panda_progress_bar_set_value(pbar,pbar->value);
}

void
gtk_panda_progress_bar_set_value(
  GtkPandaProgressBar *pbar,
  gint value)
{
  int l,u,v;
  if (pbar->lower < 0) {
    g_warning("lower(%d) < 0",pbar->lower);
    return;
  }
  if (pbar->upper < 0) {
    g_warning("upper(%d) < 0",pbar->upper);
    return;
  }
  if (pbar->lower >= pbar->upper) {
    g_warning("lower(%d) >= upper(%d)",pbar->lower,pbar->upper);
    return;
  }
  if (pbar->lower > value) {
    g_warning("lower(%d) > value(%d)",pbar->lower,value);
    return;
  }

  pbar->value = value;
  l = 0; 
  u = pbar->upper - pbar->lower;
  v = value - pbar->lower;
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pbar),
    (v * 1.0) / (u * 1.0));
}

gint 
gtk_panda_progress_bar_get_lower(
  GtkPandaProgressBar *pbar)
{
  return pbar->lower;
}

gint 
gtk_panda_progress_bar_get_upper(
  GtkPandaProgressBar *pbar)
{
  return pbar->upper;
}

gint 
gtk_panda_progress_bar_get_value(
  GtkPandaProgressBar *pbar)
{
  return pbar->value;
}
