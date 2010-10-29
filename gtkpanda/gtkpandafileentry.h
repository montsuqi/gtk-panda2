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

#ifndef __GTK_PANDA_FILE_ENTRY_H__
#define __GTK_PANDA_FILE_ENTRY_H__

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_PANDA_TYPE_FILE_ENTRY \
  (gtk_panda_file_entry_get_type ())
#define GTK_PANDA_FILE_ENTRY(obj) \
  (GTK_CHECK_CAST ((obj), GTK_PANDA_TYPE_FILE_ENTRY, GtkPandaFileEntry))
#define GTK_PANDA_FILE_ENTRY_CLASS(klass) \
  (GTK_CHECK_CLASS_CAST ((klass), GTK_PANDA_TYPE_FILE_ENTRY, GtkPandaFileEntryClass))
#define GTK_IS_PANDA_FILE_ENTRY(obj) \
  (GTK_CHECK_TYPE ((obj), GTK_PANDA_TYPE_FILE_ENTRY))
#define GTK_IS_PANDA_FILE_ENTRY_CLASS(klass) \
  (GTK_CHECK_CLASS_TYPE ((klass), GTK_PANDA_TYPE_FILE_ENTRY))

typedef struct _GtkPandaFileEntry       GtkPandaFileEntry;
typedef struct _GtkPandaFileEntryClass  GtkPandaFileEntryClass;

struct _GtkPandaFileEntry
{
  GtkHBox parent;

  GtkWidget *entry;
  GtkWidget *button;
  GtkFileChooserAction mode;
  int size;
  char *data;
  char *folder;
};

struct _GtkPandaFileEntryClass
{
  GtkHBoxClass parent_class;
  void (*browse_clicked) (GtkPandaFileEntry *fe);
  void (*done_action) (GtkPandaFileEntry *fe);
};

extern GType gtk_panda_file_entry_get_type (void);
extern GtkWidget *gtk_panda_file_entry_new (void);
extern void gtk_panda_file_entry_set_mode (GtkPandaFileEntry *fe, GtkFileChooserAction mode);
extern GtkFileChooserAction gtk_panda_file_entry_get_mode (GtkPandaFileEntry *fe);
extern void gtk_panda_file_entry_set_data (GtkPandaFileEntry *fe, int size, char *buff);
extern void gtk_panda_file_entry_set_folder (GtkPandaFileEntry *fe, char *folder);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_PANDA_FILE_ENTRY_H__ */
