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
#include <sys/types.h>
#include <unistd.h>
#include <glib-object.h>
#include <gtk/gtk.h>

#include "gtkpandaintl.h"
#include "gtkpandahtml.h"
#include "pandamarshal.h"

enum
{   
  ACTIVATE,
  LAST_SIGNAL
};

static guint signals [LAST_SIGNAL] = { 0 };

enum {
  PROP_0,
  PROP_URI
};


static void gtk_panda_html_class_init(GtkPandaHTMLClass *klass);
static void gtk_panda_html_init(GtkPandaHTML     *html);
static void gtk_panda_html_set_proxy(void);
static void gtk_panda_html_set_property (
  GObject         *object,
  guint           prop_id,
  const GValue    *value,
  GParamSpec      *pspec);
static void gtk_panda_html_get_property (
  GObject         *object,
  guint           prop_id,
  GValue          *value,
  GParamSpec      *psec);

GType
gtk_panda_html_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      static const GTypeInfo info =
      {
        sizeof (GtkPandaHTMLClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_html_class_init,
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaHTML),
        0,
        (GInstanceInitFunc) gtk_panda_html_init
      };

      type = g_type_register_static( GTK_TYPE_VBOX,
                                     "GtkPandaHTML",
                                     &info,
                                     (GTypeFlags)0);
    }

  return type;
}

GType
gtk_panda_h_tml_get_type (void)
{
	return gtk_panda_html_get_type();
}

GType
gtk_panda_ht_ml_get_type (void)
{
	return gtk_panda_html_get_type();
}

GType
gtk_panda_htm_l_get_type (void)
{
	return gtk_panda_html_get_type();
}

GType
gtk_panda_h_tm_l_get_type (void)
{
	return gtk_panda_html_get_type();
}

GType
gtk_panda_ht_m_l_get_type (void)
{
	return gtk_panda_html_get_type();
}

GType
gtk_panda_h_t_m_l_get_type (void)
{
	return gtk_panda_html_get_type();
}

static void 
gtk_panda_html_class_init (GtkPandaHTMLClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS(klass);

  gobject_class->set_property = gtk_panda_html_set_property;
  gobject_class->get_property = gtk_panda_html_get_property;

  g_object_class_install_property (gobject_class,
    PROP_URI,
    g_param_spec_string ("uri",
    _("URI"),
    _("URI"),
    "",
    (GParamFlags)G_PARAM_READWRITE));
  signals[ACTIVATE] =
  g_signal_new ("activate",
        G_TYPE_FROM_CLASS (gobject_class),
        G_SIGNAL_RUN_LAST,
        G_STRUCT_OFFSET (GtkPandaHTMLClass, activate),
        NULL, NULL,
        panda_marshal_VOID__POINTER,
        G_TYPE_NONE, 1,
        G_TYPE_POINTER);
}

static gboolean
cb_new_window(WebKitWebView *webview,
  WebKitWebFrame *frame,
  WebKitNetworkRequest *request,
  WebKitWebNavigationAction action,
  WebKitWebPolicyDecision *decision,
  gpointer user_data)
{
  const char *uri;
  char *argv[3];
  int pid;
  extern char **environ;

  uri = webkit_network_request_get_uri(request);

  if (uri != NULL) {
    if ((pid = fork()) == 0) {
      if (strlen(OPEN_BROWSER_COMMAND) > 0) {
        argv[0] = (char *)OPEN_BROWSER_COMMAND;
        argv[1] = (char *)uri;
        argv[2] = NULL;
        execve(argv[0], argv, environ);
      } else {
        g_error("cannot open external browser.");
      }
    } else if (pid < 0) {
      g_error("fork failed");
    } 
  }
#if 0
  webkit_web_policy_decision_ignore(decision);
#endif

  return TRUE;
}

static void
gtk_panda_html_init (GtkPandaHTML *self)
{
  gtk_panda_html_set_proxy();
  self->webview = webkit_web_view_new();
  g_signal_connect(WEBKIT_WEB_VIEW(self->webview), 
    "new-window-policy-decision-requested",
    G_CALLBACK(cb_new_window), NULL);
  self->scroll = gtk_scrolled_window_new(NULL,NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->scroll),
    GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(self->scroll),
    self->webview);
  
  if (getenv("GTK_PANDA_HTML_DISABLE") == NULL) {
    gtk_box_pack_start(GTK_BOX (self), 
      GTK_WIDGET(self->scroll), TRUE, TRUE, 0);
  }
  gtk_widget_show_all(GTK_WIDGET(self));
}

GtkWidget*
gtk_panda_html_new (void)
{
  return GTK_WIDGET (g_object_new (GTK_PANDA_TYPE_HTML, NULL));
}

static void
gtk_panda_html_set_proxy(void)
{
  char *proxy;

  if ((proxy = getenv("http_proxy")) != NULL) {
    SoupURI *uri = soup_uri_new(proxy);
    g_object_set(webkit_get_default_session(),
      SOUP_SESSION_PROXY_URI,uri,NULL);
    if (uri) {
      soup_uri_free(uri);
    }
  }
}

static void
gtk_panda_html_set_property (
  GObject      *object,
  guint        prop_id,
  const GValue *value,
  GParamSpec   *pspec)
{
  GtkPandaHTML  *html;

  g_return_if_fail(GTK_IS_PANDA_HTML(object));
  html = GTK_PANDA_HTML (object);
  
  switch  (prop_id)  {
    case PROP_URI:
      gtk_panda_html_set_uri(html, g_value_get_string(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gtk_panda_html_get_property (
  GObject      *object,
  guint        prop_id,
  GValue       *value,
  GParamSpec    *pspec)
{
  GtkPandaHTML  *html;

  g_return_if_fail(GTK_IS_PANDA_HTML(object));
  html = GTK_PANDA_HTML (object);
  
  switch (prop_id) {
    case PROP_URI:
      g_value_set_string(value, gtk_panda_html_get_uri(html));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

// public API

gchar *
gtk_panda_html_get_uri (GtkPandaHTML *self)
{
  return (gchar*)webkit_web_view_get_uri(WEBKIT_WEB_VIEW(self->webview));
}

void
gtk_panda_html_set_uri (GtkPandaHTML *self, const gchar *uri)
{
  if (getenv("GTK_PANDA_HTML_DISABLE") == NULL) {
    webkit_web_view_load_uri(
      WEBKIT_WEB_VIEW(self->webview), uri);
  }
}
