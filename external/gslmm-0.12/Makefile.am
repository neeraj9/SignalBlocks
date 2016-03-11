#
# $Id: Makefile.am,v 1.7 2004-06-03 14:20:08 cholm Exp $
#
#   C++ for GSL
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
AUTOMAKE_OPTIONS	= gnu
ACLOCAL_AMFLAGS		= -I .

SUBDIRS			= gslmm doc debian
EXTRA_DIST		= autogen.sh acinclude.m4 README
CLEANFILES		= configure-stamp build-stamp

ChangeLog:
	rm -f $@
	touch $@
	@RCS2LOG@ > $@ 



#
# EOF
#
