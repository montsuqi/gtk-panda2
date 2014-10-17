#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(gtk+panda, 2.0.5, [http://www.montsuqi.org/], [gtk+panda2])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([gtkpanda/gtkpandaentry.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_HOST
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
if test "$GCC" = "yes"; then
#       CFLAGS="$CFLAGS -O4 -Wall -Wunused -Wredundant-decls -funsigned-char"
        CFLAGS="-g -O4 -Wall -Wunused -funsigned-char -Wno-pointer-sign"
fi
AC_GNU_SOURCE
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PATH_PROG(GNOME_MOZ_REMOTE_PROG, gnome-open, "")
AC_DEFINE_UNQUOTED(OPEN_BROWSER_COMMAND, "$GNOME_MOZ_REMOTE_PROG", "")

# Checks for libraries.
AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION(0.16.1)

# Checks for header files.
AC_PATH_X
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h float.h locale.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_CHECK_FUNCS([dup2 memset putenv setlocale strcasecmp strchr strdup strrchr strtol])

AC_PATH_PROG(GLIB_GENMARSHAL, glib-genmarshal)


PKG_CHECK_MODULES(GTK_PANDA,
[
	glib-2.0					>=	2.0.0
	gtk+-2.0 					>=	2.0.0
	poppler-glib 				>=	0.6.4
	webkit-1.0					>=	1.2.5
])

dnl ***************************************************************************
dnl poppler version check
dnl ***************************************************************************
PKG_CHECK_MODULES(CHECK_POPPLER_VER,poppler-glib >= 0.8.0,
[
	AC_DEFINE(POPPLER_0_8,1,"")
], [])

dnl ***************************************************************************
dnl ibus version check
dnl ***************************************************************************
PKG_CHECK_MODULES(CHECK_IBUS_VER,ibus-1.0 >= 1.5.5,
[
	AC_DEFINE(IBUS_1_5,1,"")
], 
[
	PKG_CHECK_MODULES(CHECK_IBUS_VER,ibus-1.0 >= 1.4.0,
	[])
])



AC_CONFIG_FILES([	Makefile 
					docs/Makefile 
					gtkpanda/Makefile 
					gtkpanda2.pc 
					intl/Makefile 
					po/Makefile.in ])
AC_OUTPUT
