/* gtkpanda - set of widgets for gtk+
 * Copyright 2001  Masami Ogoshi <ogochan@linux.or.jp>
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

#ifndef GTK_PANDA_H
#define GTK_PANDA_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>

#include <gtkpanda/gtknumberentry.h>
#include <gtkpanda/gtkpandaclist.h>
#include <gtkpanda/gtkpandacombo.h>
#include <gtkpanda/gtkpandaentry.h>
#include <gtkpanda/gtkpandafileentry.h>
#include <gtkpanda/gtkpandahtml.h>
#include <gtkpanda/gtkpandaps.h>
#include <gtkpanda/gtkpandatext.h>
#include <gtkpanda/gtkpandatimer.h>
#include <gtkpanda/gtkpandadownload.h>
#ifdef USE_PDF
#	include <gtkpanda/gtkpandapdf.h>
#endif

void gtk_panda_init (int *argc, char ***argv);

#endif /* GTK_PANDA_H */
