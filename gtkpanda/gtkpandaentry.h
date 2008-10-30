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

#ifndef __GTK_PANDA_ENTRY_H__
#define __GTK_PANDA_ENTRY_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gtk/gtkentry.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_PANDA_TYPE_ENTRY                  (gtk_panda_entry_get_type ())
#define GTK_PANDA_ENTRY(obj)                  (GTK_CHECK_CAST ((obj), GTK_PANDA_TYPE_ENTRY, GtkPandaEntry))
#define GTK_PANDA_ENTRY_CLASS(klass)          (GTK_CHECK_CLASS_CAST ((klass), GTK_PANDA_TYPE_ENTRY, GtkPandaEntryClass))
#define GTK_IS_PANDA_ENTRY(obj)               (GTK_CHECK_TYPE ((obj), GTK_PANDA_TYPE_ENTRY))
#define GTK_IS_PANDA_ENTRY_CLASS(klass)       (GTK_CHECK_CLASS_TYPE ((klass), GTK_PANDA_TYPE_ENTRY))

typedef struct _GtkPandaEntry       GtkPandaEntry;
typedef struct _GtkPandaEntryClass  GtkPandaEntryClass;

enum gtk_panda_entry_input_mode {
  GTK_PANDA_ENTRY_ASCII_MODE,
  GTK_PANDA_ENTRY_KANA_MODE,
  GTK_PANDA_ENTRY_XIM_MODE
};

struct _GtkPandaEntry
{
  GtkEntry entry;

  enum gtk_panda_entry_input_mode input_mode;
  gboolean xim_enabled;
};

struct _GtkPandaEntryClass
{
  GtkEntryClass parent_class;
};

GType gtk_panda_entry_get_type (void);
GtkWidget* gtk_panda_entry_new (void);
void gtk_panda_entry_set_input_mode (
	GtkPandaEntry *entry, 
	enum gtk_panda_entry_input_mode mode);
void gtk_panda_entry_set_xim_enabled (GtkPandaEntry *entry, gboolean flag);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_PANDA_ENTRY_H__ */
