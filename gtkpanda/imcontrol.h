/* GTK - The GIMP Toolkit
 * Copyright (C) 2000 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <X11/Xlib.h>

#include "config.h"

/*
 * function
 */

void set_im_state_pre_focus(GtkWidget *widget, 
  GtkIMMulticontext *mim, gboolean enabled);
void set_im_state_post_focus(GtkWidget *widget, 
  GtkIMMulticontext *mim, gboolean enabled);

/*
 * for GtkIMContextXIM
 */

typedef struct _StatusWindow StatusWindow;
typedef struct _GtkXIMInfo GtkXIMInfo;
typedef struct _GtkIMContextXIM GtkIMContextXIM;

struct _GtkIMContextXIM
{
  GtkIMContext object;

  GtkXIMInfo *im_info;

  gchar *locale;
  gchar *mb_charset;

  GdkWindow *client_window;
  GtkWidget *client_widget;

  /* The status window for this input context; we claim the
   * status window when we are focused and have created an XIC
   */
  StatusWindow *status_window;

  gint preedit_size;
  gint preedit_length;
  gunichar *preedit_chars;
  XIMFeedback *feedbacks;

  gint preedit_cursor;
  
  XIMCallback preedit_start_callback;
  XIMCallback preedit_done_callback;
  XIMCallback preedit_draw_callback;
  XIMCallback preedit_caret_callback;

  XIMCallback status_start_callback;
  XIMCallback status_done_callback;
  XIMCallback status_draw_callback;

  XIMCallback string_conversion_callback;

  XIC ic;

  guint filter_key_release : 1;
  guint use_preedit : 1;
  guint finalizing : 1;
  guint in_toplevel : 1;
  guint has_focus : 1;
};

struct _GtkXIMInfo
{
  GdkScreen *screen;
  XIM im;
  char *locale;
  XIMStyle preedit_style_setting;
  XIMStyle status_style_setting;
  XIMStyle style;
  GtkSettings *settings;
  gulong status_set;
  gulong preedit_set;
#ifndef OLD_GTK
  gulong display_closed_cb;
#endif
  XIMStyles *xim_styles;
  GSList *ics;

  guint reconnecting :1;
  guint supports_string_conversion;
};

/* A context status window; these are kept in the status_windows list. */
struct _StatusWindow
{
  GtkWidget *window;
  
  /* Toplevel window to which the status window corresponds */
  GtkWidget *toplevel;

  /* Currently focused GtkIMContextXIM for the toplevel, if any */
  GtkIMContextXIM *context;
};
