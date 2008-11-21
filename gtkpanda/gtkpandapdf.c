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
#include <gtk/gtk.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkbindings.h>
#include <gtk/gtkmarshal.h>
#include <gdk/gdkkeysyms.h>

#include "gtkpandaintl.h"
#include "gtkpandapdf.h"

static void gtk_panda_pdf_class_init    (GtkPandaPDFClass *klass);
static void gtk_panda_pdf_init          (GtkPandaPDF      *pdf);
static void gtk_panda_pdf_destroy       (GtkObject      *pdf);

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

static guint signals [LAST_SIGNAL] = { 0 };

void gtk_panda_pdf_zoom_fit_page(GtkPandaPDF *pdf);
void gtk_panda_pdf_zoom_fit_width(GtkPandaPDF *pdf);
void gtk_panda_pdf_zoom_in(GtkPandaPDF *pdf);
void gtk_panda_pdf_zoom_out(GtkPandaPDF *pdf);
static void combo_changed_cb (GtkComboBox *combo, GtkPandaPDF *pdf);

GType
gtk_panda_pdf_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      static const GTypeInfo info =
      {
        sizeof (GtkPandaPDFClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_pdf_class_init,
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaPDF),
        0,
        (GInstanceInitFunc) gtk_panda_pdf_init
      };

      type = g_type_register_static( GTK_TYPE_VBOX, "GtkPandaPDF", &info, 0);
    }

  return type;
}

static void
gtk_panda_pdf_class_init (GtkPandaPDFClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GtkObjectClass *object_class;
  GtkWidgetClass *widget_class;
  GtkBindingSet *binding_set;

  object_class = (GtkObjectClass *) klass;
  widget_class = (GtkWidgetClass *) klass;
  parent_class = gtk_type_class (GTK_TYPE_CONTAINER);

  object_class->destroy = gtk_panda_pdf_destroy;

  klass->zoom_fit_page = gtk_panda_pdf_zoom_fit_page;
  klass->zoom_fit_width = gtk_panda_pdf_zoom_fit_width;
  klass->zoom_in = gtk_panda_pdf_zoom_in;
  klass->zoom_out = gtk_panda_pdf_zoom_out;

  signals[ZOOM_FIT_PAGE] =
  g_signal_new ("zoom_fit_page",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, zoom_fit_page),
        NULL, NULL,
        gtk_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[ZOOM_FIT_WIDTH] =
  g_signal_new ("zoom_fit_width",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, zoom_fit_width),
        NULL, NULL,
        gtk_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[ZOOM_IN] =
  g_signal_new ("zoom_in",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, zoom_in),
        NULL, NULL,
        gtk_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[ZOOM_OUT] =
  g_signal_new ("zoom_out",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, zoom_out),
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

static void
gtk_panda_pdf_destroy(GtkObject *object)
{
  GtkPandaPDF *self = GTK_PANDA_PDF(object);

  if (self->doc) g_object_unref(self->doc);
  if (self->pixbuf) g_object_unref(self->pixbuf);
  if (self->data) g_free(self->data);
  GTK_OBJECT_CLASS(parent_class)->destroy(object);
}

static void
arrange_scale_combo(GtkPandaPDF *self, 
  float zoom)
{
#if 0
  GtkComboBox *combo;
  guint i;

  /* FIXME cause Segv when running next line*/
  combo = GTK_COMBO_BOX(self->scale);
  for (i = 0; i < n_zoom_levels; i++)   {
    if (zoom_levels[i].level > 0 && zoom < zoom_levels[i].level) {
      g_signal_handlers_block_by_func(combo, combo_changed_cb, self);
      gtk_combo_box_set_active(combo, i);
      g_signal_handlers_unblock_by_func(combo, combo_changed_cb, self);
      break;
    }
  }
#endif
}

static double
compute_zoom(GtkPandaPDF *self)
{
  PopplerPage *page;
  double doc_w, doc_h, isize;
  double zoom;

  page = poppler_document_get_page(self->doc, self->npage);
  poppler_page_get_size(page, &doc_w, &doc_h);
  if (self->zoom == SCALE_ZOOM_FIT_PAGE) {
    isize = (gtk_scrolled_window_get_vadjustment(
              GTK_SCROLLED_WINDOW(self->scroll)))->page_size;
    zoom = isize / doc_h;
  } else if (self->zoom == SCALE_ZOOM_FIT_WIDTH) {
    isize = (gtk_scrolled_window_get_hadjustment(
              GTK_SCROLLED_WINDOW(self->scroll)))->page_size;
    zoom = isize / doc_w;
  } else {
    zoom = self->zoom;
  }
  g_object_unref(page);
  return zoom;
}

static void
render_page(GtkPandaPDF *self)
{
  double doc_w, doc_h;
  double zoom;
  PopplerPage *page;

  g_return_if_fail(self->doc);
  if (self->pixbuf) {
    g_object_unref(self->pixbuf);
    self->pixbuf = NULL;
  }
  page = poppler_document_get_page(self->doc, self->npage);

  zoom = compute_zoom(self);
  poppler_page_get_size(page, &doc_w, &doc_h);

  self->pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, 
    (int)(doc_w * zoom),
    (int)(doc_h * zoom));
  poppler_page_render_to_pixbuf(page, 0, 0, 
    (int)(doc_w), (int)(doc_h), zoom, 0, self->pixbuf);
  gtk_image_set_from_pixbuf(GTK_IMAGE(self->image), self->pixbuf);
  g_object_unref(page);
}

void
gtk_panda_pdf_zoom_fit_page (GtkPandaPDF *self)
{
  GtkComboBox *combo;

  if (self->zoom == SCALE_ZOOM_FIT_PAGE) return;

  combo = GTK_COMBO_BOX(self->scale);
  g_signal_handlers_block_by_func(combo, combo_changed_cb, self);
  gtk_combo_box_set_active(combo, 0);
  g_signal_handlers_unblock_by_func(combo, combo_changed_cb, self);

  self->zoom = SCALE_ZOOM_FIT_PAGE;
  render_page(self);
}

void
gtk_panda_pdf_zoom_fit_width (GtkPandaPDF *self)
{
  GtkComboBox *combo;

  if (self->zoom == SCALE_ZOOM_FIT_WIDTH) return;

  combo = GTK_COMBO_BOX(self->scale);
  g_signal_handlers_block_by_func(combo, combo_changed_cb, self);
  gtk_combo_box_set_active(combo, 1);
  g_signal_handlers_unblock_by_func(combo, combo_changed_cb, self);
  
  self->zoom = SCALE_ZOOM_FIT_WIDTH;
  render_page(self);
}

void
gtk_panda_pdf_zoom_in (GtkPandaPDF *self)
{
  double zoom;

  zoom = compute_zoom(self) * 1.2;
  if (zoom < 4.0) {
    self->zoom = zoom;
  } else {
    self->zoom = 4.0;
  }
  arrange_scale_combo(self, self->zoom);
  render_page(self);
}

void
gtk_panda_pdf_zoom_out (GtkPandaPDF *self)
{
  double zoom;

  zoom = compute_zoom(self) / 1.2;
  if (zoom > 0.2) {
    self->zoom = zoom;
  } else {
    self->zoom = 0.2;
  }
  arrange_scale_combo(self, self->zoom);
  render_page(self);
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
combo_changed_cb (GtkComboBox *combo, GtkPandaPDF *self)
{
  gint index;

  index = gtk_combo_box_get_active (combo);
  self->zoom = zoom_levels[index].level;
  render_page(self);
}

static void
gtk_panda_pdf_init (GtkPandaPDF *self)
{
  GtkWidget *align;
  GtkAccelGroup *accel_group;
  GtkCellRenderer *renderer;
  GtkListStore    *store;
  GtkTreeIter      iter;
  guint i;

  self->zoom = SCALE_ZOOM_FIT_WIDTH;

  self->image = gtk_image_new();
  self->scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->scroll), 
    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(self->scroll), 
    self->image);

  self->pixbuf = NULL;
  self->doc = NULL;
  self->data = NULL;

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

  /* combo_box */
  self->scale = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
  renderer = gtk_cell_renderer_text_new ();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT (self->scale), renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT (self->scale), 
    renderer,
    "text", 
    COL_TEXT, 
    NULL);
  gtk_combo_box_set_row_separator_func (GTK_COMBO_BOX(self->scale),
    (GtkTreeViewRowSeparatorFunc) row_is_separator, NULL, NULL);
  gtk_combo_box_set_focus_on_click (GTK_COMBO_BOX(self->scale), FALSE);
  gtk_combo_box_set_active (GTK_COMBO_BOX(self->scale), 0);
  gtk_combo_box_popup(GTK_COMBO_BOX(self->scale));
  g_signal_connect (GTK_COMBO_BOX(self->scale), "changed",
    G_CALLBACK (combo_changed_cb), self);
  g_object_unref (store);
  gtk_widget_show (GTK_WIDGET (self->scale));

  align = gtk_alignment_new(0, 0.5, 0, 0);
  gtk_alignment_set_padding (GTK_ALIGNMENT(align), 0, 0, 8, 8);
  gtk_container_add(GTK_CONTAINER(align), self->scale);

  gtk_box_set_spacing(GTK_BOX(self), 4);
  gtk_box_set_homogeneous(GTK_BOX(self), FALSE);
  
  gtk_box_pack_start(GTK_BOX (self), align, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX (self), self->scroll, TRUE, TRUE, 0);
  gtk_widget_show_all(GTK_WIDGET(self));

  GTK_WIDGET_SET_FLAGS (self, GTK_CAN_FOCUS);
}

// public API
GtkWidget *
gtk_panda_pdf_new (void)
{
  return g_object_new (GTK_PANDA_TYPE_PDF, NULL);
}

void
gtk_panda_pdf_set (GtkPandaPDF *self, int size, char *data)
{
  GError *error = NULL;

  if (self->doc) g_object_unref(self->doc);
  if (self->data) g_free(self->data);
  self->data = (char *)g_memdup(data, size);
  self->doc = poppler_document_new_from_data(self->data, size, NULL, &error);
  if (error != NULL) {
    fprintf(stderr, "can't open file: %s\n", error->message);
    gtk_image_set_from_pixbuf(GTK_IMAGE(self->image), NULL);
    g_error_free (error);
    return;
  }
  self->npage = 0;
  render_page(self);
}
