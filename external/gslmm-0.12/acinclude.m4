dnl -*- mode: m4 -*-
dnl
dnl
dnl $Id: acinclude.m4,v 1.6 2006-04-26 10:03:24 cholm Exp $
dnl
dnl   Copyright (C) 2002  Christian Holm Christensen <cholm@nbi.dk>
dnl
dnl   This library is free software; you can redistribute it and/or
dnl   modify it under the terms of the GNU Lesser General Public License 
dnl   as published by the Free Software Foundation; either version 2 of 
dnl   the License, or (at your option) any later version.  
dnl
dnl   This library is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl   Lesser General Public License for more details. 
dnl
dnl   You should have received a copy of the GNU Lesser General Public
dnl   License along with this library; if not, write to the Free
dnl   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
dnl   MA 02111-1307  USA  
dnl
dnl
dnl ------------------------------------------------------------------
AC_DEFUN([AC_DEBUG], 
[
  AC_REQUIRE([AC_PROG_CC])
  AC_REQUIRE([AC_PROG_CXX])
  AC_MSG_CHECKING(whether to make debug objects)
  AC_ARG_ENABLE([debug], 
    [AC_HELP_STRING([--enable-debug],[Enable debugging symbols in objects])])
  if test "x$enable_debug" = "xno" ; then 
    CFLAGS=`echo $CFLAGS | sed 's,-g,,'`
    CXXFLAGS=`echo $CXXFLAGS | sed 's,-g,,'`
  else 
    case $CXXFLAGS in 
    *-g*) ;;
    *)    CXXFLAGS="$CXXFLAGS -g" ;;
    esac
    case $CFLAGS in 
    *-g*) ;;
    *)    CFLAGS="$CFLAGS -g" ;;
    esac
  fi
	  AC_MSG_RESULT($enable_debug 'CFLAGS=$CFLAGS')
])

dnl ------------------------------------------------------------------
AC_DEFUN([AC_OPTIMIZATION], 
[
  AC_REQUIRE([AC_PROG_CC])
  AC_REQUIRE([AC_PROG_CXX])
        
  # enable_optimization=yes
  AC_ARG_ENABLE([optimization], 
    [AC_HELP_STRING([--enable-optimization],[Enable optimization of objects])])
  AC_MSG_CHECKING(for optimiztion level) 
  	    
  changequote(<<, >>)dnl
  if test "x$enable_optimization" = "xno" ; then 
    CFLAGS=`echo   $CFLAGS   | sed 's,-O\([0-9][0-9]*\|\),,'`
    CXXFLAGS=`echo $CXXFLAGS | sed 's,-O\([0-9][0-9]*\|\),,'`
  elif test "x$enable_optimization" = "xyes" ; then 
    case $CXXFLAGS in 
    *-O*) ;;
    *)    CXXFLAGS="$CXXFLAGS -O2" ;;
    esac
    case $CFLAGS in 
    *-O*) ;;
    *)    CFLAGS="$CXXFLAGS -O2" ;;
    esac
  else 
    CFLAGS=`echo   $CFLAGS   | sed "s,-O\([0-9][0-9]*\|\),-O$enable_optimization,"`
    CXXFLAGS=`echo $CXXFLAGS | sed "s,-O\([0-9][0-9]*\|\),-O$enable_optimization,"`
  fi
  changequote([, ])dnl
  AC_MSG_RESULT($enable_optimization 'CFLAGS=$CFLAGS')
])

dnl ------------------------------------------------------------------
AC_DEFUN([CHOLM_COMPILING],
[
  dnl
  dnl Rational:
  dnl
  dnl -fno-elide-constructors:
  dnl     Prevents Return Value Optimisation, which is an optional
  dnl     feature of the C++ standard
  dnl
  dnl -Werror:
  dnl     Do not allow warnings. (Would like this one, but std
  dnl     headers don't comply with -Weffc++ so this would make it
  dnl     impossible to compile the tests!)
  dnl
  dnl -W:
  dnl     Give some more warnings
  dnl
  dnl -ansi -pedantic:
  dnl     Be strictly ANSI (ISO/IEC) compliant
  dnl
  dnl -Wnon-virtual-dtor:
  dnl     Be sure not to make unheritable classes
  dnl
  dnl -Weffc++:
  dnl     Warnings about style from Scott Meyer's book' - make nicer
  dnl     code.
  dnl -Wold-style-cast:
  dnl     Warn about ugly C-style casts.  Better code
  dnl
  dnl -Woverloaded-virtual:
  dnl     We want to overload - not hide, so the signatures should
  dnl     match.
  AC_MSG_CHECKING(whether Christian is compiling)
  AC_ARG_ENABLE([extra-cflags],
	        [AC_HELP_STRING([--enable-extra-cflags],
                                [Enable extra compiler flags for checks])],
		  [if test "x$enableval" = "xyes" && 
		      test "x$ac_cv_cxx_compiler_gnu" = "xyes"; then 
    			CXXFLAGS="$CXXFLAGS -fno-elide-constructors \
				  -ansi -pedantic -Wall -W -Wnon-virtual-dtor"
		   fi])
  AC_ARG_ENABLE([effcxx-warnings],
		  [AC_HELP_STRING([--enable-effcxx-warnings],
                                [Enable Warnings from Scott Meyer's book])],
		  [if test "x$enableval" = "xyes" ; then 
		     CXXFLAGS="$CXXFLAGS  -Weffc++"
		   fi])
  AC_ARG_ENABLE([Werror],
                [AC_HELP_STRING([--enable-Werror],
                                [Make warnings into errors])], 
	 	  [if test "x$enableval" = "xyes" ; then
                   CXXFLAGS="$CXXFLAGS  -Werror"
                 fi])
  AC_MSG_RESULT(CXXFLAGS=$CXXFLAGS)
])

	
#
# EOF
#
