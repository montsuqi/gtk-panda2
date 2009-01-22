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
#include <glib.h>
#include <glib/gstdio.h>
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
  SAVE,
  PRINT,
  LAST_SIGNAL
};

static guint ps_signals [LAST_SIGNAL] = { 0 };

void gtk_panda_ps_zoom_fit_page(GtkPandaPS *ps);
void gtk_panda_ps_zoom_fit_width(GtkPandaPS *ps);
void gtk_panda_ps_zoom_in(GtkPandaPS *ps);
void gtk_panda_ps_zoom_out(GtkPandaPS *ps);
void gtk_panda_ps_save(GtkPandaPS *ps);
void gtk_panda_ps_print(GtkPandaPS *ps);

static void combo_changed_cb (GtkComboBox *combo, GtkWidget *pps);

static gchar *psfile = NULL;

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
  klass->save = gtk_panda_ps_save;
  klass->print = gtk_panda_ps_print;

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

  ps_signals[SAVE] =
  g_signal_new ("save",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPSClass, save),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  ps_signals[PRINT] =
  g_signal_new ("print",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPSClass, print),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
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
  gtk_binding_entry_add_signal (binding_set, GDK_s, GDK_CONTROL_MASK,
    "save", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_p, GDK_CONTROL_MASK,
    "print", 0);
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

void
gtk_panda_ps_print (GtkPandaPS *self)
{
  GtkWidget *dialog;
  GtkWidget *tview;
  GtkListStore *store;
  GType types[1];
  GValue *value;

  GtkTreeSelection *select;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GList *list;
  char *pname;

  char buff[256], cmd[256];
  char *p;
  FILE *fp;

  if (psfile == NULL) {
    return;
  }

  dialog = gtk_message_dialog_new (
    (GtkWindow *)gtk_widget_get_toplevel(GTK_WIDGET(self)),
    GTK_DIALOG_MODAL,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK_CANCEL,
    _("pritner selection"));

  types[0] = G_TYPE_STRING;
  store = gtk_list_store_newv(1, types);
  tview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tview));
  //gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
  gtk_tree_selection_set_mode(select, GTK_SELECTION_BROWSE);

  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW(tview),
    -1,
    _("printer"),
    gtk_cell_renderer_text_new (),
    "text", 0,
    NULL);

  snprintf(cmd, sizeof(cmd), "%s -a", LPSTAT_PATH);
  if ((fp = popen(cmd, "r")) != NULL) {
    while(1) {
      fgets(buff, sizeof(cmd), fp);
      if (feof(fp))break;
      p = strchr(buff, ' ');
      if (p != NULL) *p = '\0';
      gtk_list_store_append(store, &iter);
      value = g_new0(GValue, 1);
      g_value_init(value, G_TYPE_STRING);
      g_value_set_string(value, buff);
      gtk_list_store_set_value(store, &iter, 0 , value);
    }
    pclose(fp);
  }

  gtk_box_pack_end_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), tview);
  gtk_widget_show(tview);

  if ( gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK ) {

    select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tview));
    list = gtk_tree_selection_get_selected_rows(select, &model);

    if ( g_list_length(list) >= 1) {
      if ( gtk_tree_model_get_iter(model, &iter, 
        (GtkTreePath *)g_list_nth_data(list, 0)) ) {
        int pid;
        char *argv[5];

        gtk_tree_model_get(model, &iter, 0, &pname, -1);

        signal  (SIGCHLD, SIG_IGN);
        argv[0] = LPR_PATH;
        argv[1] = "-P";
        argv[2] = pname;
        argv[3] = psfile;
        argv[4] = (char *)NULL;
        if ((pid = fork()) == 0) {
          execv(LPR_PATH, argv);
          _exit(0);
        }
        g_free(pname);
      }
    }

    g_list_foreach (list, (GFunc)gtk_tree_path_free, NULL);
    g_list_free (list);
  }
  gtk_widget_destroy (dialog);
}

void
gtk_panda_ps_save (GtkPandaPS *self)
{
  GtkWidget *dialog;
  GError *error = NULL;
  gchar *data;
  gssize size;

  if (psfile == NULL) {
    return ;
  }

  g_file_get_contents(psfile, &data, &size, &error);
  if (error != NULL) {
    fprintf(stderr, "can't load: %s\n", error->message);
    g_error_free(error);
    return;
  }

  dialog = gtk_file_chooser_dialog_new ("Save File",
                      GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(self))),
                      GTK_FILE_CHOOSER_ACTION_SAVE,
                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                      NULL);
  gtk_file_chooser_set_do_overwrite_confirmation (
    GTK_FILE_CHOOSER (dialog), TRUE);

  gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog),
    "preview.ps");

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
      char *filename;

      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      if (!g_file_set_contents(filename,
            data, size, &error)) {
        GtkWidget *mdialog;
        mdialog = gtk_message_dialog_new (GTK_WINDOW(dialog),
                                  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  "%s",
                                  error->message);
        gtk_dialog_run (GTK_DIALOG (mdialog));
        gtk_widget_destroy (mdialog);
        g_error_free (error);
      }
      g_free(filename);
  }
  g_free(data);
  gtk_widget_destroy (dialog);
}

static void
save_clicked_cb (GtkButton *button, GtkPandaPS *self)
{
  gtk_panda_ps_save(self);
}


static void
print_clicked_cb (GtkButton *button, GtkPandaPS *self)
{
  gtk_panda_ps_print(self);
}

static void
gtk_panda_ps_init (GtkPandaPS *pps)
{
  GtkWidget *frame1;
  GtkWidget *hbox;
  GtkWidget *print_button;
  GtkWidget *save_button;
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

  /* save button */
  save_button = gtk_button_new_from_stock(GTK_STOCK_SAVE);
  g_signal_connect (G_OBJECT(save_button), "clicked",
    G_CALLBACK (save_clicked_cb), pps);

  /* print button */
  print_button = gtk_button_new_from_stock(GTK_STOCK_PRINT);
  g_signal_connect (G_OBJECT(print_button), "clicked",
    G_CALLBACK (print_clicked_cb), pps);

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
  gtk_combo_box_set_active (GTK_COMBO_BOX(pps->scale), 0);
  g_signal_connect (pps->scale, "changed",
    G_CALLBACK (combo_changed_cb), pps);

  /* frame */
  frame1 = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_IN);
  gtk_container_add (GTK_CONTAINER (frame1), pps->gs);

  GTK_BOX (pps)->spacing = 4;
  GTK_BOX (pps)->homogeneous = FALSE;

  hbox = gtk_hbox_new(FALSE, 0);

  gtk_box_pack_start(GTK_BOX (hbox), save_button, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX (hbox), print_button, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX (hbox), pps->scale, FALSE, FALSE, 2);

  gtk_box_pack_start(GTK_BOX (pps), hbox, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX (pps), frame1, TRUE, TRUE, 0);
  gtk_widget_show_all(GTK_WIDGET(pps));

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

void
gtk_panda_ps_set (GtkPandaPS *pps, int size, char *psdata)
{
  GError *error = NULL;
  static gboolean reset_size = TRUE;
  float z;

//  update_printer_list(pps->printer);

  if (psfile != NULL) {
    g_remove(psfile);
    g_free(psfile);
    psfile = NULL;
  }
  g_file_open_tmp("__gtk_panda_ps_XXXXXX.ps", &psfile, &error);
  if (error != NULL) {
    fprintf(stderr, "can't create file: %s\n", error->message);
    g_error_free(error);
    return;
  } 
  g_file_set_contents(psfile, (const gchar *)psdata, (gssize) size, &error);
  if (error != NULL) {
    fprintf(stderr, "can't write: %s\n", error->message);
    g_error_free(error);
    return;
  } 

  gtk_gs_load (GTK_GS (pps->gs), psfile);
  if (reset_size) {
    gtk_panda_ps_zoom_fit_width(pps);
    reset_size = FALSE;
  } else {
    z = gtk_gs_get_zoom (GTK_GS (pps->gs));
    gtk_gs_set_page_size(GTK_GS (pps->gs), -1, 0);
  	zoom_set(pps, z);
  }
}
