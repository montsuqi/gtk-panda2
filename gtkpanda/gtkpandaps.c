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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkbindings.h>
#include <gtk/gtkmarshal.h>
#include <gdk/gdkkeysyms.h>

#include "gtkpandaintl.h"
#include "gtkpandaps.h"

static void gtk_panda_ps_class_init    (GtkPandaPSClass *klass);
static void gtk_panda_ps_init          (GtkPandaPS      *pps);
//static void gtk_panda_ps_destroy      (GtkObject *object);
static void gtk_panda_ps_hide	       (GtkWidget *widget);

static GtkContainerClass *parent_class = NULL;

#define SCALE_ZOOM_FIT_PAGE   (-1.0)
#define SCALE_ZOOM_FIT_WIDTH  (-2.0) 
#define SCALE_ZOOM_SEPARATOR  (-3.0)

static const
struct
{
  gchar *name;
  float level;
} zoom_levels[] =
{
  { N_("Fit Page"),       SCALE_ZOOM_FIT_PAGE  },         
  { N_("Fit Page Width"), SCALE_ZOOM_FIT_WIDTH },
  { NULL,                 SCALE_ZOOM_SEPARATOR },         
  { N_("50%"), 0.7071067811 },
  { N_("75%"), 0.8408964152 },
  { N_("100%"), 1.0 },
  { N_("125%"), 1.1892071149 },
  { N_("150%"), 1.4142135623 },
  { N_("175%"), 1.6817928304 },
  { N_("200%"), 2.0 },
  { N_("300%"), 2.8284271247 },
  { N_("400%"), 4.0 }
};
static const guint n_zoom_levels = G_N_ELEMENTS (zoom_levels);
#define ZOOM_MINIMAL    (ZOOM_SEPARATOR)
#define ZOOM_MAXIMAL    (zoom_levels[n_zoom_levels - 1].level)

enum
{
  COL_TEXT,
  COL_IS_SEP
};

enum
{
  ZOOM_FIT_PAGE,
  ZOOM_FIT_WIDTH,
  ZOOM_IN,
  ZOOM_OUT,
  LAST_SIGNAL
};

static guint ps_signals [LAST_SIGNAL] = { 0 };

void gtk_panda_ps_zoom_fit_page(GtkPandaPS *ps);
void gtk_panda_ps_zoom_fit_width(GtkPandaPS *ps);
void gtk_panda_ps_zoom_in(GtkPandaPS *ps);
void gtk_panda_ps_zoom_out(GtkPandaPS *ps);
static void combo_changed_cb (GtkComboBox *combo, GtkWidget *pps);

GType
gtk_panda_ps_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      static const GTypeInfo info =
      {
        sizeof (GtkPandaPSClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_ps_class_init,
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaPS),
        0,
        (GInstanceInitFunc) gtk_panda_ps_init
      };

      type = g_type_register_static( GTK_TYPE_VBOX,
                                     "GtkPandaPS",
                                     &info,
                                     0);
    }

  return type;
}

static void
gtk_panda_ps_class_init (GtkPandaPSClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GtkObjectClass *object_class;
  GtkWidgetClass *widget_class;
  GtkBindingSet *binding_set;

  object_class = (GtkObjectClass *) klass;
  widget_class = (GtkWidgetClass *) klass;
  parent_class = gtk_type_class (GTK_TYPE_CONTAINER);

  //object_class->destroy = gtk_panda_ps_destroy;

  widget_class->hide = gtk_panda_ps_hide;

  klass->zoom_fit_page = gtk_panda_ps_zoom_fit_page;
  klass->zoom_fit_width = gtk_panda_ps_zoom_fit_width;
  klass->zoom_in = gtk_panda_ps_zoom_in;
  klass->zoom_out = gtk_panda_ps_zoom_out;

  ps_signals[ZOOM_FIT_PAGE] =
  g_signal_new ("zoom_fit_page",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPSClass, zoom_fit_page),
        NULL, NULL,
        gtk_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  ps_signals[ZOOM_FIT_WIDTH] =
  g_signal_new ("zoom_fit_width",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPSClass, zoom_fit_width),
        NULL, NULL,
        gtk_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  ps_signals[ZOOM_IN] =
  g_signal_new ("zoom_in",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPSClass, zoom_in),
        NULL, NULL,
        gtk_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  ps_signals[ZOOM_OUT] =
  g_signal_new ("zoom_out",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPSClass, zoom_out),
        NULL, NULL,
        gtk_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  binding_set = gtk_binding_set_by_class (klass);
  gtk_binding_entry_add_signal (binding_set, GDK_g, GDK_CONTROL_MASK,
    "zoom_fit_page", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_f, GDK_CONTROL_MASK,
    "zoom_fit_width", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_plus, GDK_CONTROL_MASK,
    "zoom_in", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_minus, GDK_CONTROL_MASK,
    "zoom_out", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_F5, GDK_SHIFT_MASK,
    "zoom_fit_page", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_F6, GDK_SHIFT_MASK,
    "zoom_fit_width", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_F7, GDK_SHIFT_MASK,
    "zoom_in", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_F8, GDK_SHIFT_MASK,
    "zoom_out", 0);
}

#if 0
static void
gtk_panda_ps_destroy (GtkObject *object)
{
  if (GTK_PANDA_PS (object)->gs)
    gtk_widget_destroy (GTK_PANDA_PS (object)->gs);
}
#endif

static void
gtk_panda_ps_hide (GtkWidget *widget)
{
  if (GTK_PANDA_PS (widget)->gs)
    gtk_gs_disable_interpreter (GTK_GS (GTK_PANDA_PS (widget)->gs));
}

static void
button_press_callback (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  GtkPandaPS *pps = data;
  gint button = event->button;
  gint x = 0;
  gint y = 0;

  x += event->x;
  y += event->y;
  switch (button)
    {
    case 1:
      if (!pps->pan)
	{
	  static GdkCursor *pan_cursor = NULL;
	  gint wx = 0, wy = 0;

	  gdk_window_get_pointer (widget->window, &wx, &wy, NULL);

	  pps->pan = TRUE;
	  if (pan_cursor == NULL)
	    pan_cursor = gdk_cursor_new (GDK_FLEUR);

	  gtk_grab_add (widget);
	  gdk_pointer_grab (widget->window, FALSE,
			    GDK_POINTER_MOTION_MASK |
			    GDK_BUTTON_RELEASE_MASK, NULL,
			    pan_cursor, GDK_CURRENT_TIME);
	  pps->prev_x = wx;
	  pps->prev_y = wy;
	  gtk_gs_start_scroll (GTK_GS (pps->gs));
	}
      break;
    case 2:
    case 3:
      break;
      /* Support for wheel mice */
    case 4:
      if (event->state & GDK_CONTROL_MASK)
	gtk_gs_scroll (GTK_GS (pps->gs), -64, 0);
      else
	gtk_gs_scroll (GTK_GS (pps->gs), 0, -64);
      break;
    case 5:
      if (event->state & GDK_CONTROL_MASK)
	gtk_gs_scroll (GTK_GS (pps->gs), 64, 0);
      else
	gtk_gs_scroll (GTK_GS (pps->gs), 0, 64);
      break;
    default:
      break;
    }
}

static void
button_release_callback (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  GtkPandaPS *pps = data;

  switch (event->button)
    {
    case 1:
      if (pps->pan)
	{
	  pps->pan = FALSE;
	  gdk_pointer_ungrab (GDK_CURRENT_TIME);
	  gtk_grab_remove (widget);
	  gtk_gs_end_scroll (GTK_GS (pps->gs));
	}
      break;
    default:
      break;
    }
}

static void
motion_callback (GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
  GtkPandaPS *pps = data;

  if (pps->pan)
    {
      gtk_gs_scroll (GTK_GS (pps->gs),
		     -event->x + pps->prev_x,
		     -event->y + pps->prev_y);
      pps->prev_x = event->x;
      pps->prev_y = event->y;
    }
}

void
zoom_set(GtkPandaPS *pps, float zoom)
{
  float org_zoom;

  org_zoom =  gtk_gs_get_zoom (GTK_GS (pps->gs));

  if (ABS(zoom - org_zoom) < 0.01)
	return ;

  if ( zoom < 0.02 ) {
	zoom = 0.02;    /* Min   2% */	  
  } else if ( zoom > 4) {
    zoom = 4;       /* Max 400% */
  }

  gtk_gs_set_zoom (GTK_GS (pps->gs), zoom);
}

static void
arrange_scale_combo(GtkPandaPS *pps, 
  float zoom)
{
  GtkComboBox *combo;
  guint i;
  
  combo = GTK_COMBO_BOX(pps->scale);
  for (i = 0; i < n_zoom_levels; i++)   {
    if (zoom_levels[i].level > 0 && zoom < zoom_levels[i].level) {
      g_signal_handlers_block_by_func(combo, combo_changed_cb, pps);
      gtk_combo_box_set_active(combo, i);
      g_signal_handlers_unblock_by_func(combo, combo_changed_cb, pps);
      break;
    }
  }
}

void
gtk_panda_ps_zoom_fit_page (GtkPandaPS *pps)
{
  GtkComboBox *combo;
  float z;

  combo = GTK_COMBO_BOX(pps->scale);
  g_signal_handlers_block_by_func(combo, combo_changed_cb, pps);
  gtk_combo_box_set_active(combo, 0);
  g_signal_handlers_unblock_by_func(combo, combo_changed_cb, pps);

  z = gtk_gs_zoom_to_fit (GTK_GS (pps->gs), FALSE);
  if (z > 0)
	zoom_set(pps,z);
}

void
gtk_panda_ps_zoom_fit_width (GtkPandaPS *pps)
{
  GtkComboBox *combo;
  float z;

  combo = GTK_COMBO_BOX(pps->scale);
  g_signal_handlers_block_by_func(combo, combo_changed_cb, pps);
  gtk_combo_box_set_active(combo, 1);
  g_signal_handlers_unblock_by_func(combo, combo_changed_cb, pps);
  
  z = gtk_gs_zoom_to_fit (GTK_GS (pps->gs), TRUE);
  if (z > 0)
	zoom_set(pps,z);
}

void
gtk_panda_ps_zoom_in (GtkPandaPS *pps)
{
  float z;

  z = gtk_gs_get_zoom (GTK_GS (pps->gs)) * 1.2;
  arrange_scale_combo(pps, z);
  zoom_set(pps,z);
}


void
gtk_panda_ps_zoom_out (GtkPandaPS *pps)
{
  float z;

  z = gtk_gs_get_zoom (GTK_GS (pps->gs)) / 1.2;
  arrange_scale_combo(pps, z);
  zoom_set(pps,z);
}

static gboolean
row_is_separator (GtkTreeModel *model,
  GtkTreeIter  *iter,
  gpointer      data)
{
  gboolean is_sep;
  gtk_tree_model_get (model, iter, COL_IS_SEP, &is_sep, -1);
  return is_sep;
}

static void
combo_changed_cb (GtkComboBox *combo, GtkWidget *pps)
{
  gint index;
  float zoom;

  index = gtk_combo_box_get_active (combo);
  zoom = zoom_levels[index].level;

  if (zoom == SCALE_ZOOM_FIT_PAGE) {
    gtk_panda_ps_zoom_fit_page(GTK_PANDA_PS(pps));
  } else if (zoom == SCALE_ZOOM_FIT_WIDTH) {
    gtk_panda_ps_zoom_fit_width(GTK_PANDA_PS(pps));
  } else if (zoom == SCALE_ZOOM_SEPARATOR) {
    // do nothing
  } else {
    zoom_set(GTK_PANDA_PS(pps), (gfloat)zoom);
  }
}

static void
gtk_panda_ps_init (GtkPandaPS *pps)
{
  GtkWidget *frame1;
  GtkWidget *align;
  GtkAccelGroup *accel_group;
  GtkObject *hadj = gtk_adjustment_new (0.0, 0.0, 1.0, 1.0, 1.0, 0.5);
  GtkObject *vadj = gtk_adjustment_new (0.0, 0.0, 1.0, 1.0, 1.0, 0.5);
  GtkCellRenderer *renderer;
  GtkListStore    *store;
  GtkTreeIter      iter;
  guint i;

  pps->pan = 0;
  pps->prev_x = pps->prev_y = 0;

  /* GtkGS */
  pps->gs = gtk_gs_new (GTK_ADJUSTMENT (hadj), GTK_ADJUSTMENT (vadj));
  gtk_widget_show (pps->gs);
  gtk_widget_set_events (pps->gs,
			 GDK_BUTTON_PRESS_MASK
			 | GDK_BUTTON_RELEASE_MASK
			 | GDK_POINTER_MOTION_MASK);
  g_signal_connect (GTK_OBJECT (pps->gs), "button_press_event",
		      G_CALLBACK (button_press_callback), pps);
  g_signal_connect (GTK_OBJECT (pps->gs), "button_release_event",
		      G_CALLBACK (button_release_callback), pps);
  g_signal_connect (GTK_OBJECT (pps->gs), "motion_notify_event",
		      G_CALLBACK (motion_callback), pps);

  /* Create accelerator table */
  accel_group = gtk_accel_group_new ();

  /* scale combo */
  store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_BOOLEAN);
  for (i = 0; i < n_zoom_levels; i++)
  {
     gtk_list_store_append(store, &iter);
  
     if (zoom_levels[i].name != NULL) {
       gtk_list_store_set (store, &iter,
         COL_TEXT, _(zoom_levels[i].name),
         -1);
     } else {
       gtk_list_store_set (store, &iter,
         COL_IS_SEP, zoom_levels[i].name == NULL,
         -1);
     }
  }
  pps->scale = gtk_combo_box_new_with_model(GTK_TREE_MODEL (store));
  g_object_unref (store);
  renderer = gtk_cell_renderer_text_new ();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT (pps->scale), renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT (pps->scale), renderer,
    "text", COL_TEXT, NULL);
  gtk_combo_box_set_row_separator_func (GTK_COMBO_BOX(pps->scale),
    (GtkTreeViewRowSeparatorFunc) row_is_separator, NULL, NULL);
  gtk_combo_box_set_focus_on_click (GTK_COMBO_BOX(pps->scale), FALSE);
  g_object_ref (pps->scale);
  gtk_object_sink (GTK_OBJECT (pps->scale));
  gtk_widget_show (GTK_WIDGET (pps->scale));
  gtk_combo_box_set_active (GTK_COMBO_BOX(pps->scale), 0);
  g_signal_connect (pps->scale, "changed",
    G_CALLBACK (combo_changed_cb), pps);

  /* frame */
  frame1 = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_IN);
  gtk_widget_show (frame1);
  gtk_container_add (GTK_CONTAINER (frame1), pps->gs);

  align = gtk_alignment_new(0, 0.5, 0, 0);
  gtk_alignment_set_padding (GTK_ALIGNMENT(align), 0, 0, 8, 8);
  gtk_container_add(GTK_CONTAINER(align), pps->scale);
  gtk_widget_show(align);

  GTK_BOX (pps)->spacing = 4;
  GTK_BOX (pps)->homogeneous = FALSE;
  
  //gtk_box_pack_start(GTK_BOX (pps), pps->scale, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX (pps), align, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX (pps), frame1, TRUE, TRUE, 0);

  GTK_WIDGET_SET_FLAGS (pps, GTK_CAN_FOCUS);
}

GtkWidget *
gtk_panda_ps_new (void)
{
  return g_object_new (GTK_PANDA_TYPE_PS, NULL);
}

// public API

void
gtk_panda_ps_load (GtkPandaPS *pps, const char *filename)
{
  static gboolean reset_size = TRUE;
  float z;
		
  if (reset_size){
	z = gtk_gs_zoom_to_fit (GTK_GS (pps->gs), TRUE);
    reset_size = FALSE;
  } else {
    z = gtk_gs_get_zoom (GTK_GS (pps->gs));
  }

  gtk_gs_load (GTK_GS (pps->gs), filename);
  gtk_gs_set_page_size(GTK_GS (pps->gs), -1, 0);
  zoom_set(pps, z);
}
