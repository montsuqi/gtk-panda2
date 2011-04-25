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

#ifndef __GTK_PANDA_WINDOW_H__
#define __GTK_PANDA_WINDOW_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_PANDA_TYPE_WINDOW                  (gtk_panda_window_get_type ())
#define GTK_PANDA_WINDOW(obj)                  (GTK_CHECK_CAST ((obj), GTK_PANDA_TYPE_WINDOW, GtkPandaWindow))
#define GTK_PANDA_WINDOW_CLASS(klass)          (GTK_CHECK_CLASS_CAST ((klass), GTK_PANDA_TYPE_WINDOW, GtkPandaWindowClass))
#define GTK_IS_PANDA_WINDOW(obj)               (GTK_CHECK_TYPE ((obj), GTK_PANDA_TYPE_WINDOW))
#define GTK_IS_PANDA_WINDOW_CLASS(klass)       (GTK_CHECK_CLASS_TYPE ((klass), GTK_PANDA_TYPE_WINDOW))

typedef struct _GtkPandaWindow       GtkPandaWindow;
typedef struct _GtkPandaWindowClass  GtkPandaWindowClass;

struct _GtkPandaWindow
{
  GtkWindow window;

  gchar *wmclass_class;
  gchar *wmclass_name;
  gint x;
  gint y;
};

struct _GtkPandaWindowClass
{
  GtkWindowClass parent_class;
};

GType gtk_panda_window_get_type (void);
GtkWidget* gtk_panda_window_new (GtkWindowType type);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_PANDA_WINDOW_H__ */
