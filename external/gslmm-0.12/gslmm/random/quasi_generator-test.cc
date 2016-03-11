//
// $Id: quasi_generator-test.cc,v 1.3 2006-05-01 14:24:57 cholm Exp $ 
//  
//  random/random-test.cc
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
#ifndef GSLMM_random_quasi_generator
#include <gslmm/random/quasi_generator.hh>
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

/** @file   random-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of random classses.  */


//____________________________________________________________________  
void test_sobol(gslmm::test_suite& ts)
{
  int status = 0;
  std::vector<double> v(3);
  /* int i; */

  /* test in dimension 2 */
  gslmm::quasi_generator g(2, gslmm::quasi_generator::sobol);
  g.get(v);
  g.get(v);
  g.get(v);
  status += ( v[0] != 0.25 || v[1] != 0.75 );
  g.get(v);
  status += ( v[0] != 0.375 || v[1] != 0.375 );
  
  ts.status(status, "%s, Sobol d=2", g.name());

  status = 0;
  /* test in dimension 3 */
  g = gslmm::quasi_generator(3,gslmm::quasi_generator::sobol);
  g.get(v);
  g.get(v);
  g.get(v);
  status += ( v[0] != 0.25 || v[1] != 0.75 || v[2] != 0.25 );
  g.get(v);
  status += ( v[0] != 0.375 || v[1] != 0.375 || v[2] != 0.625 );

  ts.status(status, "%s, Sobol d=3", g.name());
  
  status = 0;
  g.set();
  g.get(v);
  g.get(v);
  g.get(v);
  status += ( v[0] != 0.25 || v[1] != 0.75 || v[2] != 0.25 );
  g.get(v);
  status += ( v[0] != 0.375 || v[1] != 0.375 || v[2] != 0.625 );

  ts.status(status, "%s, Sobol d=3 (reinitialized)", g.name());
}


//____________________________________________________________________  
void test_nied2(gslmm::test_suite& ts)
{
  int status = 0;
  std::vector<double> v(3);
  /* int i; */

  /* test in dimension 2 */
  gslmm::quasi_generator g(2, gslmm::quasi_generator::niederreiter_2);
  g.get(v);
  g.get(v);
  g.get(v);
  status += ( v[0] != 0.75 || v[1] != 0.25 );
  g.get(v);
  status += ( v[0] != 0.25 || v[1] != 0.75 );
  g.get(v);
  g.get(v);
  g.get(v);
  status += ( v[0] != 0.625 || v[1] != 0.125 );

  ts.status(status, "%s, Niederreiter d=2", g.name());

  status = 0;

  /* test in dimension 3 */
  g = gslmm::quasi_generator(3,gslmm::quasi_generator::niederreiter_2);
  g.get(v);
  g.get(v);
  g.get(v);
  status += ( v[0] != 0.75 || v[1] != 0.25 || v[2] != 0.3125 );
  g.get(v);
  status += ( v[0] != 0.25 || v[1] != 0.75 || v[2] != 0.5625 );
  g.get(v);
  g.get(v);
  g.get(v);
  status += ( v[0] != 0.625 || v[1] != 0.125 || v[2] != 0.6875 );

  ts.status(status, "%s, Niederreiter d=3", g.name());

  status = 0;

  g.set();
  g.get(v);
  g.get(v);
  g.get(v);
  status += ( v[0] != 0.75 || v[1] != 0.25 || v[2] != 0.3125 );
  g.get(v);
  status += ( v[0] != 0.25 || v[1] != 0.75 || v[2] != 0.5625 );
  g.get(v);
  g.get(v);
  g.get(v);
  status += ( v[0] != 0.625 || v[1] != 0.125 || v[2] != 0.6875 );


  ts.status(status, "%s, Niederreiter d=3 (reinitialized)", g.name());
}

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("quasi_generator", argc, argv);

  test_sobol(ts);
  test_nied2(ts);

  return ts.summary() ? 0 : 1;
}

//____________________________________________________________________  
//
// EOF
//  
