//
// $Id: combination-test.cc,v 1.4 2004-06-01 13:43:18 cholm Exp $ 
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
#ifndef GSLMM_combination
#include <gslmm/combination/combination.hh>
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

/** @file   combination-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of combination classses.  */

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("combination", argc, argv);

  int status = 0;
  size_t c63[20][3] = {
    { 0, 1, 2 },  { 0, 1, 3 },  { 0, 1, 4 },  { 0, 1, 5 },
    { 0, 2, 3 },  { 0, 2, 4 },  { 0, 2, 5 },  { 0, 3, 4 },
    { 0, 3, 5 },  { 0, 4, 5 },  { 1, 2, 3 },  { 1, 2, 4 },
    { 1, 2, 5 },  { 1, 3, 4 },  { 1, 3, 5 },  { 1, 4, 5 },
    { 2, 3, 4 },  { 2, 3, 5 },  { 2, 4, 5 },  { 3, 4, 5 }
  };

  gslmm::combination c1(6,3, false);
  c1.first();

  int i = 0;
  do {
    if (i >= 20) {
      status = 1;
      break;
    }
    for (int j = 0; j < 3; j++) status |= (c1[j] != c63[i][j]);
    i++;
  } while (c1.next());
  ts.test(status, 0, "gslmm::combination::next, "
	  "6 choose 3 combination, 20 steps");

  c1.next(); c1.next(); c1.next();
  for (int j = 0; j < 3; j++) status |= (c1[j] != c63[19][j]);
  ts.test(status, 0, "gslmm::combination::next on the last combination");


  c1.last();
  i = 20;
  do {
    if (i == 0) {
      status = 1;
      break;
    }
    i--;
    for (int j = 0; j < 3; j++) status |= (c1[j] != c63[i][j]);
  } while (c1.previous());
  ts.test(status, 0, "gslmm::combination::prev, "
	  "6 choose 3 combination, 20 steps");

  c1.previous();
  c1.previous();
  c1.previous();
  for (int j = 0; j < 3; j++) status |= (c1[j] != c63[0][j]);
  ts.test(status, 0, "gslmm::combination::prev on the first combination");

  gslmm::combination c2(7, 0);
  // should return false every time 
  status |= (c2.next() != false);
  status |= (c2.next() != false);
  status |= (c2.previous() != false);
  status |= (c2.previous() != false);
  ts.test(status, 0, "gslmm::combination 7 choose 0");

  gslmm::combination c3(7, 7);
  // should return false every time 
  for(size_t j = 0; j < 7; j++) status |= (c3[j] != j);
  status |= (c3.next() != false);
  for(size_t j = 0; j < 7; j++) status |= (c3[j] != j);
  status |= (c3.next() != false);
  for(size_t j = 0; j < 7; j++) status |= (c3[j] != j);
  status |= (c3.previous() != false); 
  for(size_t j = 0; j < 7; j++) status |= (c3[j] != j);
  status |= (c3.previous() != false);
  for(size_t j = 0; j < 7; j++) status |= (c3[j] != j);
  ts.test(status, 0, "gslmm::combination 7 choose 7");

  return ts.summary() ? 0 : 1;
}

//
// EOF
//  
