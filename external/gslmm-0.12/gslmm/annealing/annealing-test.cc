//
// $Id: annealing-test.cc,v 1.5 2006-04-28 23:23:48 cholm Exp $ 
//  
//  annealing/annealing-test.cc
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
# include <iostream>
#endif
#ifndef __cmath__
# include <cmath>
#endif
#ifndef GSLMM_annealing
# include <gslmm/annealing/annealing.hh>
#endif
#ifndef GSLMM_test_suite
# include <gslmm/test/test_suite.hh>
#endif
#ifndef __IOMANIP__
# include <iomanip>
#endif

/** @file   annealing-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of annealing classses.  */

//____________________________________________________________________  
/* The function tested here has multiple mimima. 
   The global minimum is at    x = 1.36312999, (f = -0.87287)
   There is a local minimum at x = 0.60146196, (f = -0.84893) */
struct test : public gslmm::annealing<double>
{
  typedef gslmm::annealing<double> base_t;
  test(gslmm::generator& g) 
    : base_t(g, 1000, 2e-6, .008, 1, 1.003, 1)
  {}
  double energy(const double& x) 
  {
    return exp(-pow(x-1, 2)) * sin(8 * x) - exp(-pow(x-1000,2))*.89;
  }
  double metric(const double& x, const double& y) 
  {
    return fabs(x - y);
  }
  void step(double& x, const double step)
  {
    double nx = _generator.uniform() * 2 * step - step + x;
    x = nx;
  }
  void do_test(gslmm::test_suite& ts, double x0) 
  {
    const double r = 1.36312999455315182;
    double       x = x0;
    solve(x, false);
    ts.relative(x, r, 1e-3, "f(x)=exp(-(x-1)^2)sin(8x), x_0=%f", x0);
  }
};

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("annealing", argc, argv);

  gslmm::generator g;
  test t(g);
  double x_0s[] = { -10., 10, .6, .5, .4, 0. };
  for (size_t i = 0; x_0s[i] != 0; i++) 
    t.do_test(ts, x_0s[i]);
  
  return ts.summary() ? 0 : 1;
}

//____________________________________________________________________  
//
// EOF
//  
