#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(gtk+panda, 1.9.0)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([gtkpanda/gtkpandaentry.c])
AC_CONFIG_HEADER([config.h])
AC_CANONICAL_HOST
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC
if test "$GCC" = "yes"; then
#       CFLAGS="$CFLAGS -O4 -Wall -Wunused -Wredundant-decls -funsigned-char"
        CFLAGS="-g -O4 -Wall -Wunused -funsigned-char -Wno-pointer-sign"
fi
AC_GNU_SOURCE
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PATH_PROG(GS_PROG, gs)
AC_DEFINE_UNQUOTED(GS_PATH, "$GS_PROG", "")
AC_PATH_PROG(GNOME_MOZ_REMOTE_PROG, gnome-moz-remote, "")
AC_DEFINE_UNQUOTED(OPEN_BROWSER_COMMAND, "$GNOME_MOZ_REMOTE_PROG", "")
AC_PATH_PROG(GS_PROG, gs)
AC_DEFINE_UNQUOTED(GS_PATH, "$GS_PROG", [Path to the 'gs' executable.])

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

AC_SUBST(LIBGNOME_REQUIRED, 2.0.0)
AC_SUBST(GTK_REQUIRED, 2.0.0)
AC_SUBST(GLIB_REQUIRED, 2.0.0)
AC_SUBST(XUL_REQUIRED, 1.8.0.0)
AC_SUBST(POPPLER_REQUIRED, 0.8.0)

PKG_CHECK_MODULES(GTK_PANDA,
[
	libgnomeui-2.0 	>=	$LIBGNOME_REQUIRED
	glib-2.0		>=	$GLIB_REQUIRED
	gtk+-2.0 		>=	$GTK_REQUIRED
    xulrunner-gtkmozembed >= $XUL_REQUIRED
])

dnl ***************************************************************************
dnl use popler
dnl ***************************************************************************

AC_ARG_ENABLE(popplertest,
 [ --enable-poppler enable poppler [[disable]]],,[enable_poppler="no"] )

if test "x$enable_poppler" = "xyes"; then
  PKG_CHECK_MODULES(POPPLER,
  [
   poppler-glib >= $POPPLER_REQUIRED
  ])
fi
AM_CONDITIONAL(POPPLER, test "x$enable_poppler" = "xyes")

AC_CONFIG_FILES([Makefile docs/Makefile gtkpanda/Makefile gtkpanda2.pc intl/Makefile po/Makefile.in test/Makefile ])
AC_OUTPUT
