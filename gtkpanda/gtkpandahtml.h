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

#ifndef __GTK_PANDA_HTML_H__
#define __GTK_PANDA_HTML_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include <webkit/webkit.h>

G_BEGIN_DECLS

#define GTK_PANDA_TYPE_HTML                  (gtk_panda_ht_m_l_get_type ())
#define GTK_PANDA_HTML(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_PANDA_TYPE_HTML, GtkPandaHTML))
#define GTK_PANDA_HTML_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_PANDA_TYPE_HTML, GtkPandaHTMLClass))
#define GTK_IS_PANDA_HTML(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_PANDA_TYPE_HTML))
#define GTK_IS_PANDA_HTML_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_PANDA_TYPE_HTML))

#define MAX_HTML_SIZE   1048576 /* 1M */

typedef struct _GtkPandaHTML       GtkPandaHTML;
typedef struct _GtkPandaHTMLClass  GtkPandaHTMLClass;

struct _GtkPandaHTML
{
  GtkBox parent;

  GtkWidget *scroll;
  GtkWidget *webview;
};

struct _GtkPandaHTMLClass
{
  GtkBoxClass parent_class;

  void (*activate) (GtkPandaHTML *html, gpointer data);
};

GType gtk_panda_ht_m_l_get_type (void);
GtkWidget* gtk_panda_html_new (void);
gchar *gtk_panda_html_get_uri (GtkPandaHTML *html);
void gtk_panda_html_set_uri (GtkPandaHTML *html, const gchar *uri);

G_END_DECLS

#endif /* __GTK_PANDA_HTML_H__ */
