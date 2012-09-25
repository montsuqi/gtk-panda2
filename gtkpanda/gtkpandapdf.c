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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "gtkpandaintl.h"
#include "gtkpandapdf.h"

static void gtk_panda_pdf_class_init    (GtkPandaPDFClass *klass);
static void gtk_panda_pdf_init          (GtkPandaPDF      *pdf);

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
  PAGE_NEXT,
  PAGE_PREV,
  SAVE,
  PRINT,
  LAST_SIGNAL
};

static guint signals [LAST_SIGNAL] = { 0 };

void gtk_panda_pdf_zoom_fit_page(GtkPandaPDF *pdf);
void gtk_panda_pdf_zoom_fit_width(GtkPandaPDF *pdf);
void gtk_panda_pdf_zoom_in(GtkPandaPDF *pdf);
void gtk_panda_pdf_zoom_out(GtkPandaPDF *pdf);
void gtk_panda_pdf_next_page(GtkPandaPDF *pdf);
void gtk_panda_pdf_prev_page(GtkPandaPDF *pdf);
void gtk_panda_pdf_save(GtkPandaPDF *pdf);

static void combo_changed_cb (GtkComboBox *combo, GtkPandaPDF *pdf);
static void _gtk_panda_pdf_print(GtkPandaPDF *self);

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
  GtkBindingSet *binding_set;

  parent_class = g_type_class_ref (GTK_TYPE_CONTAINER);

  klass->zoom_fit_page = gtk_panda_pdf_zoom_fit_page;
  klass->zoom_fit_width = gtk_panda_pdf_zoom_fit_width;
  klass->zoom_in = gtk_panda_pdf_zoom_in;
  klass->zoom_out = gtk_panda_pdf_zoom_out;
  klass->page_next = gtk_panda_pdf_next_page;
  klass->page_prev = gtk_panda_pdf_prev_page;
  klass->save = gtk_panda_pdf_save;
  klass->print = _gtk_panda_pdf_print;

  signals[ZOOM_FIT_PAGE] =
  g_signal_new ("zoom_fit_page",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, zoom_fit_page),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[ZOOM_FIT_WIDTH] =
  g_signal_new ("zoom_fit_width",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, zoom_fit_width),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[ZOOM_IN] =
  g_signal_new ("zoom_in",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, zoom_in),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[ZOOM_OUT] =
  g_signal_new ("zoom_out",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, zoom_out),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[PAGE_NEXT] =
  g_signal_new ("page_next",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, page_next),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[PAGE_PREV] =
  g_signal_new ("page_prev",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, page_prev),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[SAVE] =
  g_signal_new ("save",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, save),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[PRINT] =
  g_signal_new ("print",
        G_OBJECT_CLASS_TYPE (gobject_class),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaPDFClass, print),
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
  gtk_binding_entry_add_signal (binding_set, GDK_Right, GDK_CONTROL_MASK,
    "page_next", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_Left, GDK_CONTROL_MASK,
    "page_prev", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_Up, GDK_CONTROL_MASK,
    "page_prev", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_Down, GDK_CONTROL_MASK,
    "page_next", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_s, GDK_CONTROL_MASK,
    "save", 0);
  gtk_binding_entry_add_signal (binding_set, GDK_p, GDK_CONTROL_MASK,
    "print", 0);
}

static double
compute_zoom(GtkPandaPDF *self)
{
  PopplerPage *page;
  GtkAdjustment *adj;
  double doc_w, doc_h, scroll_w, scroll_h;
  double zoom;

  page = poppler_document_get_page(self->doc, self->pageno);

  adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(self->scroll));
  scroll_h = gtk_adjustment_get_page_size(adj);
  adj = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(self->scroll));
  scroll_w = gtk_adjustment_get_page_size(adj);

  poppler_page_get_size(page, &doc_w, &doc_h);
  if (self->zoom == SCALE_ZOOM_FIT_PAGE) {
    double zoom_h, zoom_w;
    zoom_h = scroll_h / doc_h;
    zoom_w = scroll_w / doc_w;
    zoom = zoom_h < zoom_w ? zoom_h : zoom_w;
    if (zoom < 0.2) zoom = 1.0;
  } else if (self->zoom == SCALE_ZOOM_FIT_WIDTH) {
    zoom = scroll_w / doc_w;
    if (zoom < 0.2) zoom = 1.0;
  } else {
    zoom = self->zoom;
  }
  g_object_unref(page);
  return zoom;
}

static void
arrange_scale_combo(GtkPandaPDF *self)
{
  GtkComboBox *combo;
  guint i;

  combo = GTK_COMBO_BOX(self->scale);
  for (i = 0; i < n_zoom_levels; i++)   {
    if (zoom_levels[i].level > 0 && self->zoom < zoom_levels[i].level) {
      g_signal_handlers_block_by_func(combo, combo_changed_cb, self);
      gtk_combo_box_set_active(combo, i);
      g_signal_handlers_unblock_by_func(combo, combo_changed_cb, self);
      break;
    }
  }
}

static gboolean
render_page(GtkPandaPDF *self)
{
  static cairo_surface_t *surface = NULL;
  cairo_t *cr;
  double width, height;
  double zoom;
  PopplerPage *page;
  GdkWindow *window;

  if (self->doc == NULL) {
    return FALSE;
  }
  if (surface != NULL) {
    cairo_surface_destroy(surface);
    surface = NULL;
  }

  page = poppler_document_get_page(self->doc, self->pageno);

  zoom = compute_zoom(self);
  poppler_page_get_size(page, &width, &height);
 
  width *= zoom;
  height *= zoom;

  gtk_widget_set_size_request(self->drawingarea,(int)width,(int)height);

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
    width,height);
  cr = cairo_create(surface);
  cairo_save(cr);
  cairo_scale(cr,zoom,zoom);
  poppler_page_render(page,cr);
  cairo_restore(cr);

  cairo_set_operator (cr, CAIRO_OPERATOR_DEST_OVER);
  cairo_set_source_rgb (cr, 1., 1., 1.);
  cairo_paint (cr);
  cairo_destroy (cr);

  window = gtk_widget_get_window(self->drawingarea);
  if (window != NULL) {
    cr = gdk_cairo_create(window);
    cairo_set_source_surface(cr,surface,0,0);
    cairo_paint(cr);
    cairo_destroy(cr);
  }
  g_object_unref(page);
  return FALSE;
}

static gboolean
cb_expose(GtkWidget *widget,
  GdkEventExpose *event,
  GtkPandaPDF *self)
{
  return render_page(self);
}

void
gtk_panda_pdf_save (GtkPandaPDF *self)
{
  GtkWidget *dialog;

  if (self->data == NULL) return;
  
  dialog = gtk_file_chooser_dialog_new ("Save File",
  				      GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(self))),
  				      GTK_FILE_CHOOSER_ACTION_SAVE,
  				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
  				      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
  				      NULL);
  gtk_file_chooser_set_do_overwrite_confirmation ( 
    GTK_FILE_CHOOSER (dialog), TRUE);
  
  gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), 
    "preview.pdf");
  
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
      char *filename;
      GError *error = NULL;

      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

      if (!g_file_set_contents(filename, 
            self->data, self->size, &error)) {
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
  gtk_widget_destroy (dialog);
}

static void 
begin_print(GtkPrintOperation *print, 
  GtkPrintContext *context, 
  GtkPandaPDF *self)
{   
    gtk_print_operation_set_n_pages(print,
      poppler_document_get_n_pages(self->doc));
}

static void 
draw_page(GtkPrintOperation *print, 
  GtkPrintContext *context, 
  gint pageno, 
  GtkPandaPDF *self)
{
  PopplerPage *page;
  gdouble doc_w, doc_h;
  cairo_t *cr;

  page = poppler_document_get_page(self->doc, pageno);
  if (page == NULL) return;
  poppler_page_get_size(page, &doc_w, &doc_h);

  cr = gtk_print_context_get_cairo_context(context);
#ifdef POPPLER_0_8
  /* may be need poppler >= 0.8 */
  poppler_page_render_for_printing(page, cr);
#else
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.0);
  poppler_page_render(page, cr);
#endif
  g_object_unref(page);
}

static void
arrange_orientation(GtkPandaPDF *self, GtkPageSetup *ps)
{
  PopplerPage *page;
  gdouble doc_w, doc_h;
  int w, h;

  page = poppler_document_get_page(self->doc, 0);
  if (page == NULL) return;
  poppler_page_get_size(page, &doc_w, &doc_h);
  w = (int)(doc_w); 
  h = (int)(doc_h);

  if (w > h) {
    gtk_page_setup_set_orientation(ps, GTK_PAGE_ORIENTATION_LANDSCAPE);
  } else {
    gtk_page_setup_set_orientation(ps, GTK_PAGE_ORIENTATION_PORTRAIT);
  }
  g_object_unref(page);
}

static void
_gtk_panda_pdf_print(GtkPandaPDF *self)
{
  gtk_panda_pdf_print(self,TRUE);
}

void
gtk_panda_pdf_print(GtkPandaPDF *self,
  gboolean showdialog)
{
  GtkPrintOperation *print;
  static GtkPrintSettings *settings = NULL;
  GtkPageSetup *page_setup;
  GtkPrintOperationResult r;
  GtkPrintOperationAction action;

  if (self->doc == NULL) return;

  print = gtk_print_operation_new();
  g_assert(print);
  g_signal_connect(print, "draw_page", 
    G_CALLBACK(draw_page), (gpointer) self);  
  g_signal_connect(print, "begin_print", 
    G_CALLBACK(begin_print), (gpointer) self);

  page_setup = gtk_print_operation_get_default_page_setup(print);
  if (page_setup == NULL)
    page_setup = gtk_page_setup_new();
  gtk_page_setup_set_top_margin(page_setup, 0.0, GTK_UNIT_MM);    
  gtk_page_setup_set_bottom_margin(page_setup, 0.0, GTK_UNIT_MM);    
  gtk_page_setup_set_left_margin(page_setup, 0.0, GTK_UNIT_MM);    
  gtk_page_setup_set_right_margin(page_setup, 0.0, GTK_UNIT_MM);    
  arrange_orientation(self, page_setup);
  gtk_print_operation_set_default_page_setup(print, page_setup);

  if (settings) gtk_print_operation_set_print_settings(print, settings);
  gtk_print_operation_set_job_name(print, "gtk_panda_pdf_print");
  gtk_print_operation_set_n_pages(print , 
    poppler_document_get_n_pages(self->doc));
  if (showdialog) {
    action = GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG;
  } else {
    action = GTK_PRINT_OPERATION_ACTION_PRINT;
  }
  r = gtk_print_operation_run(print, action, NULL, NULL);

  if (r == GTK_PRINT_OPERATION_RESULT_APPLY)
  {   
      if (settings)
          g_object_unref(settings);
      settings = g_object_ref(gtk_print_operation_get_print_settings(print));
  }
  g_object_unref(print);
}

void
gtk_panda_pdf_next_page(GtkPandaPDF *self)
{
  int npage;
  gchar buf[8];

  if (self->doc == NULL) return;
  npage = poppler_document_get_n_pages(self->doc);
  if (self->pageno < npage - 1) {
    self->pageno += 1;
    sprintf(buf,"%d",self->pageno+1);
    gtk_entry_set_text(GTK_ENTRY(self->page_entry),buf);
    render_page(self);
  }
}

void
gtk_panda_pdf_prev_page(GtkPandaPDF *self)
{
  gchar buf[8];

  if (self->doc == NULL) return;
  if (self->pageno >= 1) {
    self->pageno -= 1;
    sprintf(buf,"%d",self->pageno+1);
    gtk_entry_set_text(GTK_ENTRY(self->page_entry),buf);
    render_page(self);
  }
}

void
gtk_panda_pdf_goto_page(GtkPandaPDF *self,int page)
{
  int npage;
  gchar buf[8];

  if (self->doc == NULL) {
    return;
  }
  npage = poppler_document_get_n_pages(self->doc);
  if (page > npage - 1) {
    page = npage - 1;
  }
  if (page < 0) {
    page = 0;
  }
  self->pageno = page;
  render_page(self);
  sprintf(buf,"%d",self->pageno+1);
  gtk_entry_set_text(GTK_ENTRY(self->page_entry),buf);
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
  arrange_scale_combo(self);
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
  arrange_scale_combo(self);
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

  if (self->doc == NULL) return;
  index = gtk_combo_box_get_active (combo);
  self->zoom = zoom_levels[index].level;
  render_page(self);
}

static void
prev_clicked_cb(GtkButton *button, GtkPandaPDF *self)
{
  gtk_panda_pdf_prev_page(self);
}

static void
next_clicked_cb(GtkButton *button, GtkPandaPDF *self)
{
  gtk_panda_pdf_next_page(self);
}

static void
page_entry_activated_cb(GtkEntry *entry,GtkPandaPDF *self)
{
  gtk_panda_pdf_goto_page(self,atoi(gtk_entry_get_text(entry))-1);
}

static void
save_clicked_cb (GtkButton *button, GtkPandaPDF *self)
{
  gtk_panda_pdf_save(self);
}

static void
print_clicked_cb (GtkButton *button, GtkPandaPDF *self)
{
  _gtk_panda_pdf_print(self);
}

static void
button_press_cb (GtkWidget *widget, GdkEventButton *ev, gpointer data)
{
  GtkPandaPDF *self;
  gint x;
  gint y;

  self = GTK_PANDA_PDF(data);
  if (ev->button == 1) {
    self->pan = TRUE;
    gdk_window_get_pointer(gtk_widget_get_root_window(widget), &x, &y, NULL);
    self->bx = x;
    self->by = y;
  }
}

static void
button_release_cb (GtkWidget *widget, GdkEventButton *ev, gpointer data)
{
  GtkPandaPDF *self;

  self = GTK_PANDA_PDF(data);
  self->pan = FALSE;
}

static void
motion_cb (GtkWidget *widget, GdkEventButton *ev, gpointer data)
{
  GtkPandaPDF *self;
  GtkWidget *hsb;
  GtkWidget *vsb;
  gint x;
  gint y;

  self = GTK_PANDA_PDF(data);
  if (self->pan) {
     gdk_window_get_pointer(gtk_widget_get_root_window(widget), &x, &y, NULL);
     hsb = gtk_scrolled_window_get_hscrollbar(GTK_SCROLLED_WINDOW(self->scroll));
     vsb = gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(self->scroll));
     gtk_range_set_value(GTK_RANGE(hsb), gtk_range_get_value(GTK_RANGE(hsb)) + self->bx - x);
     gtk_range_set_value(GTK_RANGE(vsb), gtk_range_get_value(GTK_RANGE(vsb)) + self->by - y);
     self->bx = x;
     self->by = y;
  }
}

static void
gtk_panda_pdf_init (GtkPandaPDF *self)
{
  GtkWidget *hbox;
  GtkWidget *prev_button;
  GtkWidget *next_button;
  GtkWidget *print_button;
  GtkWidget *save_button;
  GtkWidget *child;
  GtkCellRenderer *renderer;
  GtkListStore    *store;
  GtkTreeIter      iter;
  guint i;

  g_object_set(gtk_settings_get_default(),"gtk-button-images",TRUE,NULL);

  self->zoom = SCALE_ZOOM_FIT_WIDTH;

  self->drawingarea = gtk_drawing_area_new();
  g_signal_connect (G_OBJECT(self->drawingarea), "expose_event",
    G_CALLBACK (cb_expose),
    (gpointer)self);

  self->scroll = gtk_scrolled_window_new(NULL, NULL);
#if 0 // for glclient2 thread failure ; gtk2 bug?
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->scroll), 
    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
#else
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->scroll), 
    GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
#endif
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(self->scroll), 
    self->drawingarea);
  child = gtk_bin_get_child(GTK_BIN(self->scroll));
  gtk_widget_add_events(child,
    GDK_BUTTON_PRESS_MASK
    | GDK_BUTTON_RELEASE_MASK
    | GDK_POINTER_MOTION_MASK
    | GDK_ENTER_NOTIFY_MASK
    | GDK_LEAVE_NOTIFY_MASK);
  g_signal_connect (GTK_OBJECT (child), "button_press_event",
		      G_CALLBACK (button_press_cb), self);
  g_signal_connect (GTK_OBJECT (child), "button_release_event",
		      G_CALLBACK (button_release_cb), self);
  g_signal_connect (GTK_OBJECT (child), "motion_notify_event",
		      G_CALLBACK (motion_cb), self);

  self->doc = NULL;
  self->data = NULL;
  self->size = 0;
  self->pan = FALSE;

  /* prev button */
  prev_button = gtk_button_new_from_stock(GTK_STOCK_GO_BACK);
  g_signal_connect (G_OBJECT(prev_button), "clicked",
    G_CALLBACK (prev_clicked_cb), self);

  /* next button */
  next_button = gtk_button_new_from_stock(GTK_STOCK_GO_FORWARD);
  g_signal_connect (G_OBJECT(next_button), "clicked",
    G_CALLBACK (next_clicked_cb), self);

  /* page entry */
  self->page_entry = gtk_entry_new();
  g_signal_connect (G_OBJECT(self->page_entry), "activate",
    G_CALLBACK (page_entry_activated_cb), self);
  gtk_widget_set_size_request(self->page_entry,40,40);

  /* page label */
  self->page_label = gtk_label_new("");
  gtk_widget_set_size_request(self->page_label,40,40);

  /* save button */
  save_button = gtk_button_new_from_stock(GTK_STOCK_SAVE);
  g_signal_connect (G_OBJECT(save_button), "clicked",
    G_CALLBACK (save_clicked_cb), self);
 
  /* print button */
  print_button = gtk_button_new_from_stock(GTK_STOCK_PRINT);
  g_signal_connect (G_OBJECT(print_button), "clicked",
    G_CALLBACK (print_clicked_cb), self);

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
  gtk_combo_box_set_active (GTK_COMBO_BOX(self->scale), 1);
  g_signal_connect (G_OBJECT(self->scale), "changed",
    G_CALLBACK (combo_changed_cb), self);
  g_object_unref (store);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX (hbox), prev_button, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX (hbox), next_button, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX (hbox), self->page_entry, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX (hbox), self->page_label, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX (hbox), self->scale, FALSE, FALSE, 2);

  gtk_box_pack_start(GTK_BOX (hbox), save_button, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX (hbox), print_button, FALSE, FALSE, 2);

  gtk_box_set_spacing(GTK_BOX(self), 2);
  gtk_box_set_homogeneous(GTK_BOX(self), FALSE);
  
  gtk_box_pack_start(GTK_BOX (self), hbox, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX (self), self->scroll, TRUE, TRUE, 0);
  gtk_widget_show_all(GTK_WIDGET(self));

  gtk_widget_set_can_focus(save_button,FALSE);
  gtk_widget_set_can_focus(print_button,FALSE);
  gtk_widget_set_can_focus(self->scale,FALSE);
  gtk_widget_set_can_focus(GTK_WIDGET(self),TRUE);
}

// public API
GtkWidget *
gtk_panda_pdf_new (void)
{
  return g_object_new (GTK_PANDA_TYPE_PDF, NULL);
}

gboolean
gtk_panda_pdf_set (GtkPandaPDF *self, int size, char *data)
{
  GError *error = NULL;
  gchar buf[8];

  gtk_widget_hide(GTK_WIDGET(self->drawingarea));
  if (self->doc) {
    g_object_unref(self->doc);
    self->doc = NULL;
  }
  if (self->data) {
    g_free(self->data);
    self->size = 0;
    self->data = NULL;
  }
  if (data == NULL || size <= 0) {
    return FALSE;
  }
  gtk_widget_show(GTK_WIDGET(self->drawingarea));
  self->data = g_memdup(data, size);
  self->size = size;
  self->doc = poppler_document_new_from_data(self->data, self->size, 
    NULL, &error);
  if (error != NULL) {
    fprintf(stderr, "%s\n", error->message);
    g_error_free (error);
    return FALSE;
  }
  self->pageno = 0;
  sprintf(buf,"/%d",poppler_document_get_n_pages(self->doc));
  gtk_label_set_text(GTK_LABEL(self->page_label),buf);
  sprintf(buf,"%d",self->pageno+1);
  gtk_entry_set_text(GTK_ENTRY(self->page_entry),buf);
  render_page(self);

  return TRUE;
}

gboolean
gtk_panda_pdf_load(GtkPandaPDF *self, char *fname)
{
  GError *error = NULL;
  gchar *buf;
  gsize size;

  if (!g_file_get_contents(fname,&buf,&size,&error)) {
     g_error_free(error);
     return FALSE;
  }
  return gtk_panda_pdf_set(self,size,buf);
}

int
gtk_panda_pdf_get_page_count(GtkPandaPDF *self)
{
  if (self->doc == NULL) {
    return 0;
  }
  return poppler_document_get_n_pages(self->doc);
}
