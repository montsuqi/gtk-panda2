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

enum
{   
  ACTIVATE,
  LAST_SIGNAL
};

static GtkMozEmbedClass *parent_class = NULL;
static guint signals [LAST_SIGNAL] = { 0 };

static void gtk_panda_html_class_init    (GtkPandaHTMLClass *klass);
static void gtk_panda_html_init          (GtkPandaHTML     *html);
static gboolean gtk_panda_html_open_uri(GtkMozEmbed *embed,
  const char *uri, gpointer data);
static void gtk_panda_html_new_window(GtkMozEmbed *embed,
  GtkMozEmbed **retval, guint chromemask, gpointer data);
#if 0
static void gtk_panda_html_reload(GtkWidget *widget,
  GdkEventExpose *event, gpointer user_data);
#endif

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

      type = g_type_register_static( GTK_TYPE_MOZ_EMBED,
                                     "GtkPandaHTML",
                                     &info,
                                     0);
    }

  return type;
}

static void
gtk_panda_html_class_init (GtkPandaHTMLClass *class)
{
  GObjectClass *object_class;
  GtkObjectClass *gtk_object_class;
  GtkWidgetClass *widget_class;

  object_class = G_OBJECT_CLASS(class);
  gtk_object_class = (GtkObjectClass*) class;
  widget_class = (GtkWidgetClass*) class;
  parent_class = gtk_type_class (GTK_TYPE_MOZ_EMBED);

  signals[ACTIVATE] =
  g_signal_new ("activate",
        G_TYPE_FROM_CLASS (object_class),
        G_SIGNAL_RUN_LAST,
        G_STRUCT_OFFSET (GtkPandaHTMLClass, activate),
        NULL, NULL,
        gtk_marshal_VOID__POINTER,
        G_TYPE_NONE, 1,
        G_TYPE_POINTER);
}

static void
gtk_panda_html_init (GtkPandaHTML *html)
{
  g_signal_connect(GTK_MOZ_EMBED(html), "open-uri",
    G_CALLBACK(gtk_panda_html_open_uri), NULL);
  g_signal_connect(GTK_MOZ_EMBED(html), "new-window",
    G_CALLBACK(gtk_panda_html_new_window), NULL);
#if 0
  g_signal_connect(GTK_WIDGET(html), "expose-event",
    G_CALLBACK(gtk_panda_html_reload), NULL);
#endif
}

GtkWidget*
gtk_panda_html_new (void)
{
  return GTK_WIDGET (g_object_new (GTK_PANDA_TYPE_HTML, NULL));
}

static gboolean
gtk_panda_html_open_uri(GtkMozEmbed *embed,
  const char *uri,
  gpointer data)
{
  const char *scheme = "https";
  if (!strncmp(scheme, uri, strlen(scheme))) {
     return TRUE;
  }
  return FALSE;
}

static void
gtk_panda_html_new_window(GtkMozEmbed *embed,
  GtkMozEmbed **retval,
  guint chromemask,
  gpointer data)
{
  char *argv[3];
  int pid;
  extern char **environ;

  *retval = NULL;
  if ((pid = fork()) == 0) {
    if (strlen(OPEN_BROWSER_COMMAND) > 0) {
      argv[0] = OPEN_BROWSER_COMMAND;
      argv[1] = gtk_moz_embed_get_link_message(embed);
      argv[2] = NULL;
      execve(argv[0], argv, environ);
    } else {
      g_warning("cannot open external browser.");
      exit(0);
    }
  } else if (pid < 0) {
    g_warning("fork failed");
  } 
  return;
}

#if 0
gtk_panda_html_reload(GtkWidget *widget,
  GdkEventExpose *event,
  gpointer user_data)
{
  gtk_moz_embed_reload(GTK_MOZ_EMBED(widget), GTK_MOZ_EMBED_FLAG_RELOADNORMAL);
  fprintf(stderr,"reloaded %s\n", gtk_moz_embed_get_location(GTK_MOZ_EMBED(widget)));
  //gtk_widget_show_all(widget);
  return;
}
#endif

// public API

char *
gtk_panda_html_get_uri (GtkPandaHTML *html)
{
  return gtk_moz_embed_get_location(GTK_MOZ_EMBED(html));
}

void
gtk_panda_html_set_uri (GtkPandaHTML *html, const char *uri)
{
  gtk_moz_embed_load_url(GTK_MOZ_EMBED(html), uri);
}
