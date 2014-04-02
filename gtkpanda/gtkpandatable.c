 /* 
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
 * Based on gtk/gtktreeview.c
 */
/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib-object.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "gtkpandaintl.h"
#include "gtkpandatable.h"
#include "pandacellrenderertext.h"
#include "pandamarshal.h"

enum {
PROP_0,
PROP_ROWS,
PROP_COLUMNS,
PROP_TYPES,
PROP_TITLES,
PROP_WIDTHS,
PROP_IM_CONTROLS
};

enum
{
  CELL_EDITED,
  LAST_SIGNAL 
}; 

static GtkTreeViewClass *parent_class = NULL;
static guint signals [LAST_SIGNAL] = { 0 };

static void gtk_panda_table_class_init (GtkPandaTableClass *klass);
static void gtk_panda_table_init (GtkPandaTable *table);

static void  gtk_panda_table_set_property(GObject *object,
  guint            prop_id,
  const GValue    *value,
  GParamSpec      *pspec);
static void  gtk_panda_table_get_property(GObject *object,
  guint            prop_id,
  GValue          *value,
  GParamSpec      *pspec);
static gboolean cb_button_release_event(GtkWidget *widget,
  GdkEvent *event,
  gpointer data);
static void cb_cursor_changed(
  GtkTreeView *,
  gpointer );
static gboolean gtk_panda_table_key_press(
  GtkWidget *widget,
  GdkEventKey *event);

static void
gtk_panda_table_class_init ( GtkPandaTableClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class;

  widget_class = (GtkWidgetClass *) klass;
  widget_class->key_press_event = gtk_panda_table_key_press;

  parent_class = g_type_class_ref(GTK_TYPE_TREE_VIEW);

  signals[CELL_EDITED] =
  g_signal_new ("cell-edited",
        G_TYPE_FROM_CLASS (gobject_class),
        G_SIGNAL_RUN_FIRST,
        G_STRUCT_OFFSET (GtkPandaTableClass, cell_edited),
        NULL, NULL,
        panda_marshal_VOID__INT_INT_POINTER,
        G_TYPE_NONE, 3,
        G_TYPE_INT,
        G_TYPE_INT,
        G_TYPE_STRING);

  gobject_class->set_property = gtk_panda_table_set_property; 
  gobject_class->get_property = gtk_panda_table_get_property; 

  g_object_class_install_property (gobject_class,
    PROP_ROWS,
    g_param_spec_int ("rows",
      _("Number of rows"),
      _("Number of rows"),
      0,
      GTK_PANDA_TABLE_MAX_ROWS,
      10,
      G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
    PROP_COLUMNS,
    g_param_spec_int ("columns",
      _("Number of columns"),
      _("Number of columns"),
      1,
      GTK_PANDA_TABLE_MAX_COLS,
      1,
      G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
    PROP_TYPES,
    g_param_spec_string ("column_types",
      _("The list of column type"),
      _("The list of column type(comma separated string)"),
      "text,text,text,text,text,text,text,text,text,text",
      G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
    PROP_TITLES,
    g_param_spec_string ("column_titles",
      _("The list of column title"),
      _("The list of column title(comma separated string)"),
      "",
      G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
    PROP_WIDTHS,
    g_param_spec_string ("column_widths",
      _("The list of column width"),
      _("The list of column width(comma separated string)"),
      "",
      G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
    PROP_IM_CONTROLS,
    g_param_spec_string ("im_controls",
      _("The list of enable im control flag"),
      _("The list of enable im control flag"),
      "",
      G_PARAM_READWRITE));
}

static void
gtk_panda_table_init ( GtkPandaTable * table)
{
  GtkTreeSelection *selection;
  
  table->types = g_strdup("");
  table->titles = g_strdup("");
  table->widths = g_strdup("");
  table->im_controls = g_strdup("");
  table->rows = 0;
  table->keyevents = NULL;
  gtk_panda_table_set_columns(table,1);
  gtk_tree_view_set_enable_search(GTK_TREE_VIEW(table), FALSE );
  gtk_tree_view_set_rubber_banding(GTK_TREE_VIEW(table), FALSE );
  gtk_tree_view_set_reorderable(GTK_TREE_VIEW(table), FALSE );
  gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(table), GTK_TREE_VIEW_GRID_LINES_VERTICAL);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(table));
  gtk_tree_selection_set_mode(selection,GTK_SELECTION_NONE);
  gtk_widget_set_can_focus(GTK_WIDGET(table),TRUE);

  g_signal_connect(G_OBJECT(table),"button-release-event",
    G_CALLBACK(cb_button_release_event),NULL);
  g_signal_connect(G_OBJECT(table),"cursor-changed",
    G_CALLBACK(cb_cursor_changed),NULL);
}

GType
gtk_panda_table_get_type (void)
{
  static GType table_type = 0;

  if (!table_type)
    {
      static const GTypeInfo table_info =
      {
        sizeof (GtkPandaTableClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_table_class_init,           
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaTable),
        0,
        (GInstanceInitFunc) gtk_panda_table_init
      };

      table_type = g_type_register_static( GTK_TYPE_TREE_VIEW,
                                           "GtkPandaTable",
                                           &table_info,
                                           0);
    }
  return table_type;
}

GtkWidget *
gtk_panda_table_new ()
{
  return (GtkWidget*)g_object_new(GTK_PANDA_TYPE_TABLE, NULL);
}

static void
start_editing(GtkTreeView * view)
{
  GtkTreePath *path;
  GtkTreeViewColumn *column;

  path = NULL;

  gtk_tree_view_get_cursor(view,&path,&column);
  if (path != NULL && column != NULL) { 
    gtk_tree_view_set_cursor(view,path,column,TRUE);
  }
  if (path != NULL) {
    gtk_tree_path_free(path);
  }
}

static void
move_left(GtkTreeView * view)
{
  GtkTreePath *path;
  GtkTreeViewColumn *column;
  GList *list;
  int i,n;

  path = NULL;
  list = gtk_tree_view_get_columns(view);

  gtk_tree_view_get_cursor(view,&path,&column);
  if (path != NULL && column != NULL) { 
    for(i=n=0;i<g_list_length(list);i++) {
      if (column == g_list_nth_data(list,i)) {
        n = i;
      }
    }
    if (n > 0) {
      column = GTK_TREE_VIEW_COLUMN(g_list_nth_data(list,n-1));
    } else {
      column = GTK_TREE_VIEW_COLUMN(g_list_nth_data(list,g_list_length(list)-1));
    }
    gtk_tree_view_set_cursor(view,path,column,TRUE);
  }
  g_list_free(list);
  if (path != NULL) {
    gtk_tree_path_free(path);
  }
}

static void
move_right(GtkTreeView * view)
{
  GtkTreePath *path;
  GtkTreeViewColumn *column;
  GList *list;
  int i,n;

  path = NULL;
  list = gtk_tree_view_get_columns(view);

  gtk_tree_view_get_cursor(view,&path,&column);
  if (path != NULL && column != NULL) { 
    for(i=n=0;i<g_list_length(list);i++) {
      if (column == g_list_nth_data(list,i)) {
        n = i;
      }
    }
    if (n < g_list_length(list) - 1) {
      column = GTK_TREE_VIEW_COLUMN(g_list_nth_data(list,n+1));
    } else {
      column = GTK_TREE_VIEW_COLUMN(g_list_nth_data(list,0));
    }
    gtk_tree_view_set_cursor(view,path,column,TRUE);
  }
  g_list_free(list);
  if (path != NULL) {
    gtk_tree_path_free(path);
  }
}

static gint
_move_to_start(gpointer data)
{
  GtkTreeView *view;
  GtkTreeViewColumn *column;
  GtkTreePath *start,*end,*path;

  view = GTK_TREE_VIEW(data);
  if (gtk_tree_view_get_visible_range(view, &start, &end)) {
    gtk_tree_view_get_cursor(view,&path,&column);
    if (path != NULL && column != NULL) {
      gtk_tree_view_set_cursor(view,start,column,FALSE);
      start_editing(view);
      gtk_tree_path_free(path);
    }
  }
  gtk_tree_path_free(start);
  gtk_tree_path_free(end);
  return FALSE;
}

static gint
_move_to_end(gpointer data)
{
  GtkTreeView *view;
  GtkTreeViewColumn *column;
  GtkTreePath *start,*end,*path;

  view = GTK_TREE_VIEW(data);
  if (gtk_tree_view_get_visible_range(view, &start, &end)) {
    gtk_tree_view_get_cursor(view,&path,&column);
    if (path != NULL && column != NULL) {
      if (gtk_tree_path_prev(end)) {
        gtk_tree_view_set_cursor(view,end,column,FALSE);
        start_editing(view);
      }
      gtk_tree_path_free(path);
    }
  }
  gtk_tree_path_free(start);
  gtk_tree_path_free(end);
  return FALSE;
}

static gboolean
gtk_panda_table_key_press(GtkWidget *widget,
  GdkEventKey *event)
{
  GtkPandaTable *table;

  table = GTK_PANDA_TABLE(widget);

  switch (event->keyval) {
  case GDK_Page_Up:
    g_idle_add(_move_to_start,widget);
    break;
  case GDK_Page_Down:
    g_idle_add(_move_to_start,widget);
    break;
  case GDK_Left:
  case GDK_KP_Left:
    move_left(GTK_TREE_VIEW(widget));
    return TRUE;
    break;
  case GDK_Right:
  case GDK_KP_Right:
    move_right(GTK_TREE_VIEW(widget));
    return TRUE;
    break;
  case GDK_Tab:
  case GDK_ISO_Left_Tab:
    if (event->state & GDK_CONTROL_MASK) {
    } else {
      if (event->state & GDK_SHIFT_MASK) {
        // SHIFT + TAB
        move_left(GTK_TREE_VIEW(widget));
      } else {
        // TAB
        move_right(GTK_TREE_VIEW(widget));
      }
      return TRUE;
    }
    break;
  }
  table->keyevents = g_list_append(table->keyevents,
    gdk_event_copy((const GdkEvent *)event));
  return GTK_WIDGET_CLASS(parent_class)->key_press_event(widget,event);
}

static void
cb_text_renderer_edited(GtkCellRendererText *renderer,
  gchar *pathstr,
  gchar *new_text,
  gpointer user_data)
{
  GtkPandaTable *table = GTK_PANDA_TABLE(user_data);
  guint column;
  gpointer data = g_object_get_data(G_OBJECT(renderer),"column_num");
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreePath *path;
  
  if (data == NULL) { return; }
  column = GPOINTER_TO_UINT(data);
  path = gtk_tree_path_new_from_string(pathstr);
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(table));
  if (gtk_tree_model_get_iter(model,&iter,path)) {
    gtk_list_store_set(
      GTK_LIST_STORE(model),
      &iter,
      column,
      new_text,
      -1);
  }
  gtk_tree_path_free(path);
  
#if 0
/* 編集中にマウスクリックでカーソル移動するとCELL_EDITED発生 */
/* CELL_EDITEDのコールバックでセル移動すると元のセルが破壊される */
  g_signal_emit(table, signals[CELL_EDITED],0,row,column,new_text);
#endif
}


static void
cb_text_renderer_edited_by_key(GtkCellRendererText *renderer,
  gchar *pathstr,
  gchar *new_text,
  gpointer user_data)
{
  GtkPandaTable *table = GTK_PANDA_TABLE(user_data);
  guint row,column;
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreePath *path;
  
  column = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(renderer),"column_num"));
  row = atoi(pathstr);
  path = gtk_tree_path_new_from_string(pathstr);
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(table));
  if (gtk_tree_model_get_iter(model,&iter,path)) {
    gtk_list_store_set(
      GTK_LIST_STORE(model),
      &iter,
      column,
      new_text,
      -1);
  }
  gtk_tree_path_free(path);
  
  g_signal_emit(table, signals[CELL_EDITED],0,row,column,new_text);
}

static void
parse_prop_types(GtkPandaTable *table)
{
  int i;
  gchar **splits;

  for (i = 0; i < GTK_PANDA_TABLE_MAX_COLS; i++) {
    table->renderer_types[i] = GTK_PANDA_TABLE_RENDERER_TEXT;
    table->model_types[i] = G_TYPE_STRING;
  }

  if (table->types == NULL) {
    return;
  }

  splits = g_strsplit(table->types,",",GTK_PANDA_TABLE_MAX_COLS);


  for(i = 0; splits[i] != NULL && i < table->columns; i++) {
    if (!strcmp(splits[i], "label")) {
      table->renderer_types[i] = GTK_PANDA_TABLE_RENDERER_LABEL;
      table->model_types[i] = G_TYPE_STRING;
    } else { /* text */
      table->renderer_types[i] = GTK_PANDA_TABLE_RENDERER_TEXT;
      table->model_types[i] = G_TYPE_STRING;
    }
  }
  g_strfreev(splits);
}

static void
apply_prop_types(GtkPandaTable *table)
{
  int i;
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;

  for(i = 0; i < table->columns; i++) {
    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_resizable(col,TRUE);
    gtk_tree_view_column_set_sizing(col,GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_title(col, "");
    g_object_set(G_OBJECT(col), 
      "alignment", 0.5,
      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(table), col);
    g_object_set_data(G_OBJECT(col),
      "column_num", GUINT_TO_POINTER(i));

    renderer = NULL;
    switch(table->renderer_types[i]) {
    case GTK_PANDA_TABLE_RENDERER_TEXT:
      renderer = panda_cell_renderer_text_new();
      gtk_tree_view_column_pack_start(col, renderer, TRUE);
      g_object_set(G_OBJECT(renderer),
        "editable",TRUE,
        NULL);
      g_signal_connect(G_OBJECT(renderer),"edited",
        G_CALLBACK(cb_text_renderer_edited),table);
      g_signal_connect(G_OBJECT(renderer),"edited-by-key",
        G_CALLBACK(cb_text_renderer_edited_by_key),table);
      break;
    case GTK_PANDA_TABLE_RENDERER_LABEL:
      renderer = panda_cell_renderer_text_new();
      gtk_tree_view_column_pack_start(col, renderer, TRUE);
      g_object_set(G_OBJECT(renderer),
        "editable",FALSE,
        NULL);
      break;
    }
    if (renderer != NULL) {
      g_object_set_data(G_OBJECT(renderer),"column_num",GUINT_TO_POINTER(i));
      g_object_set_data(G_OBJECT(renderer),"treeview",GTK_TREE_VIEW(table));
      gtk_tree_view_column_set_attributes(col, renderer, 
        "text",i,
        "foreground",table->columns+i,
        "cell-background",table->columns*2+i,
        NULL);
    }
  }
}

static void
apply_prop_titles(GtkPandaTable *table)
{
  int i, len;
  gchar **splits;
  GList *list;

  g_return_if_fail(table != NULL);

  if (table->titles == NULL) {
    return;
  }

  list = gtk_tree_view_get_columns(GTK_TREE_VIEW(table));
  len = g_list_length(list);
  for (i=0; i < len; i++) {
    gtk_tree_view_column_set_title(
      GTK_TREE_VIEW_COLUMN(g_list_nth_data(list,i)),"");
  }

  splits = g_strsplit(table->titles,",",GTK_PANDA_TABLE_MAX_COLS);
  for(i = 0; splits[i] != NULL && i < len; i++) {
    gtk_tree_view_column_set_title(
      GTK_TREE_VIEW_COLUMN(g_list_nth_data(list,i)),splits[i]);
  }
  g_strfreev(splits);
  g_list_free(list);
}

static void
apply_prop_widths(GtkPandaTable *table)
{
  int i, len;
  gchar **splits;
  GList *list;

  g_return_if_fail(table != NULL);

  if (table->titles == NULL) {
    return;
  }

  list = gtk_tree_view_get_columns(GTK_TREE_VIEW(table));
  len = g_list_length(list);

  splits = g_strsplit(table->widths,",",GTK_PANDA_TABLE_MAX_COLS);
  for(i = 0; splits[i] != NULL && i < len; i++) {
    gtk_tree_view_column_set_min_width(
      GTK_TREE_VIEW_COLUMN(g_list_nth_data(list,i)),
      atoi(splits[i]));
  }
  g_strfreev(splits);
  g_list_free(list);
}

static void
apply_prop_im_controls(GtkPandaTable *table)
{
  int i;
  gchar **splits;

  g_return_if_fail(table != NULL);

  if (table->im_controls == NULL) {
    return;
  }

  splits = g_strsplit(table->im_controls,",",GTK_PANDA_TABLE_MAX_COLS);
  for(i = 0; i<GTK_PANDA_TABLE_MAX_COLS; i++) {
      table->_im_controls[i] = FALSE;
  }
  for(i = 0; splits[i] != NULL; i++) {
    if (*splits[i] == 'T' || *splits[i] == 't') {
      table->_im_controls[i] = TRUE;
    }
  }
  g_strfreev(splits);
}

void
gtk_panda_table_set_columns (
  GtkPandaTable *table,
  gint new_columns) 
{
  GtkListStore *store;
  GList *list;
  int i;

  g_return_if_fail(table != NULL);
  if (new_columns <= 0) {
    new_columns = 1;
  }

  if (new_columns > GTK_PANDA_TABLE_MAX_COLS) {
    new_columns = GTK_PANDA_TABLE_MAX_COLS;
  }

  list = gtk_tree_view_get_columns(GTK_TREE_VIEW(table));
  for (i=0; i < g_list_length(list); i++) {
    gtk_tree_view_remove_column(GTK_TREE_VIEW(table),g_list_nth_data(list,i));
  }
  gtk_tree_view_set_model(GTK_TREE_VIEW(table), NULL);
  g_list_free(list);
  table->columns = new_columns;

  parse_prop_types(table);
  apply_prop_types(table);
  apply_prop_titles(table);

  store = gtk_list_store_newv(table->columns*4, table->model_types);
  gtk_tree_view_set_model(GTK_TREE_VIEW(table), GTK_TREE_MODEL(store));

  gtk_panda_table_set_rows(table,table->rows);
}

void
gtk_panda_table_set_rows(
  GtkPandaTable *table,
  gint new_rows)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  int i,j;
  GValue *value;

  g_return_if_fail(table != NULL);
  g_return_if_fail(GTK_IS_PANDA_TABLE(table));

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(table));
  if (gtk_tree_model_get_iter_first(model, &iter)) {
    gtk_list_store_clear(GTK_LIST_STORE(model));
  }
  
  table->rows = new_rows;

  for(i=0;i<new_rows;i++) {
    gtk_list_store_append (GTK_LIST_STORE(model), &iter);
    for(j = 0; j < table->columns; j++) {
      value = g_new0(GValue, 1);
      g_value_init(value, G_TYPE_STRING);
      g_value_set_string(value, "");
      gtk_list_store_set_value(GTK_LIST_STORE(model), &iter, j, value);

      /* fgcolor */
      value = g_new0(GValue, 1);
      g_value_init(value, G_TYPE_STRING);
      g_value_set_string(value, "black");
      gtk_list_store_set_value(GTK_LIST_STORE(model), &iter, table->columns + j, value);

      /* bgcolor */
      value = g_new0(GValue, 1);
      g_value_init(value, G_TYPE_STRING);
      g_value_set_string(value, "white");
      gtk_list_store_set_value(GTK_LIST_STORE(model), &iter, table->columns*2 + j, value);

      /* bgcolor_template */
      value = g_new0(GValue, 1);
      g_value_init(value, G_TYPE_STRING);
      g_value_set_string(value, "white");
      gtk_list_store_set_value(GTK_LIST_STORE(model), &iter, table->columns*3 + j, value);
    }
  }
}

void
gtk_panda_table_set_row(
  GtkPandaTable *table,
  int row,
  gchar **rdata)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar path[16];
  int i;

  g_return_if_fail(table != NULL);
  g_return_if_fail(GTK_IS_PANDA_TABLE(table));

  sprintf(path,"%d",row);
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(table));
  gtk_tree_model_get_iter_from_string(model,&iter,path);

  for(i=0;rdata[i] !=NULL && i<table->columns;i++) {
    gtk_list_store_set(GTK_LIST_STORE(model),&iter,i,rdata[i],-1);
  }
}

gchar*
color_darker(gchar *src)
{
  GdkColor color;
  const gchar *lightgray = "#F0F0F0";

  if (src == NULL) {
    return g_strdup(lightgray);
  }

  if (gdk_color_parse(src,&color)) {
    color.red =   (color.red-256*16)   > 0 ? (color.red-256*16)   : 0;
    color.green = (color.green-256*16) > 0 ? (color.green-256*16) : 0;
    color.blue =  (color.blue-256*16)  > 0 ? (color.blue-256*16)  : 0;
    return g_strdup_printf("#%02X%02X%02X",
             (guint)(color.red/256.0),
             (guint)(color.green/256.0),
             (guint)(color.blue/256.0)
             );
  } else {
    return g_strdup(lightgray);
  }
}

void
gtk_panda_table_set_bgcolor(
  GtkPandaTable *table,
  int row,
  gchar **rdata)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar path[16];
  int i;

  g_return_if_fail(table != NULL);
  g_return_if_fail(GTK_IS_PANDA_TABLE(table));

  sprintf(path,"%d",row);
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(table));
  gtk_tree_model_get_iter_from_string(model,&iter,path);

  for(i=0;rdata[i] !=NULL && i<table->columns;i++) {
    if (strlen(rdata[i])==0) {
      gtk_list_store_set(GTK_LIST_STORE(model),&iter,table->columns*2+i,NULL,-1);
      gtk_list_store_set(GTK_LIST_STORE(model),&iter,table->columns*3+i,NULL,-1);
    } else {
      gtk_list_store_set(GTK_LIST_STORE(model),&iter,table->columns*2+i,rdata[i],-1);
      gtk_list_store_set(GTK_LIST_STORE(model),&iter,table->columns*3+i,rdata[i],-1);
    }
  }
}

void
gtk_panda_table_set_fgcolor(
  GtkPandaTable *table,
  int row,
  gchar **rdata)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar path[16];
  int i;

  g_return_if_fail(table != NULL);
  g_return_if_fail(GTK_IS_PANDA_TABLE(table));

  sprintf(path,"%d",row);
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(table));
  gtk_tree_model_get_iter_from_string(model,&iter,path);

  for(i=0;rdata[i] !=NULL && i<table->columns;i++) {
    if (strlen(rdata[i])==0) {
      gtk_list_store_set(GTK_LIST_STORE(model),&iter,table->columns+i,NULL,-1);
    } else {
      gtk_list_store_set(GTK_LIST_STORE(model),&iter,table->columns+i,rdata[i],-1);
    }
  }
}

void
gtk_panda_table_set_types(
  GtkPandaTable *table,
  const gchar *types)
{
  g_return_if_fail(table != NULL);
  g_return_if_fail(types != NULL);

  if (table->types != NULL) {
    g_free(table->types);
  }
  table->types = g_strdup(types);
  gtk_panda_table_set_columns(table,table->columns);
}

void
gtk_panda_table_set_titles(
  GtkPandaTable *table,
  const gchar *titles)
{
  g_return_if_fail(table != NULL);
  g_return_if_fail(titles != NULL);

  if (table->titles != NULL) {
    g_free(table->titles);
  }
  table->titles = g_strdup(titles);
  apply_prop_titles(table);
}

void
gtk_panda_table_set_column_widths(
  GtkPandaTable *table,
  const gchar *widths)
{
  g_return_if_fail(table != NULL);
  g_return_if_fail(widths != NULL);

  if (table->widths != NULL) {
    g_free(table->widths);
  }
  table->widths = g_strdup(widths);
  apply_prop_widths(table);
}

void
gtk_panda_table_set_im_controls(
  GtkPandaTable *table,
  const gchar *ics)
{
  g_return_if_fail(table != NULL);
  g_return_if_fail(ics != NULL);

  if (table->im_controls != NULL) {
    g_free(table->im_controls);
  }
  table->im_controls = g_strdup(ics);
  apply_prop_im_controls(table);
}

gint
gtk_panda_table_get_n_rows(
  GtkPandaTable *table)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gint nrows;

  g_return_val_if_fail (table != NULL, 0);
  g_return_val_if_fail (GTK_IS_PANDA_TABLE (table), 0);

  nrows = 0; 
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(table));
  if (gtk_tree_model_get_iter_first(model, &iter)) {
    nrows = 1;
    while(gtk_tree_model_iter_next(model, &iter)) {
      nrows++;
    }
  }
  return nrows;
}

gint
gtk_panda_table_get_column_type(GtkPandaTable *table,
  gint col)
{
  g_return_val_if_fail (table != NULL,0);
  g_return_val_if_fail (GTK_IS_PANDA_TABLE (table),0);
  g_return_val_if_fail (col < GTK_PANDA_TABLE_MAX_COLS,0);

  return table->renderer_types[col];
}

static gint
_start_editing(gpointer data)
{
  start_editing(GTK_TREE_VIEW(data));
  return FALSE;
}

void 
gtk_panda_table_moveto (
  GtkPandaTable *table,
  gint      row,
  gint      column,
  gboolean  use_align,
  gfloat    row_align,
  gfloat    col_align)
{
  GtkTreePath *path;
  GtkTreeViewColumn *col;

  g_return_if_fail (table != NULL);
  g_return_if_fail (GTK_IS_PANDA_TABLE (table));

  path = gtk_tree_path_new_from_indices(row, -1);
  col = gtk_tree_view_get_column(GTK_TREE_VIEW(table), column);

  gtk_tree_view_scroll_to_cell(
    GTK_TREE_VIEW(table), path, col, use_align, row_align, col_align);
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(table),path,col,FALSE);
  g_idle_add(_start_editing,table);
  gtk_tree_path_free(path);
}

void 
gtk_panda_table_stay (
  GtkPandaTable *table)
{
  g_return_if_fail (table != NULL);
  g_return_if_fail (GTK_IS_PANDA_TABLE (table));

  g_idle_add(_start_editing,table);
}

gboolean
gtk_panda_table_get_im_control(
  GtkPandaTable *table,
  gint column)
{
  return table->_im_controls[column];
}

static void 
gtk_panda_table_set_property (GObject *object,
  guint prop_id,
  const GValue *value,
  GParamSpec *pspec)
{
  GtkPandaTable *table;

  g_return_if_fail(GTK_IS_PANDA_TABLE(object));
  table = GTK_PANDA_TABLE (object);

  switch (prop_id)
    {
    case PROP_ROWS:
      gtk_panda_table_set_rows(table,g_value_get_int(value));
      break;
    case PROP_COLUMNS:
      gtk_panda_table_set_columns(table,g_value_get_int(value));
      break;
    case PROP_TYPES:
      gtk_panda_table_set_types(table,g_value_get_string(value));
      break;
    case PROP_TITLES:
      gtk_panda_table_set_titles(table,g_value_get_string(value));
      break;
    case PROP_WIDTHS:
      gtk_panda_table_set_column_widths(table,g_value_get_string(value));
      break;
    case PROP_IM_CONTROLS:
      gtk_panda_table_set_im_controls(table,g_value_get_string(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void 
gtk_panda_table_get_property (GObject         *object,
  guint            prop_id,
  GValue          *value,
  GParamSpec      *pspec)
{
  GtkPandaTable *table;

  g_return_if_fail(GTK_IS_PANDA_TABLE(object));
  table = GTK_PANDA_TABLE (object);

  switch (prop_id)
    {
    case PROP_ROWS:
      g_value_set_int (value, table->rows);
      break;
    case PROP_COLUMNS:
      g_value_set_int (value, table->columns);
      break;
    case PROP_TYPES:
      g_value_set_string (value, g_strdup(table->types));
      break;
    case PROP_TITLES:
      g_value_set_string (value, g_strdup(table->titles));
      break;
    case PROP_WIDTHS:
      g_value_set_string (value, g_strdup(table->widths));
      break;
    case PROP_IM_CONTROLS:
      g_value_set_string (value, g_strdup(table->im_controls));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static gboolean
cb_button_release_event(GtkWidget *widget,
  GdkEvent *event,
  gpointer data)
{
  GtkTreeView *view = GTK_TREE_VIEW(widget);
  GtkTreePath *path;
  GtkTreeViewColumn *column;

  gtk_tree_view_get_cursor(view,&path,&column);
  if (path != NULL && column != NULL) {
    gtk_tree_view_set_cursor(view,path,column,TRUE);
  }

  if (path != NULL) {
    gtk_tree_path_free(path);
  }
  return FALSE;
}

static void
cb_cursor_changed(GtkTreeView *view,
  gpointer data)
{
  GtkTreePath *path;
  GtkTreeViewColumn *column;
  gint row,col,*indices;

  col = 0;
  row = 0;

  gtk_tree_view_get_cursor(view,&path,&column);
  if (path != NULL && column != NULL) {
    col = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(column),"column_num"));
    indices = gtk_tree_path_get_indices(path);
    if (indices != NULL) {
      row = indices[0];
    }
  }
  if (path != NULL) {
    gtk_tree_path_free(path);
  }

  g_object_set_data(G_OBJECT(view),"changed_row",
    GINT_TO_POINTER(row));
  g_object_set_data(G_OBJECT(view),"changed_column",
    GINT_TO_POINTER(col));
  g_object_set_data(G_OBJECT(view),"changed_value",
    NULL);
}
