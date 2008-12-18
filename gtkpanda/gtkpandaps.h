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

#ifndef __GTK_PANDA_PS_H__
#define __GTK_PANDA_PS_H__

#include <gtk/gtk.h>
#include "gtkgs.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GTK_PANDA_TYPE_PS \
  (gtk_panda_ps_get_type ())
#define GTK_PANDA_PS(obj) \
  (GTK_CHECK_CAST ((obj), GTK_PANDA_TYPE_PS, GtkPandaPS))
#define GTK_PANDA_PS_CLASS(klass) \
  (GTK_CHECK_CLASS_CAST ((klass), GTK_PANDA_TYPE_PS, GtkPandaPSClass))
#define GTK_IS_PANDA_PS(obj) \
  (GTK_CHECK_TYPE ((obj), GTK_PANDA_TYPE_PS))
#define GTK_IS_PANDA_PS_CLASS(klass) \
  (GTK_CHECK_CLASS_TYPE ((klass), GTK_PANDA_TYPE_PS))

typedef struct _GtkPandaPS       GtkPandaPS;
typedef struct _GtkPandaPSClass  GtkPandaPSClass;

struct _GtkPandaPS
{
  GtkVBox parent;
  GtkWidget *gs;
  GtkWidget *scale;
  int pan;
  int prev_x, prev_y;
};

struct _GtkPandaPSClass
{
  GtkVBoxClass parent_class;
  void (*zoom_fit_page)  (GtkPandaPS *ps);
  void (*zoom_fit_width) (GtkPandaPS *ps);
  void (*zoom_in)        (GtkPandaPS *ps);
  void (*zoom_out)       (GtkPandaPS *ps);
  void (*save)           (GtkPandaPS *ps);
  void (*print)       (GtkPandaPS *ps);
};

extern GType gtk_panda_ps_get_type (void);
extern GtkWidget *gtk_panda_ps_new (void);
extern void gtk_panda_ps_load (GtkPandaPS *pps, const char *filename);
extern void gtk_panda_ps_set (GtkPandaPS *pps, int size, char *buff);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_PANDA_PS_H__ */
