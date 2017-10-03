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


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

#include "imcontrol.h"

static gboolean im_control_enabled = TRUE;
static gboolean is_fcitx = FALSE;
static gboolean check_fcitx = TRUE;

#define IGNORE_TOGGLE_KEY_PERIOD (100)

static void
emit_toggle_key(GtkWidget *widget)
{
#ifdef IBUS_1_5
  static guint32 prev = 0;
#endif
  guint32 t;
  GdkEvent *kevent;
  GdkWindow *window;
  GtkIMMulticontext *mim;

  if (check_fcitx) {
    if (GTK_IS_ENTRY(widget)) {
      mim = GTK_IM_MULTICONTEXT(GTK_ENTRY(widget)->im_context);
      if (mim != NULL) {
        if (mim->context_id != NULL && !strcmp("fcitx",mim->context_id)) {
          is_fcitx = TRUE;
        } else {
          is_fcitx = FALSE;
        }
        check_fcitx = FALSE;
      }
    }
  }

  window = gtk_widget_get_parent_window(widget);
  t = gdk_x11_get_server_time(window);

#if 0
fprintf(stderr,"emit_toggle_key %p %ld\n",widget,t);
#endif

#ifdef IBUS_1_5
  if (!is_fcitx) {
    if ((t - prev) < IGNORE_TOGGLE_KEY_PERIOD) {
#     if 0
      fprintf(stderr,"ignore\n");
#     endif
      return;
    }
  }
  prev = t;
#endif

  kevent = gdk_event_new(GDK_KEY_PRESS);
  kevent->key.window = window;
  kevent->key.send_event = 0;
  kevent->key.time = t;
  kevent->key.state = 16;
  kevent->key.length = 0;
  kevent->key.string = "";
  kevent->key.keyval = GDK_KEY_Zenkaku_Hankaku;
  if (GTK_IS_ENTRY(widget)) {
    gtk_entry_im_context_filter_keypress(GTK_ENTRY(widget),(GdkEventKey*)kevent);
  } else if (GTK_IS_TEXT_VIEW(widget)) {
    gtk_text_view_im_context_filter_keypress(GTK_TEXT_VIEW(widget),(GdkEventKey*)kevent);
  }
}

static void 
_set_im(
  GtkWidget *widget, 
  gboolean enable)
{
  if (widget == NULL || !G_IS_OBJECT(widget)) {
    return;
  }
  if (!gtk_widget_has_focus(widget)) {
    return;
  }
  if (!im_control_enabled) {
    return;
  }
#ifdef IBUS_1_5
  {
    gchar *statusfile,*buf;
    size_t size;
    int uid;

    uid = (int)getuid();
    statusfile = g_strdup_printf("/tmp/im-status.%d.txt",uid);
    if (g_file_get_contents(statusfile,&buf,&size,NULL)) {
#if 0
fprintf(stderr,"buf[0]:%c\n",buf[0]);
#endif
      if (enable) {
        if (size > 0 && buf[0] == '0') {
          emit_toggle_key(widget);
        }
      } else {
        if (size > 0 && buf[0] == '1') {
          emit_toggle_key(widget);
        }
      }
      g_free(buf);
    }
    g_free(statusfile);
  }
#else
  {
    gboolean *state;
    GtkIMMulticontext *mim;
    GtkIMContext *im;

    if (GTK_IS_ENTRY(widget)) {
      mim = GTK_IM_MULTICONTEXT(GTK_ENTRY(widget)->im_context);
    } else if (GTK_IS_TEXT_VIEW(widget)) {
      mim = GTK_IM_MULTICONTEXT(GTK_TEXT_VIEW(widget)->im_context);
    } else {
      return;
    }

    if (mim != NULL && !strcmp("ibus",mim->context_id)) {
      im = mim->slave;
      if (im == NULL) {
        return;
      }

      state = (gboolean *)g_object_get_data(G_OBJECT(im),"im-state");
      if (state != NULL) {
        if (*state!=enable) {
          emit_toggle_key(widget);
        }
      }
    }
  }
#endif
}

void
set_im(
  GtkWidget *widget)
{
  _set_im(widget,TRUE);
}

void
unset_im(
  GtkWidget *widget)
{
  _set_im(widget,FALSE);
}

void
set_im_control_enabled(gboolean enabled)
{
  im_control_enabled = enabled;
}

gboolean
get_im_control_enabled(void)
{
  return im_control_enabled;
}
