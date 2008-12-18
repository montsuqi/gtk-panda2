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

#ifndef __GTK_PANDA_PDF_H__
#define __GTK_PANDA_PDF_H__

#ifdef HAVE_POPPLER

#include <gtk/gtk.h>
#include <poppler.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_PANDA_TYPE_PDF \
  (gtk_panda_pdf_get_type ())
#define GTK_PANDA_PDF(obj) \
  (GTK_CHECK_CAST ((obj), GTK_PANDA_TYPE_PDF, GtkPandaPDF))
#define GTK_PANDA_PDF_CLASS(klass) \
  (GTK_CHECK_CLASS_CAST ((klass), GTK_PANDA_TYPE_PDF, GtkPandaPDFClass))
#define GTK_IS_PANDA_PDF(obj) \
  (GTK_CHECK_TYPE ((obj), GTK_PANDA_TYPE_PDF))
#define GTK_IS_PANDA_PDF_CLASS(klass) \
  (GTK_CHECK_CLASS_TYPE ((klass), GTK_PANDA_TYPE_PDF))

typedef struct _GtkPandaPDF       GtkPandaPDF;
typedef struct _GtkPandaPDFClass  GtkPandaPDFClass;

struct _GtkPandaPDF
{
  GtkVBox parent;

  GtkWidget *scale;
  GtkWidget *scroll;
  GtkWidget *image;
  GdkPixbuf *pixbuf;
  PopplerDocument *doc;
  double zoom;
  int pageno;
  int size;
  char *data;
};

struct _GtkPandaPDFClass
{
  GtkVBoxClass parent_class;
  void (*zoom_fit_page)  (GtkPandaPDF *pdf);
  void (*zoom_fit_width) (GtkPandaPDF *pdf);
  void (*zoom_in)        (GtkPandaPDF *pdf);
  void (*zoom_out)       (GtkPandaPDF *pdf);
  void (*page_next)      (GtkPandaPDF *pdf);
  void (*page_prev)      (GtkPandaPDF *pdf);
  void (*save)           (GtkPandaPDF *pdf);
  void (*print)          (GtkPandaPDF *pdf);
};

extern GType gtk_panda_pdf_get_type (void);
extern GtkWidget *gtk_panda_pdf_new (void);
extern void gtk_panda_pdf_set (GtkPandaPDF *pps, int size, char *buff);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

#endif /* __GTK_PANDA_PDF_H__ */
