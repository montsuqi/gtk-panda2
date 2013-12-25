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
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

#include "config.h"
#include "imcontrol.h"

#ifdef USE_DBUS
#include <ibus.h>
#include <dbus/dbus.h>

static GDBusConnection *
get_ibus_connect(void)
{
  GDBusConnection *connect = NULL;

  if ( ibus_get_address() != NULL) {
    connect = g_dbus_connection_new_for_address_sync (ibus_get_address (),
						      G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT |
						      G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION,
						      NULL, NULL, NULL);
  }
  return connect;
}

static gchar *
get_ibus_CurrentInputContext(
			     GDBusConnection *connect)
{
  gchar *current = NULL;
  GVariant *result;

  result = g_dbus_connection_call_sync(connect,
					IBUS_SERVICE_IBUS,
					IBUS_PATH_IBUS,
					IBUS_INTERFACE_IBUS,
					"CurrentInputContext",
					NULL,
					NULL,
					G_DBUS_CALL_FLAGS_NO_AUTO_START,
					-1,
					NULL,
					NULL);
  if (result != NULL) {
    g_variant_get(result,"(o)", &current);
    g_variant_unref (result);
  }
  return current;
}

static gboolean
is_ibus_enable(
	       GDBusConnection *connect,
	       gchar *current)
{
  gboolean bool;
  GVariant *result;

  result = g_dbus_connection_call_sync (connect,
					IBUS_SERVICE_IBUS,
					current,
					IBUS_INTERFACE_INPUT_CONTEXT,
					"IsEnabled",
					NULL,
					NULL,
					G_DBUS_CALL_FLAGS_NO_AUTO_START,
					-1,
					NULL,
					NULL);
  if (result != NULL) {
    g_variant_get(result,"(b)", &bool);
    g_variant_unref (result);
  } else {
    return FALSE;
  }
  return bool;
}

static void
ibus_change_state(
		  GDBusConnection *connect,
		  gchar *current,
		  const gchar *state)
{
  GVariant *result;
  result = g_dbus_connection_call_sync (connect,
					IBUS_SERVICE_IBUS,
					current,
					IBUS_INTERFACE_INPUT_CONTEXT,
					state,
					NULL,
					NULL,
					G_DBUS_CALL_FLAGS_NO_AUTO_START,
					-1,
					NULL,
					NULL);
  if (result != NULL) {
    g_variant_unref (result);
  }
}

#endif

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
enable_im(void)
{
#ifdef USE_DBUS
  GDBusConnection *connect = NULL;
  gchar *current = NULL;

  connect = get_ibus_connect();
  if (connect == NULL) {
    return;
  }
  current = get_ibus_CurrentInputContext(connect);
  if (current == NULL) {
    return;
  }
  ibus_change_state(connect, current, "Enable");
  g_dbus_connection_close_sync(connect,NULL,NULL);
  g_object_unref(connect);
#endif
}

void 
set_im_state_post_focus(
  GtkWidget *widget, 
  GtkIMMulticontext *mim,
  gboolean enabled)
{
#ifdef USE_DBUS
  if (enabled) {
    enable_im();
  }
#else
  GtkIMContext *im;
  gboolean *state;

  if (!strcmp("ibus", mim->context_id)) {
    im = mim->slave;
    state = (gboolean *)g_object_get_data(G_OBJECT(im), "im-state");
    if (state != NULL) {
      if (*state != enabled) {
          emit_toggle_key(widget, im);
      }
    }
  }
#endif
}
