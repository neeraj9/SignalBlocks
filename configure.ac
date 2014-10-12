#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
# see LICENSE for license

AC_PREREQ(2.59)
AC_INIT([iblocks], [1.0], [neeraj.sharma@alumni.iitg.ernet.in])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADER([config.h])

AM_PROG_AR

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL

# https://android.googlesource.com/platform/external/stressapptest/+/idea133/configure.ac
# http://lists.gnu.org/archive/html/automake/2000-10/msg00107.html
_os=`uname -s`
AM_CONDITIONAL(OS_DARWIN, [test x"$_os" = xDarwin])

# Checks for libraries.

PKG_CHECK_MODULES([PLPLOT], [plplotd-c++ >= 5.3.1], [use_plplot]="yes", [use_plplot]="no")
if test $use_plplot = "yes"; then
  CXXFLAGS="$PLPLOT_CFLAGS $CXXFLAGS"
  LIBS="$PLPLOT_LIBS $LIBS"
  AC_DEFINE(HAVE_PLPLOT,1,
    [Define if you have the PLPlot libraries and header files.])
else
  AC_MSG_WARN(PLPlot library not found! Plotting deactivated.)
fi

AM_CONDITIONAL(USE_PLPLOT, test $use_plplot = "yes")

PKG_CHECK_MODULES([PLPLOTWX], [plplotd-wxwidgets >= 5.3.1], [use_plplotwx]="yes", [use_plplotwx]="no")
if test $use_plplotwx = "yes"; then
  CXXFLAGS="$PLPLOTWX_CFLAGS $CXXFLAGS"
  LIBS="$PLPLOTWX_LIBS $LIBS"
  AC_DEFINE(HAVE_PLPLOTWX,1,
    [Define if you have the PLPlot WxWidgets libraries and header files.])
else
  AC_MSG_WARN(PLPlot WxWidgets library not found! Plotting deactivated.)
fi

AM_CONDITIONAL(USE_PLPLOTWX, test $use_plplotwx = "yes")

# --- check for sqlite3
#PKG_CHECK_MODULES([SQLITE3], [sqlite3 >= 3.2], [use_sqlite3]="yes", [use_sqlite3]="no")
#if test $use_sqlite3 = "yes"; then
#  CXXFLAGS="$SQLITE3_CFLAGS $CXXFLAGS"
#  LIBS="$SQLITE3_LIBS $LIBS"
#  AC_DEFINE(HAVE_DATABASE,1,
#            [Define if you have the Sqlite3 libraries and header files.])
#else
#  AC_MSG_ERROR(Sqlite3 library not found.)
#fi


# Works with python2.6 only
CXXFLAGS="`python-config --cflags` -I\$(top_srcdir)/ext-gmock/gmock/include -I\$(top_srcdir)/ext-gmock/gmock/gtest/include $CXXFLAGS"
LIBS="`python-config --libs` $LIBS"
# Works with python2.4 only
#CXXFLAGS="-I/usr/include/python2.4 -I/usr/lib64/python2.4/site-packages/numpy/core/include $CXXFLAGS"
#LIBS="-L/usr/lib64/python2.4 $LIBS"

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# XXX
#AC_CHECK_HEADERS( \
#
#)

# Checks for library functions.

AC_CONFIG_FILES([
 Makefile
 ext-gmock/Makefile
 blocks/Makefile
 common/Makefile
 common/codec/Makefile
 common/plugins/Makefile
 common/socket/Makefile
 common/sound/Makefile
 src/Makefile
])
AC_OUTPUT
