/* panda-cell-renderer-text.h 
 * Copyright 2011 NaCl
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
 * copy from gnumeric-cell-renderer-text.h
 */

#ifndef __PANDA_CELL_RENDERER_TEXT_H__
#define __PANDA_CELL_RENDERER_TEXT_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define PANDA_TYPE_CELL_RENDERER_TEXT	 (panda_cell_renderer_text_get_type ())
#define PANDA_CELL_RENDERER_TEXT(o)		 (G_TYPE_CHECK_INSTANCE_CAST ((o), PANDA_TYPE_CELL_RENDERER_TEXT, PandaCellRendererText))
#define PANDA_CELL_RENDERER_TEXT_CLASS(k)	 (G_TYPE_CHECK_CLASS_CAST ((k), PANDA_TYPE_CELL_RENDERER_TEXT, PandaCellRendererTextClass))
#define PANDA_IS_CELL_RENDERER_TEXT(o)	 (G_TYPE_CHECK_INSTANCE_TYPE ((o), PANDA_TYPE_CELL_RENDERER_TEXT))
#define PANDA_IS_CELL_RENDERER_TEXT_CLASS(k)	 (G_TYPE_CHECK_CLASS_TYPE ((k), PANDA_TYPE_CELL_RENDERER_TEXT))
#define PANDA_CELL_RENDERER_TEXT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), PANDA_TYPE_CELL_RENDERER_TEXT, PandaCellRendererTextClass))

typedef struct _PandaCellRendererText      PandaCellRendererText;
typedef struct _PandaCellRendererTextClass PandaCellRendererTextClass;

struct _PandaCellRendererText
{
  GtkCellRendererText parent;
};

struct _PandaCellRendererTextClass
{
  GtkCellRendererTextClass parent_class;

  void (*edited_by_key) (PandaCellRendererText *p,
    gchar *path,
    gchar *text);
};

GType            panda_cell_renderer_text_get_type (void);
GtkCellRenderer *panda_cell_renderer_text_new      (void);
GtkIMContext	*panda_cell_renderer_text_get_im(PandaCellRendererText* rend);

G_END_DECLS


#endif /* __GTK_CELL_RENDERER_TEXT_H__ */
