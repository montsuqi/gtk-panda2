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

#include "gtkpandaintl.h"
#include "glib.h"
#include "gtkmozembed.h"

void
gtk_panda_init (int *argc, char ***argv)
{
  char *profile_path;
#ifdef ENABLE_NLS
  bindtextdomain("gtkpanda2", LOCALEDIR);
#endif
  profile_path = g_build_filename(g_get_home_dir(),
    ".gtkpanda2/mozilla", NULL);
  gtk_moz_embed_set_profile_path(profile_path, "gtkpanda2");
  g_free(profile_path);
}
