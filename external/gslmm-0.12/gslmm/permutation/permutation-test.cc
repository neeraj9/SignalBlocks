//
// $Id: permutation-test.cc,v 1.7 2004-11-06 00:49:46 cholm Exp $ 
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
#ifndef GSLMM_permutation
#include <gslmm/permutation/permutation.hh>
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

/** @file   permutation-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of permutation classses.  */


void print_auth(unsigned int* p) 
{
  std::cout << "(";
  for (int i = 0; i < 5; i++) std::cout << std::setw(3) << p[i];
  std::cout << ")" << std::endl;
}

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("permutation", argc, argv);

  unsigned int p5[120][5] = {
    {0, 1, 2, 3, 4}, {0, 1, 2, 4, 3}, {0, 1, 3, 2, 4}, {0, 1, 3, 4, 2},
    {0, 1, 4, 2, 3}, {0, 1, 4, 3, 2}, {0, 2, 1, 3, 4}, {0, 2, 1, 4, 3},
    {0, 2, 3, 1, 4}, {0, 2, 3, 4, 1}, {0, 2, 4, 1, 3}, {0, 2, 4, 3, 1},
    {0, 3, 1, 2, 4}, {0, 3, 1, 4, 2}, {0, 3, 2, 1, 4}, {0, 3, 2, 4, 1},
    {0, 3, 4, 1, 2}, {0, 3, 4, 2, 1}, {0, 4, 1, 2, 3}, {0, 4, 1, 3, 2},
    {0, 4, 2, 1, 3}, {0, 4, 2, 3, 1}, {0, 4, 3, 1, 2}, {0, 4, 3, 2, 1},
    {1, 0, 2, 3, 4}, {1, 0, 2, 4, 3}, {1, 0, 3, 2, 4}, {1, 0, 3, 4, 2},
    {1, 0, 4, 2, 3}, {1, 0, 4, 3, 2}, {1, 2, 0, 3, 4}, {1, 2, 0, 4, 3},
    {1, 2, 3, 0, 4}, {1, 2, 3, 4, 0}, {1, 2, 4, 0, 3}, {1, 2, 4, 3, 0},
    {1, 3, 0, 2, 4}, {1, 3, 0, 4, 2}, {1, 3, 2, 0, 4}, {1, 3, 2, 4, 0},
    {1, 3, 4, 0, 2}, {1, 3, 4, 2, 0}, {1, 4, 0, 2, 3}, {1, 4, 0, 3, 2},
    {1, 4, 2, 0, 3}, {1, 4, 2, 3, 0}, {1, 4, 3, 0, 2}, {1, 4, 3, 2, 0},
    {2, 0, 1, 3, 4}, {2, 0, 1, 4, 3}, {2, 0, 3, 1, 4}, {2, 0, 3, 4, 1},
    {2, 0, 4, 1, 3}, {2, 0, 4, 3, 1}, {2, 1, 0, 3, 4}, {2, 1, 0, 4, 3},
    {2, 1, 3, 0, 4}, {2, 1, 3, 4, 0}, {2, 1, 4, 0, 3}, {2, 1, 4, 3, 0},
    {2, 3, 0, 1, 4}, {2, 3, 0, 4, 1}, {2, 3, 1, 0, 4}, {2, 3, 1, 4, 0},
    {2, 3, 4, 0, 1}, {2, 3, 4, 1, 0}, {2, 4, 0, 1, 3}, {2, 4, 0, 3, 1},
    {2, 4, 1, 0, 3}, {2, 4, 1, 3, 0}, {2, 4, 3, 0, 1}, {2, 4, 3, 1, 0},
    {3, 0, 1, 2, 4}, {3, 0, 1, 4, 2}, {3, 0, 2, 1, 4}, {3, 0, 2, 4, 1},
    {3, 0, 4, 1, 2}, {3, 0, 4, 2, 1}, {3, 1, 0, 2, 4}, {3, 1, 0, 4, 2},
    {3, 1, 2, 0, 4}, {3, 1, 2, 4, 0}, {3, 1, 4, 0, 2}, {3, 1, 4, 2, 0},
    {3, 2, 0, 1, 4}, {3, 2, 0, 4, 1}, {3, 2, 1, 0, 4}, {3, 2, 1, 4, 0},
    {3, 2, 4, 0, 1}, {3, 2, 4, 1, 0}, {3, 4, 0, 1, 2}, {3, 4, 0, 2, 1},
    {3, 4, 1, 0, 2}, {3, 4, 1, 2, 0}, {3, 4, 2, 0, 1}, {3, 4, 2, 1, 0},
    {4, 0, 1, 2, 3}, {4, 0, 1, 3, 2}, {4, 0, 2, 1, 3}, {4, 0, 2, 3, 1},
    {4, 0, 3, 1, 2}, {4, 0, 3, 2, 1}, {4, 1, 0, 2, 3}, {4, 1, 0, 3, 2},
    {4, 1, 2, 0, 3}, {4, 1, 2, 3, 0}, {4, 1, 3, 0, 2}, {4, 1, 3, 2, 0},
    {4, 2, 0, 1, 3}, {4, 2, 0, 3, 1}, {4, 2, 1, 0, 3}, {4, 2, 1, 3, 0},
    {4, 2, 3, 0, 1}, {4, 2, 3, 1, 0}, {4, 3, 0, 1, 2}, {4, 3, 0, 2, 1},
    {4, 3, 1, 0, 2}, {4, 3, 1, 2, 0}, {4, 3, 2, 0, 1}, {4, 3, 2, 1, 0}
  } ;

  unsigned int c5[120][5] = {
    {4, 3, 2, 1, 0}, {3, 4, 2, 1, 0}, {4, 2, 3, 1, 0}, {2, 3, 4, 1, 0},
    {2, 4, 3, 1, 0}, {3, 2, 4, 1, 0}, {4, 3, 1, 2, 0}, {3, 4, 1, 2, 0},
    {4, 1, 2, 3, 0}, {1, 2, 3, 4, 0}, {1, 2, 4, 3, 0}, {3, 1, 2, 4, 0},
    {4, 1, 3, 2, 0}, {1, 3, 4, 2, 0}, {4, 2, 1, 3, 0}, {2, 1, 3, 4, 0},
    {2, 4, 1, 3, 0}, {1, 3, 2, 4, 0}, {1, 4, 3, 2, 0}, {3, 1, 4, 2, 0},
    {2, 1, 4, 3, 0}, {3, 2, 1, 4, 0}, {1, 4, 2, 3, 0}, {2, 3, 1, 4, 0},
    {4, 3, 2, 0, 1}, {3, 4, 2, 0, 1}, {4, 2, 3, 0, 1}, {2, 3, 4, 0, 1},
    {2, 4, 3, 0, 1}, {3, 2, 4, 0, 1}, {4, 3, 0, 1, 2}, {3, 4, 0, 1, 2},
    {4, 0, 1, 2, 3}, {0, 1, 2, 3, 4}, {0, 1, 2, 4, 3}, {3, 0, 1, 2, 4},
    {4, 0, 1, 3, 2}, {0, 1, 3, 4, 2}, {4, 2, 0, 1, 3}, {2, 0, 1, 3, 4},
    {2, 4, 0, 1, 3}, {0, 1, 3, 2, 4}, {0, 1, 4, 3, 2}, {3, 0, 1, 4, 2},
    {2, 0, 1, 4, 3}, {3, 2, 0, 1, 4}, {0, 1, 4, 2, 3}, {2, 3, 0, 1, 4},
    {4, 3, 0, 2, 1}, {3, 4, 0, 2, 1}, {4, 0, 2, 3, 1}, {0, 2, 3, 4, 1},
    {0, 2, 4, 3, 1}, {3, 0, 2, 4, 1}, {4, 3, 1, 0, 2}, {3, 4, 1, 0, 2},
    {4, 1, 0, 2, 3}, {1, 0, 2, 3, 4}, {1, 0, 2, 4, 3}, {3, 1, 0, 2, 4},
    {4, 1, 3, 0, 2}, {1, 3, 4, 0, 2}, {4, 0, 2, 1, 3}, {0, 2, 1, 3, 4},
    {0, 2, 4, 1, 3}, {1, 3, 0, 2, 4}, {1, 4, 3, 0, 2}, {3, 1, 4, 0, 2},
    {0, 2, 1, 4, 3}, {3, 0, 2, 1, 4}, {1, 4, 0, 2, 3}, {0, 2, 3, 1, 4},
    {4, 0, 3, 2, 1}, {0, 3, 4, 2, 1}, {4, 2, 0, 3, 1}, {2, 0, 3, 4, 1},
    {2, 4, 0, 3, 1}, {0, 3, 2, 4, 1}, {4, 1, 0, 3, 2}, {1, 0, 3, 4, 2},
    {4, 2, 1, 0, 3}, {2, 1, 0, 3, 4}, {2, 4, 1, 0, 3}, {1, 0, 3, 2, 4},
    {4, 0, 3, 1, 2}, {0, 3, 4, 1, 2}, {4, 1, 2, 0, 3}, {1, 2, 0, 3, 4},
    {1, 2, 4, 0, 3}, {0, 3, 1, 2, 4}, {0, 3, 1, 4, 2}, {1, 4, 0, 3, 2},
    {1, 4, 2, 0, 3}, {0, 3, 2, 1, 4}, {2, 1, 4, 0, 3}, {2, 0, 3, 1, 4},
    {0, 4, 3, 2, 1}, {3, 0, 4, 2, 1}, {2, 0, 4, 3, 1}, {3, 2, 0, 4, 1},
    {0, 4, 2, 3, 1}, {2, 3, 0, 4, 1}, {1, 0, 4, 3, 2}, {3, 1, 0, 4, 2},
    {2, 1, 0, 4, 3}, {3, 2, 1, 0, 4}, {1, 0, 4, 2, 3}, {2, 3, 1, 0, 4},
    {0, 4, 3, 1, 2}, {3, 0, 4, 1, 2}, {1, 2, 0, 4, 3}, {3, 1, 2, 0, 4},
    {0, 4, 1, 2, 3}, {1, 2, 3, 0, 4}, {1, 3, 0, 4, 2}, {0, 4, 1, 3, 2},
    {0, 4, 2, 1, 3}, {1, 3, 2, 0, 4}, {2, 0, 4, 1, 3}, {2, 1, 3, 0, 4}
  } ;

  unsigned int cycles[120] = {
    5, 4, 4, 3, 3, 4, 4, 3, 3, 2,
    2, 3, 3, 2, 4, 3, 3, 2, 2, 3,
    3, 4, 2, 3, 4, 3, 3, 2, 2, 3,
    3, 2, 2, 1, 1, 2, 2, 1, 3, 2,
    2, 1, 1, 2, 2, 3, 1, 2, 3, 2,
    2, 1, 1, 2, 4, 3, 3, 2, 2, 3,
    3, 2, 2, 1, 1, 2, 2, 3, 1, 2,
    2, 1, 2, 1, 3, 2, 2, 1, 3, 2,
    4, 3, 3, 2, 2, 1, 3, 2, 2, 1,
    1, 2, 2, 1, 3, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 3, 4, 2, 3, 1, 2,
    2, 3, 1, 2, 2, 1, 1, 2, 2, 3
  } ;

  unsigned int inversions[120] = {
    0, 1, 1, 2, 2, 3, 1, 2, 2, 3,
    3, 4, 2, 3, 3, 4, 4, 5, 3, 4,
    4, 5, 5, 6, 1, 2, 2, 3, 3, 4,
    2, 3, 3, 4, 4, 5, 3, 4, 4, 5,
    5, 6, 4, 5, 5, 6, 6, 7, 2, 3,
    3, 4, 4, 5, 3, 4, 4, 5, 5, 6,
    4, 5, 5, 6, 6, 7, 5, 6, 6, 7,
    7, 8, 3, 4, 4, 5, 5, 6, 4, 5,
    5, 6, 6, 7, 5, 6, 6, 7, 7, 8,
    6, 7, 7, 8, 8, 9, 4, 5, 5, 6,
    6, 7, 5, 6, 6, 7, 7, 8, 6, 7,
    7, 8, 8, 9, 7, 8, 8, 9, 9, 10
  } ;

  int i      = 0;
  int status = 0;
  gslmm::permutation p(5);
  do {
    for (int j = 0; j < 5; j++) status |= (p[j] != p5[i][j]);
    i++;
  }
  while (p.next());
    
  ts.test(status, 0, "gslmm::permutation::next, "
	  "5-th order permutation, 120 steps");

  do {
    i--;
    for (int j = 0; j < 5; j++) status |= (p[j] != p5[i][j]);
  }
  while (p.previous());
    
  ts.test(status, 0, "gslmm::permutation::prev, "
	  "5-th order permutation, 120 steps");

  i      = 0;
  status = 0 ;
  
  gslmm::permutation p1(5);
  gslmm::permutation p2(5);
 
  std::vector<double> v(5);
  v[0] = 100;
  v[1] = 101;
  v[2] = 102;
  v[3] = 103;
  v[4] = 104;
  
  // double v[5] = { 100.0, 101.0, 102.0, 103.0, 104.0 } ;
  
  do {
    p2.identity();
    do {
      std::vector<double> x(5);
      std::vector<double> y(5);
      for (int j = 0; j < 5; j++) x[j] = y[j] = v[j];

      // Compute x= p1 p2 v
      p2.permute(x, false);
      p1.permute(x, false);

      // Compute P= p1 p2, y = P v 
      gslmm::permutation q =  p1 * p2;
      q.permute(y, false);

      for (int j = 0; j < 5; j++) if (x[j] != y[j]) status = 1;

      if (status == 1) break;
    } while (p2.next());
        
    if (status == 1) break;
  }
  while (p1.next());
  ts.test(status, 0, "gslmm::permutation::operator*, "
	  "all 5-th order combinations");

  // testing cycles representations 
  i      = 0;
  status = 0;

  gslmm::permutation  p3(5);
  gslmm::permutation  plin(5);
  gslmm::permutation  pcan(5);
    
  do {
    int s = 0;
    plin  = p3;
    
    pcan = plin.canonical();
    for (int j = 0; j < 5; j++) s |= (pcan[j] != c5[i][j]);
    
    s |= (pcan.cycles() != cycles[i]);
    s |= (plin.cycles() != cycles[i]);

    plin = pcan.linear();    
    for (int j = 0; j < 5; j++) s |= (plin[j] != p5[i][j]);
    
    i++;
    status |= s;
  }
  while (p3.next());

  ts.test(status, 0, "gslmm::permutation canonical conversion, "
	  "5-th order permutation, 120 steps");

  // testing number of inversions 
  i      = 0;
  status = 0;

  gslmm::permutation p4(5);

  do {	
    status |= p4.inversions() != inversions[i];
    i++;
  }
  while (p4.next());
  
  ts.test(status, 0, "gslmm::permutation::inversions, "
	  "5-th order permutation, 120 steps");

  return ts.summary() ? 0 : 1;
}

//
// EOF
//  
