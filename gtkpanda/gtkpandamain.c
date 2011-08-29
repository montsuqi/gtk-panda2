/* gtkpandainit - set of widgets for gtk+
 *
 * Copyright (C) 2001 AXE,Inc.
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

#include <gtk/gtk.h>
#include "gtkpandaintl.h"
#include "glib.h"

void gtk_panda_init (int *argc, char ***argv);

void
gtk_panda_init (int *argc, char ***argv)
{
  GtkSettings *set = gtk_settings_get_default();
  gtk_settings_set_long_property(set, "gtk-entry-select-on-focus", 0, 
  "gtk+panda2");
#ifdef ENABLE_NLS
  bindtextdomain("gtkpanda2", LOCALEDIR);
#endif
}
