#
#
# $Id: Makefile.am,v 1.21 2006-04-26 23:08:16 cholm Exp $
#
#   GSL C++ wrappers
#   Copyright (C) 2002  Christian Holm Christensen <cholm@nbi.dk>
#
#   This library is free software; you can redistribute it and/or
#   modify it under the terms of the GNU Lesser General Public License 
#   as published by the Free Software Foundation; either version 2 of 
#   the License, or (at your option) any later version.  
#
#   This library is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   Lesser General Public License for more details. 
#
#   You should have received a copy of the GNU Lesser General Public
#   License along with this library; if not, write to the Free
#   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
#   MA 02111-1307  USA  
#
#
#
AM_CPPFLAGS			= -I$(top_srcdir) -I$(top_builddir) \
			          $(filter-out -I/usr/include, $(GSL_CFLAGS))
AM_LDFLAGS			= $(GSL_LIBS)

include_HEADERS			=
pkginclude_HEADERS		= gslmm-util.hh
bin_PROGRAMS			= 
nobase_include_HEADERS		= 
nobase_pkginclude_HEADERS	= 
nobase_nodist_pkginclude_HEADERS =
noinst_HEADERS			= 
noinst_PROGRAMS			= 
noinst_SCRIPTS			= make-header.sh
EXTRA_DIST			= $(noinst_SCRIPTS) 
TESTS				=	 
AM_LFLAGS			= 
CLEANFILES			= $(wildcard */*.cxx) $(wildcard *-test.dat) \
				  $(nobase_nodist_pkginclude_HEADERS)
MAINTAINERCLEANFILES		= *~ 
BUILT_SOURCES			= $(nobase_nodist_pkginclude_HEADERS)
man_MANS			= 

%_double.hh:		%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< double		> $@
%_long_double.hh:	%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "long double"	> $@
%_float.hh:		%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< float		> $@
%_unsigned_long.hh:	%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "unsigned long"	> $@
%_long.hh:		%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "long"		> $@
%_unsigned_int.hh:	%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "unsigned int"	> $@
%_int.hh:		%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "int"		> $@
%_unsigned_short.hh:	%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "unsigned short"	> $@
%_short.hh:		%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "short"		> $@
%_unsigned_char.hh:	%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "unsigned char"	> $@
%_char.hh:		%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "char"		> $@
%_complex_double.hh:	%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "complex<double>"	> $@
%_complex_float.hh:	%_skel.hh make-header.sh
	-$(mkinstalldirs) $(dir $@)
	$(srcdir)/make-header.sh $< "complex<float>"	> $@

include annealing/Makefile
include blas/Makefile
include chebyshev/Makefile
include combination/Makefile
include compleks/Makefile
include constant/Makefile
include differentiation/Makefile
include eigen/Makefile
include error/Makefile
include fit/Makefile
include fourier/Makefile
include function/Makefile
include hankel/Makefile
include histogram/Makefile
include ieee/Makefile
include integration/Makefile
include interpolation/Makefile
include linear/Makefile
include math/Makefile
include minimization/Makefile
include montecarlo/Makefile
include ntuple/Makefile
include ordinary/Makefile
include permutation/Makefile
include polynomial/Makefile
include random/Makefile
include root/Makefile
include series/Makefile
include sort/Makefile
include special/Makefile
include statistic/Makefile
include test/Makefile
include vectormatrix/Makefile
include optionmm/Makefile

#
# EOF
#
