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

#ifndef __GTK_PANDA_FILEENTRY_H__
#define __GTK_PANDA_FILEENTRY_H__

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_PANDA_TYPE_FILEENTRY \
  (gtk_panda_fileentry_get_type ())
#define GTK_PANDA_FILEENTRY(obj) \
  (GTK_CHECK_CAST ((obj), GTK_PANDA_TYPE_FILEENTRY, GtkPandaFileentry))
#define GTK_PANDA_FILEENTRY_CLASS(klass) \
  (GTK_CHECK_CLASS_CAST ((klass), GTK_PANDA_TYPE_FILEENTRY, GtkPandaFileentryClass))
#define GTK_IS_PANDA_FILEENTRY(obj) \
  (GTK_CHECK_TYPE ((obj), GTK_PANDA_TYPE_FILEENTRY))
#define GTK_IS_PANDA_FILEENTRY_CLASS(klass) \
  (GTK_CHECK_CLASS_TYPE ((klass), GTK_PANDA_TYPE_FILEENTRY))

typedef struct _GtkPandaFileentry       GtkPandaFileentry;
typedef struct _GtkPandaFileentryClass  GtkPandaFileentryClass;

struct _GtkPandaFileentry
{
  GtkHBox parent;

  GtkWidget *entry;
  GtkWidget *button;
  GtkFileChooserAction mode;
  int size;
  char *data;
  char *folder;
};

struct _GtkPandaFileentryClass
{
  GtkHBoxClass parent_class;
  void (*browse_clicked) (GtkPandaFileentry *fe);
  void (*done_action) (GtkPandaFileentry *fe);
};

extern GType gtk_panda_fileentry_get_type (void);
extern GtkWidget *gtk_panda_fileentry_new (void);
extern void gtk_panda_fileentry_set_mode (GtkPandaFileentry *fe, GtkFileChooserAction mode);
extern GtkFileChooserAction gtk_panda_fileentry_get_mode (GtkPandaFileentry *fe);
extern void gtk_panda_fileentry_set_data (GtkPandaFileentry *fe, int size, char *buff);
extern void gtk_panda_fileentry_set_folder (GtkPandaFileentry *fe, char *folder);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_PANDA_FILEENTRY_H__ */
