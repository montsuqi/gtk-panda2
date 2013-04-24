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

#ifndef __GTK_PANDA_DOWNLOAD_H__
#define __GTK_PANDA_DOWNLOAD_H__

#include <glib-object.h>
#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_PANDA_TYPE_DOWNLOAD2                  (gtk_panda_download2_get_type ())
#define GTK_PANDA_DOWNLOAD2(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_PANDA_TYPE_DOWNLOAD2, GtkPandaDownload2))
#define GTK_PANDA_DOWNLOAD2_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_PANDA_TYPE_DOWNLOAD2, GtkPandaDownload2Class))
#define GTK_IS_PANDA_DOWNLOAD2(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_PANDA_TYPE_DOWNLOAD2))
#define GTK_IS_PANDA_DOWNLOAD2_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_PANDA_TYPE_DOWNLOAD2))

typedef struct _GtkPandaDownload2       GtkPandaDownload2;
typedef struct _GtkPandaDownload2Class  GtkPandaDownload2Class;

struct _GtkPandaDownload2
{
  GtkWidget widget;
};

struct _GtkPandaDownload2Class
{
  GtkWidgetClass parent_class;
};

GType gtk_panda_download2_get_type (void);
GtkWidget* gtk_panda_download2_new (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_PANDA_DOWNLOAD_H__ */
