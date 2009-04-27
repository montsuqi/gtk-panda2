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
#include <gtk/gtksignal.h>
#include <gtk/gtkbindings.h>
#include <gtk/gtkmarshal.h>
#include <gdk/gdkkeysyms.h>

#include "gtkpandaintl.h"
#include "gtkpandafileentry.h"

static void gtk_panda_fileentry_class_init    (GtkPandaFileentryClass *klass);
static void gtk_panda_fileentry_init          (GtkPandaFileentry      *fileentry);
static void gtk_panda_fileentry_destroy       (GtkObject      *fileentry);

static GtkHBoxClass *parent_class = NULL;

enum
{
  BROWSE_CLICKED,
  DONE_ACTION,
  LAST_SIGNAL
};

static guint signals [LAST_SIGNAL] = { 0 };

void gtk_panda_fileentry_browse_clicked(GtkPandaFileentry *fe);

GType
gtk_panda_fileentry_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      static const GTypeInfo info =
      {
        sizeof (GtkPandaFileentryClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_panda_fileentry_class_init,
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkPandaFileentry),
        0,
        (GInstanceInitFunc) gtk_panda_fileentry_init
      };

      type = g_type_register_static( GTK_TYPE_HBOX, 
        "GtkPandaFileentry", &info, 0);
    }

  return type;
}

static void
gtk_panda_fileentry_class_init (GtkPandaFileentryClass *klass)
{
  GtkWidgetClass *widget_class;
  GtkObjectClass *gtk_object_class;

  gtk_object_class = (GtkObjectClass *) klass;
  widget_class = (GtkWidgetClass *) klass;
  parent_class = gtk_type_class (GTK_TYPE_HBOX);

  gtk_object_class->destroy = gtk_panda_fileentry_destroy;
  klass->browse_clicked = gtk_panda_fileentry_browse_clicked;

  signals[BROWSE_CLICKED] =
  g_signal_new ("browse_clicked",
        G_OBJECT_CLASS_TYPE (klass),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaFileentryClass, browse_clicked),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

  signals[DONE_ACTION] =
  g_signal_new ("done_action",
        G_OBJECT_CLASS_TYPE (klass),
        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
        G_STRUCT_OFFSET (GtkPandaFileentryClass, done_action),
        NULL, NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);
}

static void
gtk_panda_fileentry_destroy(GtkObject *object)
{
  GTK_OBJECT_CLASS(parent_class)->destroy(object);
}

void
gtk_panda_fileentry_browse_clicked(GtkPandaFileentry *self)
{
	g_signal_emit_by_name(G_OBJECT(self->button), "clicked", NULL);
}

static void
entry_activate_cb(GtkEntry *entry, GtkPandaFileentry *self)
{
  // do nothing
}

static void
button_click_cb(GtkButton *button, GtkPandaFileentry *self)
{
  GError *error = NULL;
  GtkWidget *dialog;
  char *filename;
  char *basename;
  const gchar *label;
  GtkWidget *mdialog;

  if (self->mode == GTK_FILE_CHOOSER_ACTION_OPEN) {
	label = GTK_STOCK_OPEN;
  } else {
	label = GTK_STOCK_SAVE;
  }
  
  dialog = gtk_file_chooser_dialog_new (_("Specify filename..."),
    (GtkWindow *)gtk_widget_get_toplevel(GTK_WIDGET(self)),
    self->mode,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    label, GTK_RESPONSE_ACCEPT,
    NULL);
  gtk_file_chooser_set_do_overwrite_confirmation (
    GTK_FILE_CHOOSER (dialog), TRUE);

  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (dialog), self->folder);
  basename = g_path_get_basename(gtk_entry_get_text(GTK_ENTRY(self->entry)));
  if (self->mode == GTK_FILE_CHOOSER_ACTION_OPEN) {
    filename = g_build_filename(self->folder, basename, NULL);
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filename);
	g_free(filename);
  } else {
    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), basename);
  }
  g_free(basename);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(
      GTK_FILE_CHOOSER (dialog));
    if (self->mode == GTK_FILE_CHOOSER_ACTION_OPEN) {
      gtk_entry_set_text(GTK_ENTRY(self->entry), filename);
    } else {
      if (self->size > 0 && self->data != NULL) {
        gtk_entry_set_text(GTK_ENTRY(self->entry), filename);
        if (g_file_set_contents(filename,
          self->data, self->size, &error)) {
          mdialog = gtk_message_dialog_new (GTK_WINDOW(dialog),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_CLOSE,
                                    _("Succeeded in writing %s"),
                                    filename);
          gtk_dialog_run (GTK_DIALOG (mdialog));
          gtk_widget_destroy (mdialog);
        } else {
          mdialog = gtk_message_dialog_new (GTK_WINDOW(dialog),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_CLOSE,
                                    "%s",
                                    error->message);
          gtk_dialog_run (GTK_DIALOG (mdialog));
          gtk_widget_destroy (mdialog);
          g_error_free(error);
        }
      }
    }
	if (self->folder != NULL)
		free(self->folder);
    self->folder = g_path_get_dirname(filename);
    g_signal_emit_by_name(G_OBJECT(self->entry), "activate", NULL);
    g_signal_emit(G_OBJECT(self), signals[DONE_ACTION], 0);
    g_free(filename);
  }
  gtk_widget_destroy (dialog);
}

static void
gtk_panda_fileentry_init (GtkPandaFileentry *self)
{
  GtkWidget *hbox;

  self->entry = gtk_entry_new();
  self->button = gtk_button_new();
  self->mode = GTK_FILE_CHOOSER_ACTION_OPEN;
  self->size = 0;
  self->data = NULL;
  self->folder = NULL;

  g_signal_connect(G_OBJECT(self->entry), "activate",
    G_CALLBACK(entry_activate_cb), self);
  g_signal_connect(G_OBJECT(self->button), "clicked",
    G_CALLBACK(button_click_cb), self);

  hbox = gtk_hbox_new(FALSE, 0);

  gtk_box_pack_start(GTK_BOX(self), self->entry, TRUE, TRUE, 2);
  gtk_box_pack_start(GTK_BOX(self), self->button, FALSE, FALSE, 2);

  gtk_box_set_spacing(GTK_BOX(self), 2);
  gtk_box_set_homogeneous(GTK_BOX(self), FALSE);
  
  gtk_widget_show_all(GTK_WIDGET(self));
  GTK_WIDGET_SET_FLAGS (self, GTK_CAN_FOCUS);
}

// public API
GtkWidget *
gtk_panda_fileentry_new (void)
{
  return g_object_new (GTK_PANDA_TYPE_FILEENTRY, NULL);
}

void
gtk_panda_fileentry_set_mode (GtkPandaFileentry *self, GtkFileChooserAction mode)
{
  self->mode = mode;
  gtk_button_set_use_stock(GTK_BUTTON(self->button), TRUE);
  
  switch(mode) {
  case GTK_FILE_CHOOSER_ACTION_OPEN:
  case GTK_FILE_CHOOSER_ACTION_SAVE:
    gtk_button_set_label(GTK_BUTTON(self->button), GTK_STOCK_OPEN);
	break;
  default:
    fprintf(stderr, "invalid mode:%d\n", mode);
    gtk_button_set_label(GTK_BUTTON(self->button), GTK_STOCK_STOP);
	break;
  }
}

GtkFileChooserAction
gtk_panda_fileentry_get_mode (GtkPandaFileentry *self)
{
  return self->mode;
}

void
gtk_panda_fileentry_set_data (GtkPandaFileentry *self, 
  int size, 
  char *data)
{
  if (self->data != NULL) {
    g_free(self->data);
    self->size = 0;
  }

  if (size > 0 && data != NULL) {
    self->size = size;
    self->data = (char *)g_memdup(data, size);
  } else {
    self->size = 0;
    self->data = NULL;
  }
}

void
gtk_panda_fileentry_set_folder (GtkPandaFileentry *self, 
  char *str)
{
  if (self->folder != NULL) {
    g_free(self->folder);
  }

  if (str != NULL) {
    self->folder = g_strdup(str);
  }
}


void
gtk_panda_fileentry_get (GtkPandaFileentry * self,
  int *size,
  char **data)
{
  if (self->size > 0 && self->data != NULL) {
    *size = self->size;
    *data = (char *)g_memdup(self->data, self->size);
  } else {
    *size = 0;
    *data = NULL;
  }
}
