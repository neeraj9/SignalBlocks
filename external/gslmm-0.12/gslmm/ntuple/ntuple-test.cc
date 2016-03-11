//
// $Id: ntuple-test.cc,v 1.7 2006-05-01 14:24:57 cholm Exp $ 
//  
//  ntuple/ntuple-test.cc
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
#ifndef GSLMM_ntuple
#include <gslmm/ntuple/ntuple.hh>
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

/** @file   ntuple-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of ntuple classses.  */

//____________________________________________________________________  
struct data 
{
  int    num;
  double x;
  double y;
  double z;
};

//____________________________________________________________________  
struct selector 
{
  double _scale;
  selector(double s=1.5) : _scale(s) {}
  bool operator()(const data& d) const
  {
    bool ret = (d.x * _scale < .1);
    return ret;
  }
};

//____________________________________________________________________  
struct value 
{
  double _scale;
  value(double s=1.5) : _scale(s) {}
  double operator()(const data& d) const
  {
    return (d.x + d.y + d.z) * _scale;
  }
};

//____________________________________________________________________  
struct value2 
{
  double _scale;
  value2(double s=1.5) : _scale(s) {}
  std::pair<double,double> operator()(const data& d) const
  {
    return std::pair<double,double>((d.x + d.y + d.z) * _scale, 
				    d.x);
  }
};

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("ntuple", argc, argv);

  //__________________________________________________________________
  data row;
  const double                    scale = 1.5;
  std::vector<double>             x(1000);
  std::vector<double>             y(1000);
  std::vector<double>             z(1000);
  gslmm::histogram<double,size_t> h_out(100, 0, 1);
  gslmm::ntuple<data>             n_out("ntuple-test.dat",&row,
					gslmm::ntuple<data>::create);
  selector s(scale);
  value    v(scale);
  value2   v2(scale);

  bool status = true;
  for (int i = 0; i < 1000; i++) {
    row.num = i;
    row.x   = x[i] = 1. / (i + 1);
    row.y   = y[i] = x[i] * x[i];
    row.z   = z[i] = y[i] * x[i];
    
    if (s(row)) h_out.fill(v(row));
    status = n_out.fill();
  }
  n_out.close();
  ts.check(status, "gslmm::ntuple::fill");
  
  //__________________________________________________________________
  gslmm::ntuple<data> n_in("ntuple-test.dat",&row,gslmm::ntuple<data>::open);
  data* irow = 0;
  int i = 0;
  while ((irow = n_in.read())) {
    status =  (irow->num == i);
    status |= (irow->x == x[i]);
    status |= (irow->y == y[i]);
    status |= (irow->z == z[i]);
    i++;
  }
  n_in.close();
  ts.check(status, "gslmm::ntuple::read");

  
  //__________________________________________________________________
  gslmm::ntuple<data> n_in2("ntuple-test.dat",&row,gslmm::ntuple<data>::open);
  gslmm::histogram<double,size_t> h_in(100, 0, 1);
  status = n_in2.project(h_in, v, s);
  for (int i = 0; i < 100; i++) 
    if (h_in[i] != h_out[i]) status = false;
  ts.check(status, "gslmm::ntuple::project");

  //__________________________________________________________________
  gslmm::ntuple<data> n_in3("ntuple-test.dat",&row,gslmm::ntuple<data>::open);
  gslmm::histogram<std::pair<double,double>,
                   std::pair<size_t,size_t> > 
    h2_in(std::pair<size_t,size_t>(100,100), 
	  std::pair<double,double>(0, 0), 
	  std::pair<double,double>(1, 1));
  status = n_in3.project(h2_in, v2, s);
  ts.check(status, "gslmm::ntuple::project");
    
  return ts.summary() ? 0 : 1;
}

//____________________________________________________________________  
//
// EOF
//  
