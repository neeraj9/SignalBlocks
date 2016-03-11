//
// $Id: polynomial-test.cc,v 1.14 2006-05-01 14:24:57 cholm Exp $ 
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
#ifndef GSLMM_polynomial_double
#include "polynomial/polynomial.hh"
#endif
#ifndef GSLMM_divide_difference_double
#include "polynomial/divide_difference.hh"
#endif
#ifndef GSLMM_taylor_expansion_double
#include "polynomial/taylor_expansion.hh"
#endif
#ifndef GSLMM_math_type_trait
#include "math/type_trait.hh"
#endif
#ifndef GSLMM_test_suite
#include "test/test_suite.hh"
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
#ifndef __STDEXCEPT__
#include <stdexcept>
#endif

/** @file   polynomial/polynomial-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of polynomial classses.  */

int main (int argc, char** argv)
{
  double fac = 100;
  gslmm::test_suite ts("polynomial", argc, argv);

  const double eps = fac * gslmm::type_trait<double>::epsilon();


  double x, y;
  {
    x = 0.5;
    gslmm::polynomial<double> p1(1.0, 0.5, 0.3);
    y = p1.evaluate(x);
    ts.relative(y, 1 + 0.5 * x + 0.3 * x * x, eps,
		"gslmm::polynomial::evaluate({1, 0.5, 0.3}, 0.5)");
  }
  
  {
    double d[11] = { 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1 };
    x = 1.0;
    gslmm::polynomial<double> p2(d,11);
    y = p2.evaluate(x);
    ts.relative(y, 1.0, eps,
		"gslmm::polynomial::evaluate({1,-1,1,-1,1,-1,1,-1,1,-1,1},1.0)");
  }
  
  double xx[2];
  int n;
  // Quadratic 
  {
    gslmm::polynomial<double> p3(26, -20, 4);
    n = p3.solve(xx);
    ts.test(n, 0, "gslmm::polynomial::solve, no roots, (2x - 5)^2 = -1");
  }
  
  {
    gslmm::polynomial<double> p4(25.0, -20.0, 4.0);
    n = p4.solve(xx);
    ts.test(n, 2, "gslmm::polynomial::solve, one root, (2x - 5)^2 = 0");
    ts.relative(xx[0], 2.5, 1e-9, "xx[0], (2x - 5)^2 = 0");
    ts.relative(xx[1], 2.5, 1e-9, "xx[1], (2x - 5)^2 = 0");
    ts.check(xx[0] == xx[1], "xx[0] == xx[1], (2x - 5)^2 = 0");
  }
  
  {
    gslmm::polynomial<double> p5(21.0, -20.0, 4.0);
    n = p5.solve(xx);
    ts.test(n, 2, "gslmm::polynomial::solve, two roots, (2x - 5)^2 = 4");
    ts.relative(xx[0], 1.5, 1e-9, "xx[0], (2x - 5)^2 = 4");
    ts.relative(xx[1], 3.5, 1e-9, "xx[1], (2x - 5)^2 = 4");
  }
  
  {
    gslmm::polynomial<double> p6(0.0, 7.0, 4.0);
    n = p6.solve(xx);
    ts.test(n, 2, "gslmm::polynomial::solve, two roots, x(4x + 7) = 0");
    ts.relative(xx[0], -1.75, 1e-9, "xx[0], x(4x + 7) = 0");
    ts.relative(xx[1], 0.0, 1e-9, "xx[1], x(4x + 7) = 0");
  }
  
  {
    gslmm::polynomial<double> p7(-20.0, 0.0, 5.0);
    n = p7.solve(xx);
    ts.test(n, 2, "gslmm::polynomial::solve, two roots b = 0, 5 x^2 = 20");
    ts.relative(xx[0], -2.0, 1e-9, "xx[0], 5 x^2 = 20");
    ts.relative(xx[1], 2.0, 1e-9, "xx[1], 5 x^2 = 20");
  }
  
  // Cubic
  double xxx[3];
  {
    gslmm::polynomial<double> p8(-27.0, 0.0, 0.0, 1);
    n = p8.solve(xxx);
    ts.test(n, 1, "gslmm::polynomial::solve, one root, x^3 = 27");
    ts.relative(xxx[0], 3.0, 1e-9, "xxx[0], x^3 = 27");
  }

  {
    gslmm::polynomial<double> p9(-4913.0, 867.0, -51.0, 1);
    n = p9.solve(xxx);
    ts.test(n, 3, "gslmm::polynomial::solve, three roots, (x-17)^3=0");
    ts.relative(xxx[0], 17.0, 1e-9, "xxx[0], (x-17)^3=0");
    ts.relative(xxx[1], 17.0, 1e-9, "xxx[1], (x-17)^3=0");
    ts.relative(xxx[2], 17.0, 1e-9, "xxx[2], (x-17)^3=0");
  }
  
  {
    
    gslmm::polynomial<double> p10(-6647.0, 1071.0, -57.0, 1);
    n = p10.solve(xxx);
    ts.test(n, 3,
	    "gslmm::polynomial::solve, three roots, (x-17)(x-17)(x-23)=0");
    ts.relative(xxx[0], 17.0, 1e-9, "xxx[0], (x-17)(x-17)(x-23)=0");
    ts.relative(xxx[1], 17.0, 1e-9, "xxx[1], (x-17)(x-17)(x-23)=0");
    ts.relative(xxx[2], 23.0, 1e-9, "xxx[2], (x-17)(x-17)(x-23)=0");
  }
  {
    gslmm::polynomial<double> p11(+6647.0,-493.0,-11.0, 1);
    n = p11.solve(xxx);
    ts.test(n, 3,
	    "gslmm::polynomial::solve, three roots, (x+23)(x-17)(x-17)=0");
    ts.relative(xxx[0], -23.0, 1e-9, "xxx[0], (x+23)(x-17)(x-17)=0");
    ts.relative(xxx[1], 17.0, 1e-9, "xxx[1], (x+23)(x-17)(x-17)=0");
    ts.relative(xxx[2], 17.0, 1e-9, "xxx[2], (x+23)(x-17)(x-17)=0");
  }
  {
    gslmm::polynomial<double> p12(-50065.0, 5087.0, -143.0, 1);
    n = p12.solve(xxx);
    ts.test(n, 3,
	    "gslmm::polynomial::solve, three roots, (x-17)(x-31)(x-95)=0");
    ts.relative(xxx[0], 17.0, 1e-9, "xxx[0], (x-17)(x-31)(x-95)=0");
    ts.relative(xxx[1], 31.0, 1e-9, "xxx[1], (x-17)(x-31)(x-95)=0");
    ts.relative(xxx[2], 95.0, 1e-9, "xxx[2], (x-17)(x-31)(x-95)=0");
  }
  {
    gslmm::polynomial<double> p13(50065.0, 803.0, -109.0, 1);
    n = p13.solve(xxx);
    ts.test(n, 3,
	    "gslmm::polynomial::solve, three roots, (x+17)(x-31)(x-95)=0");
    ts.relative(xxx[0], -17.0, 1e-9, "xxx[0], (x+17)(x-31)(x-95)=0");
    ts.relative(xxx[1], 31.0, 1e-9, "xxx[1], (x+17)(x-31)(x-95)=0");
    ts.relative(xxx[2], 95.0, 1e-9, "xxx[2], (x+17)(x-31)(x-95)=0");
  }
  // Quadratic with complex roots 
  gslmm::complex<double> zz[2];
  {
    gslmm::polynomial<double> p14(26.0, -20.0, 4.0);
    n = p14.solve(zz);
    ts.test(n, 2,"gslmm::polynomial::solve, 2 roots (2x - 5)^2 = -1");
    ts.relative(zz[0].real(), 2.5, 1e-9, "z0.real, (2x - 5)^2 = -1");
    ts.relative(zz[0].imag(), -0.5, 1e-9, "z0.imag, (2x - 5)^2 = -1");

    ts.relative(zz[1].real(), 2.5, 1e-9, "z1.real, (2x - 5)^2 = -1");
    ts.relative(zz[1].imag(), 0.5, 1e-9, "z1.imag, (2x - 5)^2 = -1");
  }
  {
    gslmm::polynomial<double> p15(25.0, -20.0, 4.0);
    n = p15.solve(zz);
    ts.test(n, 2,"gslmm::polynomial::solve, one root, (2x - 5)^2 = 0");
    ts.relative(zz[0].real(), 2.5, 1e-9, "z0.real, (2x - 5)^2 = 0");
    ts.relative(zz[0].imag(), 0.0, 1e-9, "z0.imag (2x - 5)^2 = 0");
    ts.relative(zz[1].real(), 2.5, 1e-9, "z1.real, (2x - 5)^2 = 0");
    ts.relative(zz[1].imag(), 0.0, 1e-9, "z1.imag (2x - 5)^2 = 0");
    ts.check(zz[0].real() == zz[1].real(),"z0.real == z1.real, (2x - 5)^2 = 0");
    ts.check(zz[0].imag() == zz[1].imag(),"z0.imag == z1.imag, (2x - 5)^2 = 0");
  }
  {
    gslmm::polynomial<double> p16(21.0, -20.0, 4.0);
    n = p16.solve(zz);
    ts.test(n, 2,"gslmm::polynomial::solve, two roots, (2x - 5)^2 = 4");
    ts.relative(zz[0].real(), 1.5, 1e-9, "z0.real, (2x - 5)^2 = 4");
    ts.relative(zz[0].imag(), 0.0, 1e-9, "z0.imag, (2x - 5)^2 = 4");
    ts.relative(zz[1].real(), 3.5, 1e-9, "z1.real, (2x - 5)^2 = 4");
    ts.relative(zz[1].imag(), 0.0, 1e-9, "z1.imag, (2x - 5)^2 = 4");
  }
  {
    gslmm::polynomial<double> p17(0.0, 7.0, 4.0);
    n = p17.solve(zz);
    ts.test(n, 2,"gslmm::polynomial::solve, two roots, x(4x + 7) = 0");
    ts.relative(zz[0].real(), -1.75, 1e-9, "z0.real, x(4x + 7) = 0");
    ts.relative(zz[0].imag(), 0.0, 1e-9, "z0.imag, x(4x + 7) = 0");
    ts.relative(zz[1].real(), 0.0, 1e-9, "z1.real, x(4x + 7) = 0");
    ts.relative(zz[1].imag(), 0.0, 1e-9, "z1.imag, x(4x + 7) = 0");
  }
  {
    gslmm::polynomial<double> p18(-20.0, 0.0, 5.0);
    n = p18.solve(zz);
    ts.test(n, 2,"gslmm::polynomial::solve, two roots b = 0, 5 x^2 = 20");
    ts.relative(zz[0].real(), -2.0, 1e-9, "z0.real, 5 x^2 = 20");
    ts.relative(zz[0].imag(), 0.0, 1e-9, "z0.imag, 5 x^2 = 20");
    ts.relative(zz[1].real(), 2.0, 1e-9, "z1.real, 5 x^2 = 20");
    ts.relative(zz[1].imag(), 0.0, 1e-9, "z1.imag, 5 x^2 = 20");
  }
  {
    gslmm::polynomial<double> p19(20.0, 0.0, 5.0);
    n = p19.solve(zz);
    ts.test(n, 2,"gslmm::polynomial::solve, two roots b = 0, 5 x^2 = -20");
    ts.relative(zz[0].real(), 0.0, 1e-9, "z0.real, 5 x^2 = -20");
    ts.relative(zz[0].imag(), -2.0, 1e-9, "z0.imag, 5 x^2 = -20");
    ts.relative(zz[1].real(), 0.0, 1e-9, "z1.real, 5 x^2 = -20");
    ts.relative(zz[1].imag(), 2.0, 1e-9, "z1.imag, 5 x^2 = -20");
  }
  // Cubic with complex roots 
  gslmm::complex<double> zzz[3];
  {
    gslmm::polynomial<double> p20(-27.0, 0.0, 0.0, 1);
    n = p20.solve(zzz);
    ts.test(n, 3, "gsl_poly_complex_solve_cubic, three root, x^3 = 27");
    ts.relative(zzz[0].real(), -1.5, 1e-9, "z0.real, x^3 = 27");
    ts.relative(zzz[0].imag(), -1.5 * sqrt (3.0), 1e-9,
		"z0.imag, x^3 = 27");
    ts.relative(zzz[1].real(), -1.5, 1e-9, "z1.real, x^3 = 27");
    ts.relative(zzz[1].imag(), 1.5 * sqrt (3.0), 1e-9, "z1.imag, x^3 = 27");
    ts.relative(zzz[2].real(), 3.0, 1e-9, "z2.real, x^3 = 27");
    ts.relative(zzz[2].imag(), 0.0, 1e-9, "z2.imag, x^3 = 27");
  }
  {
    gslmm::polynomial<double> p21(39.0, 1.0, -1.0, 1);
    n = p21.solve(zzz);
    ts.test(n, 3, "gsl_poly_complex_solve_cubic, "
	    "three root, (x+3)(x^2-4x+13) = 0");
    ts.relative(zzz[0].real(), -3.0, 1e-9, "z0.real, (x+3)(x^2+1) = 0");
    ts.relative(zzz[0].imag(), 0.0, 1e-9, "z0.imag, (x+3)(x^2+1) = 0");
    ts.relative(zzz[1].real(), 2.0, 1e-9, "z1.real, (x+3)(x^2+1) = 0");
    ts.relative(zzz[1].imag(), -3.0, 1e-9, "z1.imag, (x+3)(x^2+1) = 0");
    ts.relative(zzz[2].real(), 2.0, 1e-9, "z2.real, (x+3)(x^2+1) = 0");
    ts.relative(zzz[2].imag(), 3.0, 1e-9, "z2.imag, (x+3)(x^2+1) = 0");
  }
  {
    gslmm::polynomial<double> p22(-4913.0, 867.0, -51.0, 1);
    n = p22.solve(zzz);
    ts.test(n, 3, "gsl_poly_complex_solve_cubic, three roots, (x-17)^3=0");
    ts.relative(zzz[0].real(), 17.0, 1e-9, "z0.real, (x-17)^3=0");
    ts.relative(zzz[0].imag(), 0.0, 1e-9, "z0.imag, (x-17)^3=0");
    ts.relative(zzz[1].real(), 17.0, 1e-9, "z1.real, (x-17)^3=0");
    ts.relative(zzz[1].imag(), 0.0, 1e-9, "z1.imag, (x-17)^3=0");
    ts.relative(zzz[2].real(), 17.0, 1e-9, "z2.real, (x-17)^3=0");
    ts.relative(zzz[2].imag(), 0.0, 1e-9, "z2.imag, (x-17)^3=0");
  }
  {
    gslmm::polynomial<double> p23(-6647.0, 1071.0, -57.0, 1);
    n = p23.solve(zzz);
    ts.test(n, 3, "gsl_poly_complex_solve_cubic, "
	    "three roots, (x-17)(x-17)(x-23)=0");
    ts.relative(zzz[0].real(), 17.0, 1e-9, "z0.real, (x-17)(x-17)(x-23)=0");
    ts.relative(zzz[0].imag(), 0.0, 1e-9, "z0.imag, (x-17)(x-17)(x-23)=0");
    ts.relative(zzz[1].real(), 17.0, 1e-9, "z1.real, (x-17)(x-17)(x-23)=0");
    ts.relative(zzz[1].imag(), 0.0, 1e-9, "z1.imag, (x-17)(x-17)(x-23)=0");
    ts.relative(zzz[2].real(), 23.0, 1e-9, "z2.real, (x-17)(x-17)(x-23)=0");
    ts.relative(zzz[2].imag(), 0.0, 1e-9, "z2.imag, (x-17)(x-17)(x-23)=0");
  }
  {
    gslmm::polynomial<double> p24(+6647.0, -493.0, -11.0, 1);
    n = p24.solve(zzz);
    ts.test(n, 3, "gsl_poly_complex_solve_cubic, three roots, "
	    "(x+23)(x-17)(x-17)=0");
    ts.relative(zzz[0].real(), -23.0, 1e-9, "z0.real, (x+23)(x-17)(x-17)=0");
    ts.relative(zzz[0].imag(), 0.0, 1e-9, "z0.imag, (x+23)(x-17)(x-17)=0");
    ts.relative(zzz[1].real(), 17.0, 1e-9, "z1.real, (x+23)(x-17)(x-17)=0");
    ts.relative(zzz[1].imag(), 0.0, 1e-9, "z1.imag, (x+23)(x-17)(x-17)=0");
    ts.relative(zzz[2].real(), 17.0, 1e-9, "z2.real, (x+23)(x-17)(x-17)=0");
    ts.relative(zzz[2].imag(), 0.0, 1e-9, "z2.imag, (x+23)(x-17)(x-17)=0");
  }
  {
    gslmm::polynomial<double> p25(-50065.0, 5087.0, -143.0, 1);
    n = p25.solve(zzz);
    ts.test(n, 3, "gsl_poly_complex_solve_cubic, three roots, "
	    "(x-17)(x-31)(x-95)=0");
    ts.relative(zzz[0].real(), 17.0, 1e-9, "z0.real, (x-17)(x-31)(x-95)=0");
    ts.relative(zzz[0].imag(), 0.0, 1e-9, "z0.imag, (x-17)(x-31)(x-95)=0");
    ts.relative(zzz[1].real(), 31.0, 1e-9, "z1.real, (x-17)(x-31)(x-95)=0");
    ts.relative(zzz[1].imag(), 0.0, 1e-9, "z1.imag, (x-17)(x-31)(x-95)=0");
    ts.relative(zzz[2].real(), 95.0, 1e-9, "z2.real, (x-17)(x-31)(x-95)=0");
    ts.relative(zzz[2].imag(), 0.0, 1e-9, "z2.imag, (x-17)(x-31)(x-95)=0");
  }
  {
    /* Wilkinson polynomial: y = (x-1)(x-2)(x-3)(x-4)(x-5) */
    gslmm::polynomial<double> wilkinson(-120, 274, -225, 85, -15, 1);
    gslmm::polynomial<double>::workspace w(wilkinson);
    gslmm::complex<double> zw[6];
  
    n = wilkinson.solve(zw, w);
  
    ts.test(n, 5, "gslmm::polynomial::solve, 5th-order Wilkinson polynomial");
    ts.relative(zw[0].real(), 1.0, 1e-9, "z0.real, 5th-order polynomial");
    ts.relative(zw[0].imag(), 0.0, 1e-9, "z0.imag, 5th-order polynomial");
    ts.relative(zw[1].real(), 2.0, 1e-9, "z1.real, 5th-order polynomial");
    ts.relative(zw[1].imag(), 0.0, 1e-9, "z1.imag, 5th-order polynomial");
    ts.relative(zw[2].real(), 3.0, 1e-9, "z2.real, 5th-order polynomial");
    ts.relative(zw[2].imag(), 0.0, 1e-9, "z2.imag, 5th-order polynomial");
    ts.relative(zw[3].real(), 4.0, 1e-9, "z3.real, 5th-order polynomial");
    ts.relative(zw[3].imag(), 0.0, 1e-9, "z3.imag, 5th-order polynomial");
    ts.relative(zw[4].real(), 5.0, 1e-9, "z4.real, 5th-order polynomial");
    ts.relative(zw[4].imag(), 0.0, 1e-9, "z4.imag, 5th-order polynomial");
  }
  {
    // : 8-th order polynomial y = x^8 + x^4 + 1 
    gslmm::polynomial<double> p26(1, 0, 0, 0, 1, 0, 0, 0, 1);
    gslmm::polynomial<double>::workspace w26(p26);
    gslmm::complex<double> z26[8];
  
    double C = 0.5;
    double S = sqrt (3.0) / 2.0;
  
    n = p26.solve(z26, w26);
    ts.test(n, 8, "gslmm::polynomial::solve, 8th-order polynomial");
    ts.relative(z26[0].real(), -S, 1e-9, "z0.real, 8th-order polynomial");
    ts.relative(z26[0].imag(),  C, 1e-9, "z0.imag, 8th-order polynomial");
    ts.relative(z26[1].real(), -S, 1e-9, "z1.real, 8th-order polynomial");
    ts.relative(z26[1].imag(), -C, 1e-9, "z1.imag, 8th-order polynomial");
    ts.relative(z26[2].real(), -C, 1e-9, "z2.real, 8th-order polynomial");
    ts.relative(z26[2].imag(),  S, 1e-9, "z2.imag, 8th-order polynomial");
    ts.relative(z26[3].real(), -C, 1e-9, "z3.real, 8th-order polynomial");
    ts.relative(z26[3].imag(), -S, 1e-9, "z3.imag, 8th-order polynomial");
    ts.relative(z26[4].real(),  C, 1e-9, "z4.real, 8th-order polynomial");
    ts.relative(z26[4].imag(),  S, 1e-9, "z4.imag, 8th-order polynomial");
    ts.relative(z26[5].real(),  C, 1e-9, "z5.real, 8th-order polynomial");
    ts.relative(z26[5].imag(), -S, 1e-9, "z5.imag, 8th-order polynomial");
    ts.relative(z26[6].real(),  S, 1e-9, "z6.real, 8th-order polynomial");
    ts.relative(z26[6].imag(),  C, 1e-9, "z6.imag, 8th-order polynomial");
    ts.relative(z26[7].real(),  S, 1e-9, "z7.real, 8th-order polynomial");
    ts.relative(z26[7].imag(), -C, 1e-9, "z7.imag, 8th-order polynomial");
  }
    
  double xa[7] = {0.16, 0.97, 1.94, 2.74, 3.58, 3.73, 4.70 };
  double ya[7] = {0.73, 1.11, 1.49, 1.84, 2.30, 2.41, 3.07 };
  double dd_expected[7] = {  7.30000000000000e-01,
			     4.69135802469136e-01,
			    -4.34737219941284e-02,
			     2.68681098870099e-02,
			    -3.22937056934996e-03,
			     6.12763259971375e-03,
			    -6.45402453527083e-03 };
  gslmm::divide_difference<double> dd1(xa, ya, 7);
  
  const double* my_dd = dd1.dd();
  my_dd = dd1.dd();
  for (int i = 0; i < 7; i++) 
    ts.relative(my_dd[i], dd_expected[i], 1e-10, 
		"divided difference dd[%d]", i);
  
  for (int i = 0; i < 7; i++) {
    double y = dd1.evaluate(xa[i]);
    ts.relative(y, ya[i], 1e-10, "divided difference y[%d]", i);
  }

  gslmm::taylor_expansion<double> coeff(1.5, dd1);
  for (int i = 0; i < 7; i++) {
    double y = coeff.evaluate(xa[i]);
    ts.relative(y, ya[i], 1e-10, "taylor expansion about 1.5 y[%d]", i);
  }

  // now summarize the results 
  return ts.summary() ? 0 : 1;

}
//___________________________________________________________________
//
// EOF
//
