/* pandaTEXT - TEXT widget for gtk+
 * Copyright 1997 Paolo Molaro
 * Copyright 2001 AXE, Inc.
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
 *
 * Based on gtk/gtkTEXT.h
 */

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#ifndef __GTK_PANDA_TEXT_H__
#define __GTK_PANDA_TEXT_H__

#include <gtk/gtktextview.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_PANDA_TYPE_TEXT \
  (gtk_panda_text_get_type ())
#define GTK_PANDA_TEXT(obj)	\
  GTK_CHECK_CAST (obj, gtk_panda_text_get_type (), GtkPandaText)
#define GTK_PANDA_TEXT_CLASS(klass)	\
  GTK_CHECK_CLASS_CAST (klass, gtk_panda_text_get_type (), GtkPandaTextClass)
#define GTK_PANDA_IS_TEXT(obj) \
  GTK_CHECK_TYPE (obj, gtk_panda_text_get_type ())

typedef struct _GtkPandaText		GtkPandaText;
typedef struct _GtkPandaTextClass	GtkPandaTextClass;

/* you should access only the entry and list fields directly */
struct _GtkPandaText {
  GtkTextView textview;
  
  gboolean xim_enabled;
};

struct _GtkPandaTextClass {
  GtkTextViewClass parent_class;

  void (*activate) (GtkPandaText *widget);
  void (*changed) (GtkPandaText *widget);
};

GType gtk_panda_text_get_type (void);
GtkWidget* gtk_panda_text_new (void);
void gtk_panda_text_set_xim_enabled (GtkPandaText *text, gboolean flag);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_PANDA_TEXT_H__ */
