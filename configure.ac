#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(hagraph, 0.2.0, bjoern@bjoern-b.de)
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE(hagraph, 0.2.0)

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_INT16_T
AC_TYPE_INT8_T
AC_STRUCT_TM
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([floor memset strchr strstr])

#gtk
PKG_CHECK_MODULES([gtk], gtk+-2.0 >= 2.12)
AC_SUBST(gtk_LIBS)
AC_SUBST(gtk_CFLAGS)

#libhac
PKG_CHECK_MODULES([libhagraph], libhagraph >= 0.2.0)
AC_SUBST(libhagraph_LIBS)
AC_SUBST(libhagraph_CFLAGS)

win32=no
case $host in
  *-*-mingw32* | *-*-windows)
    win32=yes
    EXTRA_CFLAGS="$EXTRA_CFLAGS -mno-cygwin -mwindows -mms-bitfields"
    EXTRA_LDFLAGS="-mwindows -mms-bitfields" ;;
esac

AM_CONDITIONAL(WIN32, test x$win32 = xyes)


AC_OUTPUT(Makefile src/Makefile)

