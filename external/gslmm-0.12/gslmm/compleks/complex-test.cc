//
// $Id: complex-test.cc,v 1.13 2008-02-13 15:50:59 cholm Exp $ 
//  
//  gslmm::error
//  Copyright (C) 2002 Christian Holm Christensen <cholm@nbi.dk> 
//
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public License 
//  as published by the Free Software Foundation; either version 2.1 
//  of the License, or (at your option) any later version. 
//
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free 
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 
//  02111-1307 USA 
//
#ifndef __IOSTREAM__
#include <iostream>
#endif
#ifndef GSLMM_complex
#include <gslmm/compleks/complex.hh>
#endif
#ifndef GSLMM_ieee
#include <gslmm/ieee/ieee.hh>
#endif
#ifndef GSLMM_math_type_trait
#include <gslmm/math/type_trait.hh>
#endif
#ifndef GSLMM_test_suite
#include <gslmm/test/test_suite.hh>
#endif
#ifndef __FSTREAM__
#include <fstream>
#endif
#ifndef __IOMANIP__
#include <iomanip>
#endif
#ifndef __CSTDLIB__
#include <cstdlib>
#endif

/** @file   complex-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of complex classses.  */

struct test_f
{
  const char*  name;
  double x;
  double y;
  double fx;
  double fy;
};

#define FN(x)    "" # x
#define ARG(x,y) x, y
#define RES(x,y) x, y

struct test_f list1[] =
{
#include "compleks/complex_results1.h"
  {0, 0, 0, 0, 0}
};

struct test_f list2[] =
{
#include "compleks/complex_results.h"
  {0, 0, 0, 0, 0}
};

int main (int argc, char** argv)
{
  gslmm::ieee<>::instance().setup();

  double fac = 1e8;
  gslmm::test_suite ts("complex", argc, argv);

  gslmm::complex<double> a = 1 + 3 * gslmm::complex<double>::i();
  // gslmm::complex<double> a = 1;
  // a +=  3 * gslmm::complex<double>::i();
  ts.test(a.real(), 1, "Real(a = 1 + 3 * i) = 1");
  ts.test(a.imag(), 3, "Imag(a = 1 + 3 * i) = 3");

  size_t n1 = 10;
  for (size_t i = 0 ; i < n1; i++) {
    double r = (i - 5.0) * 0.3 ;
    double t = 2.0 * M_PI * i / 5 ;
    double x = r * cos(t), y = r * sin(t) ;
    gslmm::complex<double> z(r, t, true);
    ts.relative(z.real(), x, fac * gslmm::type_trait<double>::epsilon(), 
		"gslmm::complex %4d (polar) real part at (r=%g,t=%g)", 
		i, r, t);
    ts.relative(z.imag(), y, fac * gslmm::type_trait<double>::epsilon(), 
		"gslmm::complex %4d (polar) imag part at (r=%g,t=%g)", 
		i, r, t);
  }
  size_t i = 0;
  while (list1[i].name) {
    struct test_f t = list1[i];
    double f = 0;
    std::string n(t.name);
    gslmm::complex<double> z = gslmm::complex<double>(t.x, t.y);
    if      (n == "abs" )   f = z.abs();	
    else if (n == "abs2")   f = z.square_abs();
    else if (n == "arg")    f = z.arg();	
    else if (n == "logabs") f = z.log_of_abs();
    else continue;
    ts.relative(f, t.fx, fac * gslmm::type_trait<double>::epsilon(), 
		"gslmm::complex %4d %s at (%g,%g)", 
		i + n1, t.name, t.x, t.y);
    i++;
  }
  n1 += i;
  
  i = 0;
  while (list2[i].name) {
    struct test_f t = list2[i];
    gslmm::complex<double> z(t.x, t.y);
    gslmm::complex<double> fz;
    std::string n(t.name);
    if (n == "arccos")          fz = gslmm::arccos(z);
    else if (n == "arccosh")    fz = gslmm::arccosh(z);
    else if (n == "arccot")     fz = gslmm::arccot(z);
    else if (n == "arccoth")    fz = gslmm::arccoth(z);
    else if (n == "arccsc")     fz = gslmm::arccsc(z);
    else if (n == "arccsch")    fz = gslmm::arccsch(z);
    else if (n == "arcsec")     fz = gslmm::arcsec(z);
    else if (n == "arcsech")    fz = gslmm::arcsech(z);
    else if (n == "arcsin")     fz = gslmm::arcsin(z);
    else if (n == "arcsinh")    fz = gslmm::arcsinh(z);
    else if (n == "arctan")     fz = gslmm::arctan(z);
    else if (n == "arctanh")    fz = gslmm::arctanh(z);
    else if (n == "cos")        fz = gslmm::cos(z);
    else if (n == "cosh")       fz = gslmm::cosh(z);
    else if (n == "cot")        fz = gslmm::cot(z);
    else if (n == "coth")       fz = gslmm::coth(z);
    else if (n == "csc")        fz = gslmm::csc(z);
    else if (n == "csch")       fz = gslmm::csch(z);
    else if (n == "exp")        fz = gslmm::exp(z);
    else if (n == "log")        fz = gslmm::log(z);
    else if (n == "log10")      fz = gslmm::log10(z);
    else if (n == "sec")        fz = gslmm::sec(z);
    else if (n == "sech")       fz = gslmm::sech(z);
    else if (n == "sin")        fz = gslmm::sin(z);
    else if (n == "sinh")       fz = gslmm::sinh(z);
    else if (n == "sqrt")       fz = gslmm::sqrt(z);
    else if (n == "tan")        fz = gslmm::tan(z);
    else if (n == "tanh")       fz = gslmm::tanh(z);
    else continue;

    ts.relative(fz.real(), t.fx, fac * gslmm::type_trait<double>::epsilon(), 
		"gslmm::complex %4d %s real part at (%g,%g)", 
		i + n1, t.name, t.x, t.y);
    ts.relative(fz.imag(), t.fy, fac * gslmm::type_trait<double>::epsilon(), 
		"gslmm::complex %4d %s imaginary part at (%g,%g)", 
		i + n1, t.name, t.x, t.y);
    i++;
  }

  ts.summary();
#if 0
  std::cout << "Accepted relative error: " 
	    << fac * gslmm::type_trait<double>::epsilon() << std::endl;
#endif
  exit(ts.failed());
}

//
// EOF
//  
