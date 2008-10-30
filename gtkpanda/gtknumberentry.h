/* GtkItemEntry - widget for gtk+
 * Copyright (C) 1999-2001 Adrian E. Feiguin <adrian@ifir.ifir.edu.ar>
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * GtkItemEntry widget by Adrian E. Feiguin
 * Based on GtkEntry widget 
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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef __GTK_NUMBER_ENTRY_H__
#define __GTK_NUMBER_ENTRY_H__

#include	<glib-object.h>
#include	<gtk/gtkentry.h>
#include	<gtkpanda/numeric.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GTK_TYPE_NUMBER_ENTRY \
  (gtk_number_entry_get_type ())
#define GTK_NUMBER_ENTRY(obj) \
  (GTK_CHECK_CAST (obj, gtk_number_entry_get_type (), GtkNumberEntry))
#define GTK_NUMBER_ENTRY_CLASS(klass) \
  (GTK_CHECK_CLASS_CAST (klass,	\
    gtk_number_entry_get_type (), \
    GtkNumberEntryClass))
#define GTK_IS_NUMBER_ENTRY(obj) \
  (GTK_CHECK_TYPE (obj, gtk_number_entry_get_type ()))
#define GTK_IS_NUMBER_ENTRY_CLASS(klass) \
  (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ENTRY))

typedef struct _GtkNumberEntry       GtkNumberEntry;
typedef struct _GtkNumberEntryClass  GtkNumberEntryClass;

struct _GtkNumberEntry
{
  GtkEntry parent;
  
  char *format;
  Numeric value;
  int scale;
  int expo;
};

struct _GtkNumberEntryClass
{
  GtkEntryClass parent_class;
};

GType      gtk_number_entry_get_type       (void);
GtkWidget* gtk_number_entry_new            (void);
GtkWidget* gtk_number_entry_new_with_max_length (guint16   max);
GtkWidget* gtk_number_entry_new_with_format (char *fmt);

void    gtk_number_entry_set_value(GtkNumberEntry *number_entry, 
                                   Numeric value);
Numeric	gtk_number_entry_get_value(GtkNumberEntry *number_entry);
void    gtk_number_entry_set_format(GtkNumberEntry *number_entry,
                                    const gchar *fmt);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_NUMBER_ENTRY_H__ */
