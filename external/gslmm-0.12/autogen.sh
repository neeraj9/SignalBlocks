#!/usr/bin/make -f
#
# $Id: autogen.sh,v 1.4 2006-04-26 10:03:24 cholm Exp $
#
#   General C++ parser and lexer
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
CXX		= g++-3.2
CC		= gcc-3.2
AUTOMAKE_VERSION= -1.9
PREFIX		= $(HOME)/tmp
CONFFLAGS	= 
PACKAGE		:= $(strip $(shell grep AC_INIT configure.ac |\
			sed 's/.*([^,]*,\([^,]*\),[^,]*,\(.*\)).*/\2/'))
VERSION		:= $(strip $(shell grep AC_INIT configure.ac |\
			sed 's/.*([^,]*,\([^,]*\),[^,]*,\(.*\)).*/\1/'))

all:	setup
	./configure $(CONFFLAGS)

noopt:	setup 
	./configure --disable-optimization --with-gsl-prefix=$(PREFIX) \
		--enable-maintainer-mode --prefix=$(PREFIX) $(CONFFLAGS) \
		CC=$(CC) CXX=$(CXX) 

setup: 	ChangeLog configure

aclocal.m4:configure.ac
	aclocal$(AUTOMAKE_VERSION)

config/config.hh.in:aclocal.m4 configure.ac
	autoheader 

Makefile.in:config/config.hh.in configure.ac Makefile.am \
	$(wildcard */Makefile.am) $(wildcard */*/Makefile)
	automake$(AUTOMAKE_VERSION) -a 

configure:Makefile.in
	autoconf 

make:	all
	$(MAKE) -f Makefile 

dists:  ChangeLog 
	$(MAKE) dist
	$(MAKE) tar-ball -C doc


ChangeLog:
	rm -f $@
	touch $@
	rcs2log > $@

show:
	@echo "$(PACKAGE) version $(VERSION)"
clean: 
	find . -name Makefile.in 	| xargs rm -f 
	find . -name "*~" 		| xargs rm -f 
	find . -name core 		| xargs rm -f 
	find . -name .libs 		| xargs rm -rf 
	find . -name .deps 		| xargs rm -rf 
	find . -name "*.lo" 		| xargs rm -f 
	find . -name "*.o" 		| xargs rm -f 
	find . -name "*.la" 		| xargs rm -rf
	find . -name "*.log" 		| xargs rm -rf

	rm -f 	config/missing       	\
	  	config/mkinstalldirs 	\
	  	config/ltmain.sh     	\
	  	config/config.guess 	\
		config/config.sub 	\
		config/install-sh 	\
		config/ltconfig 	\
		config/config.hh 	\
		config/config.hh.in 	\
		config/stamp-h		\
		config/stamp-h1		\
		config/depcomp

	rm -rf  gslmm/vectormatrix/vector_{double,long_double,float}.hh	\
		gslmm/vectormatrix/vector_{long,unsigned_long}.hh	\
		gslmm/vectormatrix/vector_{int,unsigned_int}.hh		\
		gslmm/vectormatrix/vector_{short,unsigned_short}.hh	\
		gslmm/vectormatrix/vector_{char,unsigned_char}.hh	\
		gslmm/vectormatrix/matrix_{double,long_double,float}.hh	\
		gslmm/vectormatrix/matrix_{long,unsigned_long}.hh	\
		gslmm/vectormatrix/matrix_{int,unsigned_int}.hh		\
		gslmm/vectormatrix/matrix_{short,unsigned_short}.hh	\
		gslmm/vectormatrix/matrix_{char,unsigned_char}.hh	\
		gslmm/compleks/complex_{double,long_double,float}.hh	\
		gslmm/polynomial/divide_difference_double.hh		\
		gslmm/polynomial/taylor_expansion_double.hh		\
		gslmm/polynomial/polynomial_double.hh			\
		gslmm/gslmm-util.hh					\
		gslmm/*-test						\
		gslmm/*-test.dat					\
		gslmm/Makefile

	rm -rf	doc/html		\
		doc/doxyconfig		\
		doc/gslmm.tags		\
		doc/latex		\
		doc/xml			\
		doc/Makefile

	rm -rf 	aclocal.m4 		\
		autom4te.cache		\
		config.cache 		\
		config.status  		\
		config.log 		\
		configure 		\
		INSTALL 		\
		ChangeLog		\
		configure-stamp		\
		build-stamp		\
		libtool			\
		Makefile

	rm -rf	gslmm-*.tar.gz
#
# EOF
#
