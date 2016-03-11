//
// $Id: annealing-ex.cc,v 1.1 2006-04-28 23:23:48 cholm Exp $ 
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
#ifndef GSLMM_annealing_annealing
# include <gslmm/annealing/annealing.hh>
#endif
#include <cmath>

//____________________________________________________________________
struct trivial_print
{
  void operator()(const double& v)
  {
    printf ("%12g", v);
  }
};


//____________________________________________________________________
struct trivial : public gslmm::annealing<double,trivial_print>
{
  trivial(gslmm::generator g) 
    : gslmm::annealing<double,trivial_print>(g, 10, 2.e-6, 0.002, 10,
					     1.005, 1)
  {}
  double energy(const double& v)
  {
    return exp(-pow(v - 1., 2.)) * sin(8 * v);
  }
  double metric(const double& x, const double& y)
  {
    return fabs(x - y);
  }
  void step(double& v, double step)
  {
    double u = _generator.uniform();
    double r = u * 2 * step - step + v;
    v        = r;
  }
};


//____________________________________________________________________
int 
main()
{
  gslmm::generator g(gsl_rng_default);
  trivial          t(g);
  double v = 15.5;
  t.solve(v, true);
  std::cout << "# Result is " << v  << std::endl;
}

//____________________________________________________________________
//
// EOF
//
