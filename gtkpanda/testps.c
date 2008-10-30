#include <stdlib.h>
#include <gtk/gtk.h>
#include "gtkpanda.h"
#include "gtkpandaps.h"
#include <locale.h>
#include <libintl.h>

GtkWidget *pps;

extern	void	*
_xmalloc(
	size_t	size,
	char	*fn,
	int		line)
{
	return	(malloc(size));
}
extern	void
_xfree(
	void	*p,
	char	*fn,
	int		line)
{
	free(p);
}

static GtkWidget *
create_window (void)
{
  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_policy (GTK_WINDOW (window), TRUE, TRUE, FALSE);
  gtk_window_set_default_size(GTK_WINDOW (window), 200, 200);
  pps = gtk_panda_ps_new ();
  gtk_widget_show (pps);
  gtk_container_add (GTK_CONTAINER (window), pps);
  return window;
}

int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      puts ("Usage: testps file.ps");
      exit (1);
    }
  
  setlocale(LC_MESSAGES, "");  
  setlocale(LC_CTYPE, ""); 

  gtk_init (&argc, &argv);
  gtk_panda_init (&argc, &argv);

  gtk_widget_show (create_window ());
  gtk_panda_ps_load (GTK_PANDA_PS (pps), argv[1]);

  gtk_main ();
  return 0;
}
