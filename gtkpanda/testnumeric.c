#define	DEBUG
#define	TRACE
/*
*/
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "numeric.h"
#include "gtkpanda.h"
#include "debug.h"

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

extern	void
activate(
	GtkNumberEntry	*entry,
	gpointer		user_data)
{
	Numeric	value;
	char	*str;

	value = gtk_number_entry_get_value(entry);
	str =  NumericOutput(value);
	printf("activate [%s]\n",str);
	NumericFree(value);
	free(str);
}

extern	void
changed(
	GtkNumberEntry	*entry,
	gpointer		user_data)
{
	Numeric	value;
	char	*str;

	value = gtk_number_entry_get_value(entry);
	str =  NumericOutput(value);
	printf("changed [%s]\n",str);
	NumericFree(value);
	free(str);
}

extern	int
main(
	int		argc,
	char	**argv)
{
	GtkWidget	*window;
	GtkWidget	*entry;
	GtkStyle	*st
	,			*dst;
	Numeric		value;

	gtk_init(&argc, &argv);

	dst = gtk_widget_get_default_style();
	st = gtk_style_copy(dst);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(G_OBJECT(window),"destroy",
	   G_CALLBACK(gtk_main_quit), NULL);
	entry = gtk_number_entry_new_with_format("----,---.99");
	g_signal_connect(G_OBJECT(entry), "activate",
	   G_CALLBACK(activate), NULL);
	g_signal_connect(G_OBJECT(entry), "changed",
	   G_CALLBACK(changed), NULL);
	if (argc >= 2){
		value = NumericInput(argv[1],9,0);
	} else {
		value = NumericInput("7.0",9,0);		
	}

dbgmsg("*");
	gtk_number_entry_set_value(GTK_NUMBER_ENTRY(entry),value);
dbgmsg("**");
	NumericFree(value);
dbgmsg("***");

	gtk_container_add(GTK_CONTAINER(window),entry);
dbgmsg("****");

	gtk_widget_show(entry);
dbgmsg("*****");
	gtk_widget_show(window);
dbgmsg("******");

	gdk_color_parse ("red", &(st->text[GTK_STATE_NORMAL]));
	gtk_widget_set_style(entry, st);

	value = NumericInput("99999.0",9,0);		
	gtk_number_entry_set_value(GTK_NUMBER_ENTRY(entry),value);
	NumericFree(value);

	gtk_main();

	return	0;
}
