/* pandacombo - combo widget for gtk+
 * Copyright 1997 Paolo Molaro
 * Copyright 2001 AXE, Inc.
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
 *
 * Based on gtk/gtkcombo.h
 */

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#ifndef __GTK_PANDA_COMBO_H__
#define __GTK_PANDA_COMBO_H__

#include <gtk/gtkcombobox.h>
#include "gtkpandaentry.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_PANDA_TYPE_COMBO			(gtk_panda_combo_get_type ())
#define GTK_PANDA_COMBO(obj)			GTK_CHECK_CAST (obj, gtk_panda_combo_get_type (), GtkPandaCombo)
#define GTK_PANDA_COMBO_CLASS(klass)	GTK_CHECK_CLASS_CAST (klass, gtk_panda_combo_get_type (), GtkPandaComboClass)
#define GTK_IS_PANDA_COMBO(obj)			GTK_CHECK_TYPE (obj, gtk_panda_combo_get_type ())

typedef struct _GtkPandaCombo		GtkPandaCombo;
typedef struct _GtkPandaComboClass	GtkPandaComboClass;

/* you should access only the entry and list fields directly */
struct _GtkPandaCombo {
  GtkComboBox box;

  gboolean case_sensitive:1;
  gboolean use_arrows:1;
  gboolean loop_selection:1;

  GtkWidget *entry;
};

struct _GtkPandaComboClass {
  GtkComboBoxClass parent_class;

  /* Action signals
   */
  void (* activate)  (GtkPandaCombo *combo);
};

GType gtk_panda_combo_get_type(void);

GtkWidget *gtk_panda_combo_new(void);

/* sett arrows working for changing the value (can be annoying */
void gtk_panda_combo_set_use_arrows(
       GtkPandaCombo* combo, 
       gboolean enabled);

/* up/arrows change value if current value not in list */
void gtk_panda_combo_set_loop_selection(
       GtkPandaCombo* combo, 
       gboolean enabled);

/* percase-sensitive compares */
void gtk_panda_combo_set_case_sensitive(
       GtkPandaCombo* combo, 
       gboolean enabled);

/* simnterface */
void gtk_panda_combo_set_popdown_strings(
       GtkPandaCombo* combo, 
       GList *strings);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_PANDA_COMBO_H__ */
