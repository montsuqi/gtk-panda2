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

#include "pandamarshal.h"
#include "gtkpandaintl.h"
#include "gtkpandatable.h"
#include "gtkpandaentry.h"

enum {
PROP_0,
PROP_ROWS,
PROP_COLUMNS,
PROP_TYPES,
PROP_TITLES,
PROP_WIDTHS
};

enum
{
  CELL_EDITED,
  LAST_SIGNAL 
}; 

static GtkVBoxClass *parent_class = NULL;
static guint signals [LAST_SIGNAL] = { 0 };

static void gtk_panda_table_class_init (GtkPandaTableClass *klass);
static void gtk_panda_table_init (GtkPandaTable *table);

static void gtk_panda_table_set_property(GObject *object,
  guint        prop_id,
  const GValue *value,
  GParamSpec   *pspec);
static void gtk_panda_table_get_property(GObject *object,
  guint      prop_id,
  GValue     *value,
  GParamSpec *pspec);
static void cb_size_allocate(GtkWidget *widget,
  GdkRectangle *rect,
  gpointer     p);
static void cb_set_focus_child(GtkContainer *con,
  GtkWidget *child,
  gpointer data);

static void
gtk_panda_table_class_init ( GtkPandaTableClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_ref(GTK_TYPE_VBOX);

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
}

static void
gtk_panda_table_init (GtkPandaTable *table)
{
  GtkAdjustment *hadj,*vadj;

  table->xim_enabled = FALSE;
  table->types = g_strdup("");
  table->titles = g_strdup("");
  table->widths = g_strdup("");
  table->labels = NULL;
  table->entries = NULL;
  table->hseparators = NULL;
  table->cseparators = NULL;
  table->rows = 1;
  table->cols = 1;
  table->row_height = 30;
  table->inset = 1;

  if (getenv("MONSIA_EDITING") != NULL) {
    return;
  }

  hadj = GTK_ADJUSTMENT(gtk_adjustment_new(0,0,100,1,1,1));
  vadj = GTK_ADJUSTMENT(gtk_adjustment_new(0,0,100,1,1,1));

  /*header*/
  table->hscroll = gtk_scrolled_window_new(hadj,NULL);
  gtk_widget_set_can_focus(table->hscroll,FALSE);
  table->hbox = gtk_hbox_new(FALSE,0);
  gtk_widget_set_can_focus(table->hbox,FALSE);

  gtk_scrolled_window_add_with_viewport(
    GTK_SCROLLED_WINDOW(table->hscroll),
    table->hbox);
  gtk_scrolled_window_set_policy(
    GTK_SCROLLED_WINDOW(table->hscroll),
    GTK_POLICY_NEVER,GTK_POLICY_NEVER);

  /*contents*/
  table->cscroll = gtk_scrolled_window_new(hadj,vadj);
  table->fixed = gtk_fixed_new();
  gtk_scrolled_window_add_with_viewport(
    GTK_SCROLLED_WINDOW(table->cscroll),
    table->fixed);

  gtk_box_set_homogeneous(GTK_BOX(table),FALSE);
  gtk_box_set_spacing(GTK_BOX(table),0);
  gtk_box_pack_start(GTK_BOX(table),table->hscroll,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(table),table->cscroll,TRUE,TRUE,0);

  g_signal_connect(G_OBJECT(table),
    "size-allocate",
    G_CALLBACK(cb_size_allocate),NULL);
  g_signal_connect(G_OBJECT(table->fixed),
    "set-focus-child",
    G_CALLBACK(cb_set_focus_child),table);
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

      table_type = g_type_register_static( GTK_TYPE_VBOX,
        "GtkPandaTable", &table_info, 0);
    }
  return table_type;
}

static void
cb_size_allocate(GtkWidget *widget,
  GdkRectangle *rect,
  gpointer data)
{
  GtkPandaTable *table;
  GtkAdjustment *adj;
  gdouble page_size;

  table = GTK_PANDA_TABLE(widget);

  gtk_widget_set_size_request(table->hscroll,
    rect->width,table->row_height+table->inset*2);

  page_size = rect->width;
  adj = gtk_scrolled_window_get_hadjustment(
    GTK_SCROLLED_WINDOW(table->cscroll));
  gtk_adjustment_set_page_size(adj,page_size);
  gtk_adjustment_set_page_increment(adj,page_size);
  gtk_adjustment_set_step_increment(adj,page_size/10.0);

  page_size = rect->height - table->row_height - table->inset *2;
  adj = gtk_scrolled_window_get_vadjustment(
    GTK_SCROLLED_WINDOW(table->cscroll));
  gtk_adjustment_set_page_size(adj,page_size);
  gtk_adjustment_set_page_increment(adj,page_size);
  gtk_adjustment_set_step_increment(adj,page_size/10.0);
}

static void
cb_set_focus_child(GtkContainer *con,
  GtkWidget *child,
  gpointer data)
{
  GtkPandaTable *table;
  GtkScrolledWindow *scroll;
  GtkAdjustment *adj;
  gint x,y,w,h;
  gdouble value,page_size;

  if (!GTK_IS_ENTRY(child)) {
    return;
  }
  table = GTK_PANDA_TABLE(data);
  scroll = GTK_SCROLLED_WINDOW(table->cscroll);
  gtk_container_child_get(GTK_CONTAINER(table->fixed),
    child,
    "x",&x,
    "y",&y,
    NULL);
  gtk_widget_get_size_request(child,&w,&h);
  /*row*/
  adj = gtk_scrolled_window_get_vadjustment(scroll);
  value = gtk_adjustment_get_value(adj);
  page_size = gtk_adjustment_get_page_size(adj);

  if ((y+h) > (value + page_size)) {
    gtk_adjustment_set_value(adj,
      y - page_size + h + table->inset * 2);
  }
  if (y < value) {
    gtk_adjustment_set_value(adj,y);
  }

  /*col*/
  adj = gtk_scrolled_window_get_hadjustment(scroll);
  value = gtk_adjustment_get_value(adj);
  page_size = gtk_adjustment_get_page_size(adj);

  if ((x+w) > (value + page_size)) {
    gtk_adjustment_set_value(adj,
      x - page_size + w + table->inset * 2);
  }
  if (x < value) {
    gtk_adjustment_set_value(adj,x);
  }
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
      table->rows = g_value_get_int(value);
      break;
    case PROP_COLUMNS:
      table->cols = g_value_get_int(value);
      break;
    case PROP_TYPES:
      if (table->types != NULL) {
        g_free(table->types);
      }
      table->types = g_strdup(g_value_get_string(value));
      break;
    case PROP_TITLES:
      if (table->titles != NULL) {
        g_free(table->titles);
      }
      table->titles = g_strdup(g_value_get_string(value));
      break;
    case PROP_WIDTHS:
      if (table->widths != NULL) {
        g_free(table->widths);
      }
      table->widths = g_strdup(g_value_get_string(value));
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
      g_value_set_int (value, table->cols);
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
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

GtkWidget *
gtk_panda_table_new ()
{
  return (GtkWidget*)g_object_new(GTK_PANDA_TYPE_TABLE, NULL);
}

static gboolean
cb_entry_activate(
  GtkEntry *entry,
  gpointer data)
{
  GtkPandaTable *table;
  const gchar *text;
  int row,col;
  
  table = GTK_PANDA_TABLE(data);
  text = gtk_entry_get_text(entry);
  row = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(entry),
    "panda-table-row"));
  col = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(entry),
    "panda-table-column"));
  g_signal_emit(table,signals[CELL_EDITED],0,row,col,text);
  return FALSE;
}

void
gtk_panda_table_build(
  GtkPandaTable *table)
{
  int i,j,w,h;
  GtkWidget *child;
  GtkAdjustment *adj;
  GList *titles,*types,*widths,*xs;
  gchar **strtitles,**strtypes,**strwidths,*type;

  g_return_if_fail(GTK_IS_PANDA_TABLE(table));

  if (table->rows <= 0) {
    table->rows = 1;
  }
  if (table->rows > GTK_PANDA_TABLE_MAX_ROWS) {
    table->rows = GTK_PANDA_TABLE_MAX_ROWS;
  }
  if (table->cols <= 0) {
    table->cols = 1;
  }
  if (table->cols > GTK_PANDA_TABLE_MAX_COLS) {
    table->cols = GTK_PANDA_TABLE_MAX_COLS;
  }

  /*free old parts*/
  if (table->labels != NULL) {
    for(i=0;i<g_list_length(table->labels);i++) {
      gtk_container_remove(GTK_CONTAINER(table->hbox),
        GTK_WIDGET(g_list_nth_data(table->labels,i)));
      gtk_widget_destroy(GTK_WIDGET(g_list_nth_data(table->labels,i)));
    }
  }
  g_list_free(table->labels);
  table->labels = NULL;

  if (table->entries != NULL) {
    for(i=0;i<g_list_length(table->entries);i++) {
      gtk_container_remove(GTK_CONTAINER(table->fixed),
        GTK_WIDGET(g_list_nth_data(table->entries,i)));
      gtk_widget_destroy(GTK_WIDGET(g_list_nth_data(table->entries,i)));
    }
  }
  g_list_free(table->entries);
  table->entries = NULL;

  if (table->hseparators != NULL) {
    for(i=0;i<g_list_length(table->hseparators);i++) {
      gtk_container_remove(GTK_CONTAINER(table->hbox),
        GTK_WIDGET(g_list_nth_data(table->hseparators,i)));
      gtk_widget_destroy(GTK_WIDGET(g_list_nth_data(table->hseparators,i)));
    }
  }
  g_list_free(table->hseparators);
  table->hseparators = NULL;

  if (table->cseparators != NULL) {
    for(i=0;i<g_list_length(table->cseparators);i++) {
      gtk_container_remove(GTK_CONTAINER(table->fixed),
        GTK_WIDGET(g_list_nth_data(table->cseparators,i)));
      gtk_widget_destroy(GTK_WIDGET(g_list_nth_data(table->cseparators,i)));
    }
  }
  g_list_free(table->cseparators);
  table->cseparators = NULL;

  /*build parts*/
  titles = NULL;
  strtitles = g_strsplit(table->titles,",",GTK_PANDA_TABLE_MAX_COLS);
  for(j=0;strtitles[j]!=NULL;j++);
  for(i=0;i<table->cols;i++) {
    if (i<j) {
      titles = g_list_append(titles,strtitles[i]);
    } else {
      titles = g_list_append(titles,"");
    }
  }
  types = NULL;
  strtypes = g_strsplit(table->types,",",GTK_PANDA_TABLE_MAX_COLS);
  for(j=0;strtypes[j]!=NULL;j++);
  for(i=0;i<table->cols;i++) {
    if (i<j) {
      types = g_list_append(types,strtypes[i]);
    } else {
      types = g_list_append(types,"text");
    }
  }
  widths = NULL;
  strwidths = g_strsplit(table->widths,",",GTK_PANDA_TABLE_MAX_COLS);
  for(j=0;strwidths[j]!=NULL;j++);
  for(i=0;i<table->cols;i++) {
    if (i<j) {
      widths = g_list_append(widths,GINT_TO_POINTER(atoi(strwidths[i])));
    } else {
      widths = g_list_append(widths,GINT_TO_POINTER(100));
    }
  }

  w = 0;
  xs = NULL;
  for(i=0;i<table->cols;i++) {
    xs = g_list_append(xs,GINT_TO_POINTER(w));
    w += table->inset * 2 + GPOINTER_TO_INT(g_list_nth_data(widths,i));
  }
  h = table->row_height*(table->rows) + table->inset * 2;

  adj = gtk_scrolled_window_get_hadjustment(
    GTK_SCROLLED_WINDOW(table->cscroll));
  gtk_adjustment_set_lower(adj,0);
  gtk_adjustment_set_upper(adj,w);
  adj = gtk_scrolled_window_get_vadjustment(
    GTK_SCROLLED_WINDOW(table->cscroll));
  gtk_adjustment_set_lower(adj,0);
  gtk_adjustment_set_upper(adj,h);

  /*label*/
  for(i=0;i<table->cols;i++) {
    child = gtk_label_new((char*)g_list_nth_data(titles,i));
    table->labels = g_list_append(table->labels,child);
    gtk_widget_set_size_request(child,
      GPOINTER_TO_INT(g_list_nth_data(widths,i)),
      table->row_height);
    gtk_widget_set_can_focus(child,FALSE);
    gtk_misc_set_alignment(GTK_MISC(child),0.5,0.5);
    gtk_box_pack_start(GTK_BOX(table->hbox),child,FALSE,FALSE,0);
    /*separators*/
    child = gtk_vseparator_new();
    gtk_widget_set_can_focus(child,FALSE);
    table->hseparators = g_list_append(table->hseparators,child);
    gtk_widget_set_size_request(child,
      table->inset*2,
      table->row_height+table->inset*2);
    gtk_box_pack_start(GTK_BOX(table->hbox),child,FALSE,FALSE,0);
  }

  /*entry*/
  for(i=0;i<table->rows;i++) {
    for(j=0;j<table->cols;j++) {
      child = gtk_panda_entry_new();
      gtk_panda_entry_set_input_mode(GTK_PANDA_ENTRY(child),
        GTK_PANDA_ENTRY_XIM);
      gtk_panda_entry_set_xim_enabled(GTK_PANDA_ENTRY(child),FALSE);
      gtk_widget_set_size_request(child,
        GPOINTER_TO_INT(g_list_nth_data(widths,j)),
        table->row_height);
      table->entries = g_list_append(table->entries,child);
      type = (gchar*)g_list_nth_data(types,j);
      if (*type == 'l') {
         // label
         g_object_set(G_OBJECT(child),"editable",FALSE,NULL);
      } else {
        // do nothing -> text
      }
      g_object_set(G_OBJECT(child),"has-frame",FALSE,NULL);
      g_object_set_data(G_OBJECT(child),"panda-table-row",
        GINT_TO_POINTER(i));
      g_object_set_data(G_OBJECT(child),"panda-table-column",
        GINT_TO_POINTER(j));

      g_signal_connect(G_OBJECT(child),"activate",
        G_CALLBACK(cb_entry_activate),table);

      gtk_fixed_put(GTK_FIXED(table->fixed),
        child,
        GPOINTER_TO_INT(g_list_nth_data(xs,j))+table->inset,
        (table->row_height * i) + table->inset);

      /*separators*/
      child = gtk_vseparator_new();
      gtk_widget_set_can_focus(child,FALSE);
      table->cseparators = g_list_append(table->cseparators,child);
      gtk_widget_set_size_request(child,
        table->inset,
        (table->row_height+table->inset*2) * table->rows);
      gtk_fixed_put(GTK_FIXED(table->fixed),
        child,
        GPOINTER_TO_INT(g_list_nth_data(xs,j)),
        0);
    }
  }
  g_list_free(titles);
  g_list_free(types);
  g_list_free(widths);
  g_list_free(xs);
  g_strfreev(strtitles);
  g_strfreev(strtypes);
  g_strfreev(strwidths);
}

void 
gtk_panda_table_set_cell_text(
  GtkPandaTable *table,
  gint row,
  gint col,
  const gchar *text)
{
  GtkEntry *entry;

  g_return_if_fail(GTK_IS_PANDA_TABLE(table));
  if (0 <= row && row <= (table->rows-1)) {
  } else {
    return;
  }
  if (0 <= col && col <= (table->cols-1)) {
  } else {
    return;
  }

  entry = GTK_ENTRY(g_list_nth_data(table->entries,
    (table->cols)*row+col));
  gtk_entry_set_text(entry,text);
}

const gchar* 
gtk_panda_table_get_cell_text(
  GtkPandaTable *table,
  gint row,
  gint col)
{
  GtkEntry *entry;

  g_return_val_if_fail(GTK_IS_PANDA_TABLE(table),"");
  if (0 <= row && row <= (table->rows-1)) {
  } else {
    return"";
  }
  if (0 <= col && col <= (table->cols-1)) {
  } else {
    return"";
  }

  entry = GTK_ENTRY(g_list_nth_data(table->entries,
    (table->cols)*row+col));
  return gtk_entry_get_text(entry);
}

void 
gtk_panda_table_set_cell_color(
  GtkPandaTable *table,
  gint row,
  gint col,
  const gchar *fgcolor,
  const gchar *bgcolor)
{
  GdkColor color;
  GtkWidget *widget;

  g_return_if_fail(GTK_IS_PANDA_TABLE(table));
  if (0 <= row && row <= (table->rows-1)) {
  } else {
    return;
  }
  if (0 <= col && col <= (table->cols-1)) {
  } else {
    return;
  }

  widget = GTK_WIDGET(g_list_nth_data(table->entries,
    (table->cols)*row+col));
  if (gdk_color_parse(fgcolor,&color)) {
    gtk_widget_modify_text(widget,GTK_STATE_NORMAL,&color);
    gtk_widget_modify_text(widget,GTK_STATE_ACTIVE,&color);
    gtk_widget_modify_text(widget,GTK_STATE_SELECTED,&color);
  }
  if (gdk_color_parse(bgcolor,&color)) {
    gtk_widget_modify_base(widget,GTK_STATE_NORMAL,&color);
    gtk_widget_modify_base(widget,GTK_STATE_ACTIVE,&color);
    gtk_widget_modify_base(widget,GTK_STATE_SELECTED,&color);
  }
}

void 
gtk_panda_table_moveto (
  GtkPandaTable *table,
  gint      row,
  gint      col,
  gfloat    row_align,
  gfloat    col_align)
{
  GtkWidget *child;
  GtkScrolledWindow *scroll;
  GtkAdjustment *adj;
  gint x,y,w,h;
  gdouble value,page_size;

  g_return_if_fail(GTK_IS_PANDA_TABLE(table));

  if (0 <= row && row <= (table->rows-1)) {
  } else {
    return;
  }
  if (0 <= col && col <= (table->cols-1)) {
  } else {
    return;
  }

  child = GTK_WIDGET(g_list_nth_data(table->entries,
    (table->cols)*row+col));

  if (gtk_widget_has_focus(child)) {
    return;
  }

  scroll = GTK_SCROLLED_WINDOW(table->cscroll);
  gtk_container_child_get(GTK_CONTAINER(table->fixed),
    child,
    "x",&x,
    "y",&y,
    NULL);
  gtk_widget_get_size_request(child,&w,&h);

  /*row*/
  adj = gtk_scrolled_window_get_vadjustment(scroll);
  page_size = gtk_adjustment_get_page_size(adj);
  value = y - page_size * row_align + h + table->inset * 2;
  gtk_adjustment_set_value(adj,value);

  /*col*/
  adj = gtk_scrolled_window_get_hadjustment(scroll);
  page_size = gtk_adjustment_get_page_size(adj);
  value = x - page_size * col_align + w + table->inset * 2;
  gtk_adjustment_set_value(adj,value);

  gtk_widget_grab_focus(child);
}

void
gtk_panda_table_set_xim_enabled(
  GtkPandaTable *table,
  gboolean enabled)
{
  int i;
  g_return_if_fail(GTK_IS_PANDA_TABLE(table));

  table->xim_enabled = enabled;
  for(i=0;i<g_list_length(table->entries);i++) {
    gtk_panda_entry_set_input_mode(
      GTK_PANDA_ENTRY(g_list_nth_data(table->entries,i)),
      GTK_PANDA_ENTRY_XIM);
    gtk_panda_entry_set_xim_enabled(
      GTK_PANDA_ENTRY(g_list_nth_data(table->entries,i)),
      enabled);
  }
}

gboolean
gtk_panda_table_get_xim_enabled(
  GtkPandaTable *table)
{
  g_return_val_if_fail(GTK_IS_PANDA_TABLE(table),FALSE);
  return table->xim_enabled;
}
