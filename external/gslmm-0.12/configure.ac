dnl
dnl $Id: configure.ac,v 1.23 2008-02-13 15:50:59 cholm Exp $
dnl
dnl   GSL C++ wrappers
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
dnl 
dnl Autoconf 2.53 requires a lot of stuff here
AC_INIT([GSL--], [0.12], [cholm@nbi.dk], [gslmm])
AC_PREREQ(2.53)
AC_COPYRIGHT([GNU Lesser General Public License])
AC_REVISION($Revision: 1.23 $)
AC_CONFIG_SRCDIR(gslmm/error/error_handler.hh)
AC_CONFIG_AUX_DIR([config])

dnl
dnl This may be changed in never Automake 
dnl
AM_INIT_AUTOMAKE([1.6])
AM_INIT_AUTOMAKE([gnu])
AM_CONFIG_HEADER([config/config.hh])

dnl
dnl For some extra stuff in maitainer-mode
dnl 
dnl AM_MAINTAINER_MODE

dnl Compilers and such 
dnl
dnl For ICC and the like (MSVC?) we probably need an explicit 
dnl
dnl  --enable-dependency-tracking 
dnl
dnl to configure for dependencies to be created. 
AC_PROG_CC
AC_PROG_CXX

dnl
dnl Hack to make icc -KPIC work.  This _must_ be done before the
dnl libtool checks, as it will otherwise kill the PIC stuff for
dnl ICC.  
dnl
dnl AC_CACHE_VAL(lt_cv_prog_cc_pic,
dnl [
dnl 	case $CC in
dnl 	icc) lt_cv_prog_cc_pic=-kPIC ;;
dnl 	esac
dnl ])
dnl AC_PROG_LIBTOOL 


dnl
dnl The GSL library
dnl
AM_PATH_GSL(1.0,, AC_MSG_ERROR(GSL not found))
gsl_version=`$GSL_CONFIG --version` 
GSL_VERSION=`$GSL_CONFIG --version | awk 'BEGIN {FS="."}{printf "%d", (($1*100)+$2)*100+$3}'`
AC_DEFINE_UNQUOTED(GSL_VERSION_CODE,$GSL_VERSION,[Version code of GSL])

dnl
dnl Turn on/off optimisation and/or debugging
dnl
AC_DEBUG
AC_OPTIMIZATION
CHOLM_COMPILING

dnl
dnl Documentation 
dnl
AC_ARG_VAR(DOXYGEN, The Documentation Generator)
AC_ARG_VAR(PERL, The Perl interpreter)
AC_PATH_PROG(PERL, perl)
AC_PATH_PROG(DOXYGEN, doxygen)
AM_CONDITIONAL(HAVE_DOXYGEN, test ! "x$DOXYGEN" = "x")
HAVE_DOT=NO
DOT_PATH=
AC_PATH_PROG(DOT, dot)
if ! test "x$DOT" = "x" ; then
   HAVE_DOT=YES
   DOT_PATH=`dirname $DOT`
fi
AM_CONDITIONAL(HAVE_DOXYGEN, test "x$DOXYGEN" != "x")
DOC_KEYWORDS="GSL,\
	GNU,Scientific,Library,vector,matrix,complex,polynomial,\
	permutation,combination,sort,BLAS,linear,algebra,eigenvalue,\
	eigenvector,eigensystem,fast,fourier,transform,numerical,\
	integration,differentation,psuedo,random,number,generator,\
	statistic,histogram,ntuple,n-tuple,monte,carlo,simulated,\
	annealing,ordinary,differential,equation,interpolation,\
	chebyshev,approximation,serie,accelration,discrete,hankel,\
	transform,root-finding,minimization,least-square,fitting,\
	nonlinear,physical,contant,ieee,floating,point,C++,wrapper,\
	wrappers,GSL--,gslmm,gsl++"
AC_SUBST([DOC_KEYWORDS])
AC_SUBST([HAVE_DOT])
AC_SUBST([DOT_PATH])

dnl Changelog
AC_ARG_VAR(RCS2LOG, Make the changelog from repository)
AC_PATH_PROG(RCS2LOG, rcs2log)

AC_OUTPUT([Makefile 
	   gslmm/gslmm-util.hh
	   gslmm/Makefile 
	   debian/Makefile
	   doc/header.html
	   doc/Makefile
	   doc/doxyconfig])

dnl
dnl EOF
dnl
