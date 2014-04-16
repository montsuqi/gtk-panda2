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
#include <unistd.h>
#include <sys/types.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

#include "config.h"
#include "imcontrol.h"

static gboolean im_control_enabled = TRUE;

static void
emit_toggle_key(GtkWidget *widget,
  GtkIMContext *im)
{
  GdkEvent *kevent;

  kevent = gdk_event_new(GDK_KEY_PRESS);
  kevent->key.window = gtk_widget_get_parent_window(widget);
  kevent->key.send_event = 0;
  kevent->key.time = gdk_x11_get_server_time(kevent->key.window);
  kevent->key.state = 16;
  kevent->key.length = 0;
  kevent->key.string = "";
  kevent->key.keyval = GDK_Zenkaku_Hankaku;
  gtk_im_context_filter_keypress(im, (GdkEventKey *)kevent);
}

static void 
_set_im(
  GtkWidget *widget, 
  GtkIMMulticontext *mim,
  gboolean enable)
{
  GtkIMContext *im;

  if (!im_control_enabled) {
    return;
  }

  if (mim != NULL && !strcmp("ibus", mim->context_id)) {
    im = mim->slave;
    if (mim->slave == NULL) {
      return;
    }
#ifdef IBUS_1_5
    {
      gchar *statusfile,*buf;
      size_t size;
      int uid;

      uid = (int)getuid();
      statusfile = g_strdup_printf("/tmp/ibus-im-status.%d.txt",uid);
      if (g_file_get_contents(statusfile,&buf,&size,NULL)) {
        if (enable) {
          if (size > 0 && buf[0] == '0') {
            emit_toggle_key(widget, im);
          }
        } else {
          if (size > 0 && buf[0] == '1') {
            emit_toggle_key(widget, im);
          }
        }
        g_free(buf);
      }
      g_free(statusfile);
    }
#else
    {
      gboolean *state;

      state = (gboolean *)g_object_get_data(G_OBJECT(im), "im-state");
      if (state != NULL) {
        if (*state) {
          emit_toggle_key(widget, im);
        }
      }
    }
#endif
  }
}

void
set_im(
  GtkWidget *widget, 
  GtkIMMulticontext *mim)
{
  _set_im(widget,mim,TRUE);
}

void
unset_im(
  GtkWidget *widget, 
  GtkIMMulticontext *mim)
{
#ifdef IBUS_1_5
  _set_im(widget,mim,FALSE);
#endif
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
