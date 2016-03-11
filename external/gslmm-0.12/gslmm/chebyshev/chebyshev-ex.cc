//
// $Id: chebyshev-ex.cc,v 1.2 2008-02-13 15:50:59 cholm Exp $ 
//  
//  chebyshev/chebyshev-ex.cc
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
#ifndef GSLMM_chebyshev
#include <gslmm/chebyshev/chebyshev.hh>
#endif
#ifndef __IOMANIP__
#include <iomanip>
#endif

/** @file   chebyshev-ex.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Example of chebyshev class.  */

struct functor 
{
  double operator()(double x) 
  {
    return (x < .5 ? 0.25 : 0.75);
  }
};

double f(double x) 
{
  return (x < .5 ? 0.25 : 0.75);
}

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  size_t n = 100;
  
  functor of;
  gslmm::chebyshev_series c_functor(of, 40, 0, 1);
  gslmm::chebyshev_series c_f(f, 40, 0, 1);
  for (size_t i = 0; i < n; i++) {
    double x   = i / double(n);
    double e10, e40;
    double o10 = c_functor(10u, x, e10);
    double o40 = c_functor(x, e40);
    double f10 = c_f(size_t(10u), x);
    double f40 = c_f(x);
    std::cout << std::right << std::fixed
	      << std::setw(8) << x << " -> " 
	      << std::setw(8) << f(x) << ":   " 
	      << std::setw(8) << o10 << " +/- " 
	      << std::setw(8) << e10 << "     " 
	      << std::setw(8) << o40 << " +/- " 
	      << std::setw(8) << e40 << std::endl;
  }
  return 0;
}

//____________________________________________________________________  
//
// EOF
//  
