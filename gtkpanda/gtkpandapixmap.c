/* GTK - The GIMP Toolkit
 * Copyright (C) 2010- NaCl
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
#include <unistd.h>

#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "config.h"
#include "gtkpandaintl.h"
#include "gtkpandapixmap.h"

static GtkWidgetClass *parent_class = NULL;

static void
gtk_panda_pixmap_class_init (GtkPandaPixmapClass *klass)
{
  parent_class = gtk_type_class (GTK_TYPE_IMAGE);
}

static void
gtk_panda_pixmap_init (GtkPandaPixmap *pixmap)
{
}

GtkWidget*
gtk_panda_pixmap_new (void)
{
  return g_object_new (GTK_PANDA_TYPE_PIXMAP, NULL);
}

GType
gtk_panda_pixmap_get_type (void)
{
  static GType type = 0;

  if (!type) {
      static const GTypeInfo info =
      {
        sizeof (GtkPandaPixmapClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_pixmap_class_init,
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaPixmap),
        0,
        (GInstanceInitFunc) gtk_panda_pixmap_init
      };
      type = g_type_register_static( GTK_TYPE_IMAGE,
                                     "GtkPandaPixmap",
                                     &info,
                                     0);
  }
  return type;
}

void
gtk_panda_pixmap_set_image(GtkPandaPixmap *pixmap,
  gchar *buf,
  gsize size)
{
  GdkPixbuf *pixbuf;
  gchar *filename;
  GtkRequisition req;
  
  int fd = g_file_open_tmp("gtk_panda_pixmap_XXXXXX", &filename, NULL);
  if (fd == 1) {
    return;
  }
  FILE *fp = fdopen(fd, "wb");
  if (fp != NULL) {
    fwrite(buf,1,size,fp);
    fclose(fp);
  } else {
    return;
  }
  gtk_widget_size_request(GTK_WIDGET(pixmap), &req);
  pixbuf = gdk_pixbuf_new_from_file_at_size(filename,
    req.width, req.height, NULL);
  if (pixbuf != NULL) {
    gtk_image_set_from_pixbuf(GTK_IMAGE(pixmap), pixbuf);
  }
  unlink(filename); 
  g_free(filename);
}
