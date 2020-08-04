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
static void gtk_panda_html_set_proxy(GtkPandaHTML *html,const gchar *uri);
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

static void
gtk_panda_html_init (GtkPandaHTML *self)
{
  self->scroll = gtk_scrolled_window_new(NULL,NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->scroll),
    GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
  
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
gtk_panda_html_set_proxy(
	GtkPandaHTML *html,
	const gchar *uri)
{
  int i,fnoproxy = 0;
  gchar *proxy,*noproxy,**hosts;

  proxy   = getenv("http_proxy");
  noproxy = getenv("no_proxy");

  if (proxy == NULL) {
    return;
  }

  if (noproxy != NULL) {
    hosts = g_strsplit(noproxy,",",-1);
    for (i=0;hosts[i]!=NULL;i++) {
      if (g_strstr_len(uri,-1,hosts[i]) != NULL) {
        fnoproxy = 1;
      }
    }
    g_strfreev(hosts);
  }

#if 0
  if (fnoproxy) {
    g_object_set(webkit_get_default_session(),SOUP_SESSION_PROXY_URI,NULL,NULL);
  } else {
    SoupURI *soup_uri = soup_uri_new(proxy);
    g_object_set(webkit_get_default_session(),SOUP_SESSION_PROXY_URI,soup_uri,NULL);
    soup_uri_free(soup_uri);
  }
#endif
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
  return "";
}

void
gtk_panda_html_set_uri (GtkPandaHTML *self, const gchar *uri)
{
  if (getenv("GTK_PANDA_HTML_DISABLE") == NULL) {
    gtk_panda_html_set_proxy(self,uri);
  }
}
