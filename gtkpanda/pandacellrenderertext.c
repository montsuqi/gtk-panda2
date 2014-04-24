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
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>
#include "gtkpandaentry.h"
#include "gtkpandatable.h"
#include "pandacellrenderertext.h"
#include "pandamarshal.h"
#include "imcontrol.h"

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

enum
{
  EDITED_BY_KEY,
  LAST_SIGNAL 
};

static GtkCellRendererTextClass *parent_class = NULL;
static GtkWidget *entry;
static gulong focus_out_id;
static guint signals[LAST_SIGNAL] = {0};
static guint keyval = 0;

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
        NULL,    /* base_init */
        NULL,    /* base_finalize */
        (GClassInitFunc)panda_cell_renderer_text_class_init,
        NULL,    /* class_finalize */
        NULL,    /* class_data */
        sizeof (PandaCellRendererText),
        0,              /* n_preallocs */
        (GInstanceInitFunc) NULL
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

  signals[EDITED_BY_KEY] =
  g_signal_new ("edited-by-key",
    G_TYPE_FROM_CLASS (object_class),
    G_SIGNAL_RUN_FIRST,
    G_STRUCT_OFFSET (PandaCellRendererTextClass, edited_by_key),
    NULL, NULL,
    panda_marshal_VOID__STRING_STRING,
    G_TYPE_NONE, 2,
    G_TYPE_STRING,
    G_TYPE_STRING);
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

  if (focus_out_id > 0) {
      if (g_signal_handler_is_connected(_entry, focus_out_id)) {
      g_signal_handler_disconnect (_entry, focus_out_id);
      }
      focus_out_id = 0;
  }

  g_object_get(_entry,"editing-canceled",&canceled,NULL);
  gtk_cell_renderer_stop_editing (GTK_CELL_RENDERER (data), canceled);

  path = g_object_get_data(G_OBJECT(_entry),PANDA_CELL_RENDERER_TEXT_PATH);
  new_text = gtk_entry_get_text(GTK_ENTRY(_entry));

  if (keyval != 0) {
    keyval = 0;
    g_signal_emit_by_name(data,"edited-by-key",path,new_text);
  } else {
    g_signal_emit_by_name(data,"edited",path,new_text);
  }
  keyval = 0;
}

static gboolean
focus_out_event (
  GtkWidget *entry,
  GdkEvent  *event,
  gpointer   data)
{
  g_object_set(entry,"editing-canceled",TRUE,NULL);
  gtk_cell_editable_editing_done(GTK_CELL_EDITABLE(entry));
  gtk_cell_editable_remove_widget(GTK_CELL_EDITABLE(entry));

  /* entry needs focus-out-event */
  return FALSE;
}

static gboolean
editing_entry_key_press (GtkWidget *entry,
  GdkEventKey *event,
  gpointer data)
{
  GtkCellRendererText *renderer;
  GtkTreeView *view;
  GtkTreePath *path;
  GtkTreeViewColumn *column;
  int n;

  g_return_val_if_fail(GTK_IS_ENTRY(entry),TRUE);
  keyval = 0;

  renderer = GTK_CELL_RENDERER_TEXT(data);
  view = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(renderer),"treeview"));
  n = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(renderer),"column_num"));
  path = NULL;

  switch(event->keyval) {
  case GDK_Return:
  case GDK_KP_Enter:
    if (!(event->state & GDK_SHIFT_MASK ) && 
        !(event->state & GDK_CONTROL_MASK) && 
        !(event->state & GDK_MOD1_MASK) ) {
      keyval = event->keyval;
    }
    break;
  case GDK_Up:
  case GDK_KP_Up:
    if (!(event->state & GDK_SHIFT_MASK ) && 
        !(event->state & GDK_CONTROL_MASK) && 
        !(event->state & GDK_MOD1_MASK) ) {
      if (view != NULL) {
        gtk_tree_view_get_cursor(view,&path,&column);
        if (path != NULL && column != NULL) {
          if (gtk_tree_path_prev(path)) {
            gtk_tree_view_set_cursor(view,path,column,TRUE);
          } else {
            return TRUE;
          }
        }
      }
    }
    break;
  case GDK_Down:
  case GDK_KP_Down:
    if (!(event->state & GDK_SHIFT_MASK ) && 
        !(event->state & GDK_CONTROL_MASK) && 
        !(event->state & GDK_MOD1_MASK) ) {
      if (view != NULL) {
        gtk_tree_view_get_cursor(view,&path,&column);
        if (path != NULL && column != NULL) {
          GtkTreeIter itr;
          GtkTreeModel *model;
 
          model = gtk_tree_view_get_model(view);
          gtk_tree_path_next(path);
          if (gtk_tree_model_get_iter(model,&itr,path)) {
            gtk_tree_view_set_cursor(view,path,column,TRUE);
          } else {
            return TRUE;
          }
        }
      }
    }
    break;
  case GDK_Tab:
  case GDK_KP_Tab:
  case GDK_ISO_Left_Tab:
    if (!(event->state & GDK_MOD1_MASK) && view != NULL) {
      if (!(event->state & GDK_CONTROL_MASK)) {
        if (!(event->state & GDK_SHIFT_MASK)) {
          // TAB
          GList *list = gtk_tree_view_get_columns(view);
          gtk_tree_view_get_cursor(view,&path,&column);
          if ((n+1) >= g_list_length(list)) {
            column = GTK_TREE_VIEW_COLUMN(g_list_nth_data(list,0));
          } else {
            column = GTK_TREE_VIEW_COLUMN(g_list_nth_data(list,n+1));
          }
          g_list_free(list);
          gtk_tree_view_set_cursor(view,path,column,TRUE);
        } else {
          // SHIFT + TAB
          GList *list = gtk_tree_view_get_columns(view);
          gtk_tree_view_get_cursor(view,&path,&column);
          if (n <=0 ) {
            column = GTK_TREE_VIEW_COLUMN(
              g_list_nth_data(list,g_list_length(list)-1));
          } else {
            column = GTK_TREE_VIEW_COLUMN(g_list_nth_data(list,n-1));
          }
          g_list_free(list);
          gtk_tree_view_set_cursor(view,path,column,TRUE);
        }
      }
    }
    break;
  }
  if (path != NULL) {
    gtk_tree_path_free(path);
  }
  return FALSE;
}

static gboolean
cb_button_press_event(GtkWidget *widget,
  GdkEventButton *event,
  gpointer data)
{
  if (event->button == 1) {
    /* left click */
    return FALSE;
  } else {
    /* other click */
    return TRUE;
  }
}

static gint
_set_im(gpointer data)
{
  if (!GTK_IS_PANDA_ENTRY(data)) {
    return FALSE;
  }
#if IBUS_1_5
  {
    GtkIMMulticontext *mim;

    mim = GTK_IM_MULTICONTEXT(GTK_ENTRY(data)->im_context);
    if (mim != NULL) {
      if (mim->context_id != NULL && !strcmp("fcitx",mim->context_id)) {
        gtk_panda_entry_set_im(GTK_PANDA_ENTRY(data));
      }
    }
  }
#else
  gtk_panda_entry_set_im(GTK_PANDA_ENTRY(data));
#endif
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
  GtkPandaTable *table;
  GtkRequisition requisition;
  GtkCellRendererText *celltext;
  gboolean im_enabled;
  gpointer data;
  int i,column_num;

  celltext = GTK_CELL_RENDERER_TEXT(cell);
  table = GTK_PANDA_TABLE(widget);

  /* If the cell isn't editable we return NULL. */
  if (celltext->editable == FALSE)
    return NULL;

  column_num = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(cell),"column_num"));
  im_enabled = FALSE;
  if (GTK_IS_PANDA_TABLE(widget)) {
    im_enabled = gtk_panda_table_get_im_control(table,column_num);
  }

  entry = g_object_new (GTK_PANDA_TYPE_ENTRY,
            "has-frame", FALSE,
            "xalign", cell->xalign,
            NULL);

  if (celltext->text) {
    if (im_enabled) {
      gtk_panda_entry_set_xim_enabled(GTK_PANDA_ENTRY(entry),TRUE);
    }
    gtk_entry_set_text(GTK_ENTRY (entry),celltext->text);
  }

  g_object_set_data_full(G_OBJECT(entry),
    PANDA_CELL_RENDERER_TEXT_PATH, g_strdup(path),g_free);
  
  gtk_widget_size_request (entry, &requisition);
  if (requisition.height < cell_area->height) {
    GtkBorder *style_border;
    GtkBorder border;

    gtk_widget_style_get (entry,"inner-border",&style_border,NULL);
    if (style_border) {
      border = *style_border;
      g_boxed_free (GTK_TYPE_BORDER, style_border);
    } else {
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
  g_signal_connect (entry,
        "key-press-event",
        G_CALLBACK (editing_entry_key_press),
        celltext);
  g_signal_connect(entry,
         "button-press-event",
         G_CALLBACK(cb_button_press_event),
         NULL);
  focus_out_id = g_signal_connect_after (entry, "focus-out-event",
                 G_CALLBACK(focus_out_event), celltext);
  gtk_widget_show (entry);
  gtk_editable_set_position(GTK_EDITABLE(entry),-1);
  for (i=0;i<g_list_length(table->keyevents);i++) {
    data = g_list_nth_data(table->keyevents,i);
    gtk_entry_im_context_filter_keypress(GTK_ENTRY(entry),
      (GdkEventKey*)data);
    gdk_event_free((GdkEvent*)data);
  }
  g_list_free(table->keyevents);
  table->keyevents = NULL;

  g_idle_add(_set_im,entry);

  return GTK_CELL_EDITABLE (entry);
}
