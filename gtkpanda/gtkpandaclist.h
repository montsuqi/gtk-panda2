/* pandaCLIST - CLIST widget for gtk+
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
 * Based on gtk/gtkCLIST.h
 */

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#ifndef __GTK_PANDA_CLIST_H__
#define __GTK_PANDA_CLIST_H__

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_PANDA_TYPE_CLIST			(gtk_panda_clist_get_type ())
#define GTK_PANDA_CLIST(obj)			G_TYPE_CHECK_INSTANCE_CAST (obj, gtk_panda_clist_get_type (), GtkPandaCList)
#define GTK_PANDA_CLIST_CLASS(klass)	G_TYPE_CHECK_CLASS_CAST (klass, gtk_panda_clist_get_type (), GtkPandaCListClass)
#define GTK_IS_PANDA_CLIST(obj)			G_TYPE_CHECK_INSTANCE_TYPE (obj, gtk_panda_clist_get_type ())

typedef struct _GtkPandaCList		GtkPandaCList;
typedef struct _GtkPandaCListClass	GtkPandaCListClass;

/* you should access only the entry and list fields directly */
struct _GtkPandaCList {
  GtkTreeView tree;

  gboolean show_titles;
  gchar *column_widths;
  int columns;
  int prev_selected_num;
  int nrows;
  GtkSelectionMode selection_mode;
};

struct _GtkPandaCListClass {
  GtkTreeViewClass parent_class;

  void (* select_row) (GtkPandaCList *clist, int column, int row);
  void (* unselect_row) (GtkPandaCList *clist, int column, int row);
};

GType gtk_panda_clist_get_type (void);

GtkWidget *gtk_panda_clist_new ();

void gtk_panda_clist_set_column_widths(GtkPandaCList *clist, const gchar *column_widths);
void gtk_panda_clist_set_column_width(GtkPandaCList *clist, gint column,gint width);

void gtk_panda_clist_set_selection_mode(GtkPandaCList *clist,GtkSelectionMode mode);
GtkSelectionMode gtk_panda_clist_get_selection_mode(GtkPandaCList *clist);
void gtk_panda_clist_moveto(GtkPandaCList *clist,gint row,gint column,gfloat row_align,gfloat col_align);

gboolean gtk_panda_clist_row_is_visible (GtkPandaCList *clist, gint row);
gboolean gtk_panda_clist_row_is_selected (GtkPandaCList *clist, gint row);

gint gtk_panda_clist_get_columns(GtkPandaCList *clist);
void gtk_panda_clist_set_columns(GtkPandaCList *clist, gint n_columns);

void gtk_panda_clist_set_row(GtkPandaCList *clist,int row,gchar *rowdata[]);
gint gtk_panda_clist_get_rows(GtkPandaCList *clist);
void gtk_panda_clist_set_rows(GtkPandaCList *clist,int nrows);

void gtk_panda_clist_select_row (GtkPandaCList *clist,gint row,gint column);
void gtk_panda_clist_unselect_row (GtkPandaCList *clist, gint row,gint column);

void gtk_panda_clist_set_show_titles(GtkPandaCList *clist, gboolean show_titles);
void gtk_panda_clist_set_bgcolor(GtkPandaCList *clist,int row,gchar *c);
void gtk_panda_clist_set_fgcolor(GtkPandaCList *clist,int row,gchar *c);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_PANDA_CLIST_H__ */
