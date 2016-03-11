#
# $Id: Makefile.am,v 1.12 2006-05-09 07:40:34 cholm Exp $
#
#   General C++ parser and lexer
#   Copyright (C) 2002	Christian Holm Christensen <cholm@nbi.dk>
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
PNGS			= annealing-ex-e.png		\
			  annealing-ex-t.png		\
			  annealing-tsm-dist.png	\
			  annealing-tsm-final.png	\
			  annealing-tsm-init.png	\
			  chebyshev-ex.png		\
			  favicon.png			\
			  logo.png			
CXXS			= examples.cc			\
			  fdl.cc			\
			  lgpl.cc			\
			  mainpage.cc			
EXTRA_DIST		= header.html.in footer.html style.css doxyconfig.in \
			  $(CXXS) $(PNGS)
DISTCLEANFILES		= doxygen.log $(PACKAGE).tags
if HAVE_DOXYGEN
HTML			= html/index.html $(PACKAGE).tags
else 
HTML			=
endif

all-local:$(HTML)

clean-local:
	rm -rf *~ html latex man

html/index.html $(PACKAGE).tags: doxyconfig header.html footer.html style.css 
	$(DOXYGEN) $<  
	$(foreach p, $(PNGS), cp -f $(srcdir)/$(p) html/ ;)
#	if test -f html/installdox ; then \
#		(cd html && \
#		 ./installdox -lfluct.tag@$(datadir)/doc/fluct/html/) ; fi

tar-ball:$(HTML)
	if test -f html/index.html ; then \
	  mv html $(PACKAGE) ;	\
	  tar -czvf ../$(PACKAGE)-$(VERSION)-doc.tar.gz $(PACKAGE) ; \
	  mv $(PACKAGE) html ; fi 


#
# EOF
#
