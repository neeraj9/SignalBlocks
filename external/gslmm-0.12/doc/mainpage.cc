//
// $Id: mainpage.cc,v 1.10 2006-04-26 23:08:16 cholm Exp $
//
//   General C++ parser and lexer
//   Copyright (C) 2002  Christian Holm Christensen <cholm@nbi.dk>
//
//   This library is free software; you can redistribute it and/or
//   modify it under the terms of the GNU Lesser General Public License 
//   as published by the Free Software Foundation; either version 2 of 
//   the License, or (at your option) any later version.  
//
//   This library is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//   Lesser General Public License for more details. 
//
//   You should have received a copy of the GNU Lesser General Public
//   License along with this library; if not, write to the Free
//   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
//   MA 02111-1307  USA  
//
//
/** @file   mainpage.cc
    @author Christian Holm
    @date   Sun Sep 15 16:29:24 2002
    @brief  Title page documentation. */
/** @mainpage 
    @image html logo.png 

    @section intro Introduction. 

    This package contains some C++ wrappers for the C API provided by
    the <a href="http://www.gnu.org/software/gsl">GNU Scientific library</a> 
    (@b GSL).   

    @section doc Documentation 

    The code it self is documented extensively.  That is, the
    interfaces are explained.  However, the underlying concepts - for
    example: what is `Simulated Annealing' - isn't explained.  Please
    refer to the documentation that came with your @b GSL
    installation, and the references there in.  The references in the
    @b GSL documentation are rather good, so I deemed it a bl**dy
    waste of time repeating it.  The @b GSL documentation usally comes
    as @b info pages and a @b Postscript file.  To read the @b info
    pages, do 
    @verbatim
      prompt> info gsl-ref 
    @endverbatim 
    or however you prefer to read @b info pages (Emacs!?). @b GSL--
    does not come with @b info pages I'm afraid.  That's because I use
    @b Doxygen, which currently can not export to @b Texinfo (however,
    it can export to @b LaTeX, so presumably, it shouldn't be that
    difficult to implement that in @b Doxygen :-). 

    @section usage Using the class library 

    Simply include the class declaration files you need in your
    code. 

    The library provide three helper facilities to compile against the
    header files.  One is the application @c gslmm-config which will
    report various features of GSL--.  The second is an @b Autoconf
    macro @c AC_GLMM_PATH that will define @c GSLMM_CPPFLAGS in your
    make file.  Use that with @c AM_CPPFLAGS to include the proper
    search path for the GSL-- header files. Finally, there a 
    @b pkgconfig file that can be used in a similar way as the 
    @b Autoconf macro. 

    @section org Organisation of the code 
    
    As almost all of the classes are implemented as templates, the
    library mainly consists of header files.  This has the advantage,
    that the user can easily select the level of optimisation and
    specialisation as needed. 

    The header files generally live in a subdirectory that corresponds
    to the module that the class belongs to.  So for example, all the
    fourier transformation classes live in the the subdirectory 
    @c fourier.  Note, that the complex number classes live in 
    @c compleks rather than @c complex, as some older GCC version have
    a problem when you do @c @# @c include @c <complex> (one of the
    ISO/IEC C++ header files). 

    This means you should use a rather long path to include the header
    files into your code. For example, to include the histogram class
    header, you need to do 
    @code 
    #ifndef GSLMM_histogram 
    #include <gslmm/histogram/histogram.hh>
    #endif
    @endcode 
    The reason for this, is mainly maintainability, but it does
    provide a bit of clarity. 

    Please also refer to the <a href="modules.html">modules</a> pages.

    @section convention Conding conventions. 

    As far as possible, member functions are implmented as inline for
    speed optimisation.

    As far as possible, the classes are implmented as template
    specialisations.  To make the specialisations, the headers use
    macro expansion to help do that. 

    All classes are in the gslmm namespace, to pollute the users
    namespace as little as possible.

    Class and member names are in small letters, and words are
    sperated by an underscore.  Data member names start with an
    underscore.

    Names are spelled out, so that they are easier to read.  

    @section headers Specialisations and headers

    Many of the classes in this class library have explicit
    specialistions for @c double, @c float, @c int, @c long, and so
    on.  To use these specialisations, include the proper header file
    in your code. For example, if you want to use complex number that
    has float values, you can do
    @code
    #ifndef GSLMM_complex_float
    #include <gslmm/compleks/complex_float.hh>
    #endif
    @endcode 
    (The preprocessor guard is a common idiom to speed up
    compilation).  Note, that these headers are automatically created
    at build time, using the corresponding @c _skel.hh file.  In that
    way, I needn't duplicate the code for the specialisations, and
    still have seperate headers. 

    The headers without an explicit type name in the file name,
    include all possible specialisations.  For after 
    @code 
    #ifndef GSLMM_complex
    #inclucde <gslmm/compleks/complex.hh>
    #endif 
    @endcode 
    the specialisations gslmm::complex<double>, 
    gslmm::complex<long double> and gslmm::complex<float> will be
    defined and can be used in the client application.

    There are also headers endinding in `@c _base.hh' - these are the
    base template declarations.  For example 
    @code 
    #ifndef GSLMM_vector_base
    #inclucde <gslmm/vectormatrix/vector_base.hh>
    #endif 
    @endcode 
    will declare the template gslmm::vector<typename T, typname T1>,
    and although many of the member functions are defined, they do not
    use the GSL optimised functions. In other words, include these
    headers with care.  
    
    @section test The test

    As the library consists only of header files, and no compiled
    code, the only code that is compiled during installation are the
    various tests.  This is to ensure that everything in fact works.
    Therefor it is highly recommended that you run the checks after
    building, that is you should run <tt>make check</tt>.  

    All the tests supports the command line options @c -h that gives a
    short help message and @c -v that turns on verbose output from the
    tests (see also the class description of gslmm::test_suite). 

    When I compile the tests, I usually invoke a slew of options to
    GCC (I'm currently using 2.95.4 and 3.2.2).  These options are 
    <dl>
      <dt> <tt>-fno-elide-constructors</tt>: </dt> 
      <dd> Prevents Return Value Optimisation, which is an optional
           feature of the C++ standard</dd>
      <dt> <tt>-Werror</tt>: </dt>
      <dd> Do not allow warnings.</dd> 
      <dt> <tt>-W</tt>: </dt>
      <dd> Give some more warnings </dd>
      <dt> <tt>-ansi -pedantic</tt>: </dt>
      <dd> Be strictly ANSI (ISO/IEC) compliant </dd>
      <dt> <tt>-Wnon-virtual-dtor</tt>: </dt>
      <dd> Be sure not to make un-inheritable classes </dd>
      <dt> <tt>-Weffc++</tt>: </dt>
      <dd> Warnings about style from Scott Meyer's book' - make nicer
           code. This option isn't invoked per default, as it would
	   make the compilation fail.  The reason is, that some of the 
	   <tt>libstdc++</tt> from GCC does not follow the guidelines
	   set forth by Scott Meyer.  Therefor, this option is only
	   invoked if the option <tt>--enable-effc++-warnings</tt> 
	   @e and <tt>--enable-maintainer-mode</tt> is given to the 
	   @c ./configure script.  Another interesting option in this
	   respect is @c --disable-Werror that suppresses the 
	   @c -Werror option to GCC.
      <dt> <tt>-Wold-style-cast:</tt> </dt> 
      <dd> Warn about ugly C-style casts.  Better code</dd>
      <dt> <tt>-Woverloaded-virtual:</tt> </dt>
      <dd> We want to overload - not hide, so the signatures should
           match.</dd>
    </dl>
    
*/


//
// EOF
//
