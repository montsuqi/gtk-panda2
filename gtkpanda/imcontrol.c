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
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>

#include "config.h"
#include "imcontrol.h"

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

void 
set_im_state_pre_focus(
  GtkWidget *widget, 
  GtkIMMulticontext *mim,
  gboolean enabled)
{
  GtkIMContext *im;
  gboolean *state;

  if (!strcmp("scim-bridge", mim->context_id)) {
    im = mim->slave;
    state = (gboolean *)g_object_get_data(G_OBJECT(im), "im-state");
    if (state != NULL) {
      if (*state != enabled) {
          emit_toggle_key(widget, im);
      }
    }
  }
}

void 
set_im_state_post_focus(
  GtkWidget *widget, 
  GtkIMMulticontext *mim,
  gboolean enabled)
{
  GtkIMContextXIM *xim;
  GtkIMContext *im;
  gboolean *state;

  if (!strcmp("xim", mim->context_id)) {
#if 0
    xim = (GtkIMContextXIM *)mim->slave;
    if (xim == NULL || xim->ic == NULL) {
      return;
    }
    if (enabled)
    {
      XVaNestedList *preedit_attr =
        XVaCreateNestedList (0,
          XNPreeditState, XIMPreeditEnable,
          NULL);
      XSetICValues (xim->ic,
        XNPreeditAttributes, preedit_attr, NULL);
      XFree(preedit_attr);
    } else {
      XVaNestedList *preedit_attr =
        XVaCreateNestedList (0,
          XNPreeditState, XIMPreeditDisable,
          NULL);
      XSetICValues (xim->ic,
        XNPreeditAttributes, preedit_attr, NULL);
      XFree(preedit_attr);
    }
#endif
  } else if (!strcmp("ibus", mim->context_id)) {
    im = mim->slave;
    state = (gboolean *)g_object_get_data(G_OBJECT(im), "im-state");
    if (state != NULL) {
      if (*state != enabled) {
          emit_toggle_key(widget, im);
      }
    }
  }
}
