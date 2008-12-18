#ifndef __GTKPANDAINTL_H__
#define __GTKPANDAINTL_H__

#include "config.h"

#ifdef ENABLE_NLS
#  include<libintl.h>
#  define _(String) dgettext("gtkpanda2",(String))
#  ifdef gettext_noop
#    define N_(String) gettext_noop(String)
#  else
#    define N_(String) (String)
#  endif
#else /* NLS is disabled */
#  define _(String) (String)
#  define N_(String) (String)
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,String) (String)
#  define dcgettext(Domain,String,Type) (String)
#  define bindtextdomain(Domain,Directory) (Domain) 
#endif

#endif