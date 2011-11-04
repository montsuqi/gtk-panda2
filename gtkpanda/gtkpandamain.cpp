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
#define XPCOM_GLUE
#include "gtkmozembed_glue.cpp"

G_BEGIN_DECLS
void init_gtkmozembed();
void gtk_panda_init (int *argc, char ***argv);
G_END_DECLS

void
init_gtkmozembed()
{
  char *profile_path;
  static const GREVersionRange greVersion = {
  "1.9.0", PR_TRUE,
  "1.9.3", PR_FALSE
  };
  char xpcomLocation[4096];

  nsresult rv = GRE_GetGREPathWithProperties(&greVersion, 1, nsnull, 0, xpcomLocation, 4096);
  if (NS_FAILED(rv)) {
  printf("GRE_GetGREPathWithProperties failed\n");
  return;
  }

  // Startup the XPCOM Glue that links us up with XPCOM.
  XPCOMGlueStartup(xpcomLocation);
  if (NS_FAILED(rv)) {
      printf("XPCOMGlueStartup failed\n");
      return;
  }

  rv = GTKEmbedGlueStartup();
  if (NS_FAILED(rv)) {printf("location: %s \n", xpcomLocation);
      printf("GTKEmbedGlueStartup failed\n");
      return;
  }

  rv = GTKEmbedGlueStartupInternal();
  if (NS_FAILED (rv)) {
      printf ("GTKEmbedGlueStartupInternal failed\n");
      return;
  }

  //gtk_moz_embed_set_comp_path(xpcomLocation);

  char *lastSlash = strrchr(xpcomLocation, '/');
  if (lastSlash)
    *lastSlash = '\0';

  gtk_moz_embed_set_path(xpcomLocation);
  gtk_moz_embed_push_startup ();

  profile_path = g_build_filename(g_get_home_dir(),
    ".gtkpanda2/mozilla", NULL);
  gtk_moz_embed_set_profile_path(profile_path, "gtkpanda2");
  g_free(profile_path);
}

void
gtk_panda_init (int *argc, char ***argv)
{
#ifdef ENABLE_NLS
  bindtextdomain("gtkpanda2", LOCALEDIR);
#endif
  init_gtkmozembed();
}
