//
// $Id: chebyshev-test.cc,v 1.6 2006-05-01 14:24:37 cholm Exp $ 
//  
//  chebyshev/chebyshev-test.cc
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
#ifndef GSLMM_math_type_trait
#include <gslmm/math/type_trait.hh>
#endif
#ifndef GSLMM_chebyshev
#include <gslmm/chebyshev/chebyshev.hh>
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

/** @file   chebyshev-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of chebyshev classses.  */

//____________________________________________________________________  
struct f_T0
{
  double operator()(double) { return 1; }
};


//____________________________________________________________________  
struct f_T1
{
  double operator()(double x) { return x; }
};

//____________________________________________________________________  
struct f_T2
{
  double operator()(double x) { return 2 * x * x - 1; }
};

//____________________________________________________________________  
struct f_sin
{
  double operator()(double x) { return sin(x); }
};


//____________________________________________________________________  
double isin(double x) { return -(1+cos(x)); }
  
//____________________________________________________________________  
void
check_eval(gslmm::test_suite& ts, gslmm::chebyshev_series& cs,
	   double (*f)(double), const char* msg, double tol) 
{
  // Check evaluation 
  for (double x = -M_PI; x < M_PI; x += M_PI / 100.) {
    double r = cs(x);
    ts.absolute(r, (*f)(x), tol, msg, x);
  }
}

//____________________________________________________________________  
void
check_eval(gslmm::test_suite& ts, gslmm::chebyshev_series& cs,
	     double (*f)(double), const char* msg, double tol, size_t n) 
{
  // Check evaluation 
  for (double x = -M_PI; x < M_PI; x += M_PI / 100.) {
    double r = cs(n, x);
    ts.absolute(r, (*f)(x), tol, msg, x);
  }
}

//____________________________________________________________________  
void
check_eval(gslmm::test_suite& ts, gslmm::chebyshev_series& cs,
	     double (*f)(double), const char* msg, double tol, double ftol) 
{
  double eps  = gslmm::type_trait<double>::epsilon();
  for (double x = -M_PI; x < M_PI; x += M_PI/100.0) {
    double e;
    double r = cs(x, e);
    double y = (*f)(x);
    ts.absolute(r, y, tol, msg, x);
    ts.factor(fabs(r-y) + eps, e, ftol, msg, x); 
  }
}

//____________________________________________________________________  
void
check_eval(gslmm::test_suite& ts, gslmm::chebyshev_series& cs,
	       double (*f)(double), const char* msg,
	       double tol, double ftol, size_t n) 
{
  double eps  = gslmm::type_trait<double>::epsilon();
  for (double x = -M_PI; x < M_PI; x += M_PI/100.0) {
    double e;
    double r = cs(n, x, e);
    double y = (*f)(x);
    ts.absolute(r, y, tol, msg, x);
    ts.factor(fabs(r-y) + eps, e, ftol, msg, x); 
  }
}

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("chebyshev", argc, argv);
  double eps  = gslmm::type_trait<double>::epsilon();
  double tol  = 100 * eps;
  double ftol = 20.;
  size_t tr   = 25;
  
  {
    f_T0 t0;
    gslmm::chebyshev_series cs(t0, 40, -1, 1);
    for (size_t i = 0; i < cs.order(); i++) {
      double c_exp = (i == 0 ? 2. : 0.);
      ts.absolute(cs.coefficent(i), c_exp, tol, "c[%d] for T_0(x)", i);
    }
  }
  {
    f_T1 t1;
    gslmm::chebyshev_series cs(t1, 40, -1, 1);
    for (size_t i = 0; i < cs.order(); i++) {
      double c_exp = (i == 1 ? 1. : 0.);
      ts.absolute(cs.coefficent(i), c_exp, tol, "c[%d] for T_1(x)", i);
    }
  }
  {
    f_T2 t2;
    gslmm::chebyshev_series cs(t2, 40, -1, 1);
    for (size_t i = 0; i < cs.order(); i++) {
      double c_exp = (i == 2 ? 1. : 0.);
      ts.absolute(cs.coefficent(i), c_exp, tol, "c[%d] for T_1(x)", i);
    }
  }
  
  f_sin f;
  gslmm::chebyshev_series cs(f, 40, -M_PI, M_PI);

  // Check coefficents
  double c_exp[] = { 0.,  5.69230686359506e-01, 
		     0., -6.66916672405979e-01,
		     0.,  1.04282368734237e-01 };
  for (size_t i = 0; i < 5; i++) 
    ts.absolute(cs.coefficent(i), c_exp[i], tol, "c[%d] for F_sin(x)",i);

  check_eval(ts, cs, sin, "sin(%.3g)", tol);
  check_eval(ts, cs, sin, "sin(%.3g)", tol, tr);
  check_eval(ts, cs, sin, "sin(%.3g)", tol, ftol);
  check_eval(ts, cs, sin, "sin(%.3g)", 100 * tol, ftol, tr);

  //__________________________________________________________________
  // Test derivative
  gslmm::chebyshev_series csd(cs.derivative());
  check_eval(ts, csd, cos, "sin'(%.3g)", 1600 * tol);
  check_eval(ts, csd, cos, "sin'(%.3g)", 1600 * tol, tr);
  //check_eval(ts, csd, cos, "sin'(%.3g)", 1600 * tol, ftol);
  //check_eval(ts, csd, cos, "sin'(%.3g)", 1600 * tol, ftol, tr);

  //__________________________________________________________________
  // Test integral
  gslmm::chebyshev_series csi(cs.integral());

  check_eval(ts, csi, isin, "-1-cos(%.3g)", tol);
  check_eval(ts, csi, isin, "-1-cos(%.3g)", tol, tr);
  //check_eval(ts, csi, isin, "-1-cos(%.3g)", tol, ftol);
  //check_eval(ts, csi, isin, "-1-cos(%.3g)", 100 * tol, ftol, tr);
    
  return ts.summary() ? 0 : 1;
}

//____________________________________________________________________  
//
// EOF
//  
