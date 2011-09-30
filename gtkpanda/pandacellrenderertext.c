/* panda-cell-renderer-text.c
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
 * copy from gnumeric-cell-renderer-text.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib-object.h>
#include <gtk/gtk.h>

#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include "pandacellrenderertext.h"

static void panda_cell_renderer_text_class_init
    (PandaCellRendererTextClass *cell_text_class);
static GtkCellEditable *
start_editing (GtkCellRenderer      *cell,
				      GdkEvent             *event,
				      GtkWidget            *widget,
				      const gchar          *path,
				      GdkRectangle         *background_area,
				      GdkRectangle         *cell_area,
				      GtkCellRendererState  flags);

static GtkCellRendererTextClass *parent_class = NULL;
static GtkWidget *entry;
static gulong focus_out_id;

#define PANDA_CELL_RENDERER_TEXT_PATH "panda-cell-renderer-text-path"

GType
panda_cell_renderer_text_get_type (void)
{
	static GType cell_text_type = 0;

	if (!cell_text_type)
	{
		static const GTypeInfo cell_text_info =
			{
				sizeof (PandaCellRendererTextClass),
				NULL,		/* base_init */
				NULL,		/* base_finalize */
				(GClassInitFunc)panda_cell_renderer_text_class_init,
				NULL,		/* class_finalize */
				NULL,		/* class_data */
				sizeof (PandaCellRendererText),
				0,              /* n_preallocs */
				(GInstanceInitFunc) NULL,
			};

		cell_text_type = g_type_register_static (GTK_TYPE_CELL_RENDERER_TEXT, "PandaCellRendererText", &cell_text_info, 0);
	}

	return cell_text_type;
}

static void
panda_cell_renderer_text_class_init (PandaCellRendererTextClass *class)
{
	GtkCellRendererClass *cell_class = GTK_CELL_RENDERER_CLASS  (class);
	GObjectClass *object_class = G_OBJECT_CLASS (class);
    cell_class->start_editing = start_editing;

	parent_class = g_type_class_peek_parent (object_class);
}


GtkCellRenderer *
panda_cell_renderer_text_new (void)
{
	return GTK_CELL_RENDERER (g_object_new (PANDA_TYPE_CELL_RENDERER_TEXT, NULL));
}

static void
editing_done (GtkCellEditable *_entry,
				     gpointer         data)
{
  const gchar *path;
  const gchar *new_text;
  gboolean canceled;

  entry = NULL;

  if (focus_out_id > 0)
    {
      g_signal_handler_disconnect (_entry, focus_out_id);
      focus_out_id = 0;
    }

  g_object_get (_entry,
                "editing-canceled", &canceled,
                NULL);
  gtk_cell_renderer_stop_editing (GTK_CELL_RENDERER (data), canceled);

  if (canceled)
    return;

  path = g_object_get_data (G_OBJECT (_entry), PANDA_CELL_RENDERER_TEXT_PATH);
  new_text = gtk_entry_get_text (GTK_ENTRY (_entry));

  g_signal_emit_by_name (data, "edited", path, new_text);
}


static gboolean
focus_out_event (GtkWidget *entry,
		                        GdkEvent  *event,
					gpointer   data)
{
  g_object_set (entry,
                "editing-canceled", TRUE,
                NULL);
  gtk_cell_editable_editing_done (GTK_CELL_EDITABLE (entry));
  gtk_cell_editable_remove_widget (GTK_CELL_EDITABLE (entry));

  /* entry needs focus-out-event */
  return FALSE;
}

static GtkCellEditable *
start_editing (GtkCellRenderer      *cell,
				      GdkEvent             *event,
				      GtkWidget            *widget,
				      const gchar          *path,
				      GdkRectangle         *background_area,
				      GdkRectangle         *cell_area,
				      GtkCellRendererState  flags)
{
  GtkRequisition requisition;
  GtkCellRendererText *celltext;
  gboolean editable;
  gfloat xalign;
  gchar *text;

  celltext = GTK_CELL_RENDERER_TEXT (cell);

  /* If the cell isn't editable we return NULL. */
  g_object_get(celltext,
    "editable",&editable,
    "xalign",&xalign,
    "text",&text,
    NULL);
  if (editable == FALSE)
    return NULL;

  entry = g_object_new (GTK_TYPE_ENTRY,
			      "has-frame", FALSE,
			      "xalign", xalign,
			      NULL);

  gtk_entry_set_text (GTK_ENTRY (entry), text);
  g_free(text);

  g_object_set_data_full (G_OBJECT (entry), PANDA_CELL_RENDERER_TEXT_PATH, g_strdup (path), g_free);
  
  gtk_widget_size_request (entry, &requisition);
  if (requisition.height < cell_area->height)
    {
      GtkBorder *style_border;
      GtkBorder border;

      gtk_widget_style_get (entry,
			    "inner-border", &style_border,
			    NULL);

      if (style_border)
        {
	  border = *style_border;
	  g_boxed_free (GTK_TYPE_BORDER, style_border);
	}
      else
        {
	  /* Since boxed style properties can't have default values ... */
	  border.left = 2;
	  border.right = 2;
	}

      border.top = (cell_area->height - requisition.height) / 2;
      border.bottom = (cell_area->height - requisition.height) / 2;
      gtk_entry_set_inner_border (GTK_ENTRY (entry), &border);
    }


  g_signal_connect (entry,
		    "editing-done",
		    G_CALLBACK (editing_done),
		    celltext);
  focus_out_id = g_signal_connect_after (entry, "focus-out-event",
					       G_CALLBACK (focus_out_event),
					       celltext);
  gtk_widget_show (entry);
  gtk_editable_set_position(GTK_EDITABLE(entry),-1);

  return GTK_CELL_EDITABLE (entry);
}

GtkWidget*
panda_cell_renderer_text_get_entry(PandaCellRendererText *rend)
{
  return entry;
}
