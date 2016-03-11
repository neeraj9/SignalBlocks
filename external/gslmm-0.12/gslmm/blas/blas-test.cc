
//
// $Id: blas-test.cc,v 1.13 2006-05-01 14:24:13 cholm Exp $ 
//  
//  blas/blas-test.cc
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
#ifndef GSLMM_blas
#include <gslmm/blas/blas.hh>
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


extern void do_test_amax(gslmm::test_suite& ts);
extern void do_test_asum(gslmm::test_suite& ts);
extern void do_test_gemv(gslmm::test_suite& ts);
extern void do_test_symv(gslmm::test_suite& ts);
extern void do_test_hemv(gslmm::test_suite& ts);
extern void do_test_trmv(gslmm::test_suite& ts);
extern void do_test_gemm(gslmm::test_suite& ts);
extern void do_test_symm(gslmm::test_suite& ts);
extern void do_test_hemm(gslmm::test_suite& ts);
extern void do_test_trmm(gslmm::test_suite& ts);
extern void do_test_herk(gslmm::test_suite& ts);
extern void do_test_her2k(gslmm::test_suite& ts);
extern void do_test_her(gslmm::test_suite& ts);
extern void do_test_her2(gslmm::test_suite& ts);
extern void do_test_syrk(gslmm::test_suite& ts);
extern void do_test_syr2k(gslmm::test_suite& ts);
extern void do_test_syr(gslmm::test_suite& ts);
extern void do_test_syr2(gslmm::test_suite& ts);
extern void do_test_ger(gslmm::test_suite& ts);

/** @file   blas-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of blas classses.  
    @ingroup blas 
    @todo Implement tests of BLAS level 1 
*/

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("blas", argc, argv);

  gslmm::vector<float> v1(2);
  gslmm::vector<float> v2(2);
  v1[0] = v1[1] = 1;
  v2[0] = v2[1] = 2;

  float v1dotv2 = dot(v1, v2);
  (void)v1dotv2;
  // std::cout << v1dotv2 << std::endl;
  
  // Test trmm 
  ts.message("Level 1");
  do_test_amax(ts);
  do_test_asum(ts);
  ts.message("Level 2");
  do_test_gemv(ts);
  do_test_symv(ts);
  do_test_hemv(ts);
  do_test_trmv(ts);
  do_test_ger(ts);
  do_test_syr(ts);
  do_test_syr2(ts);
  do_test_her(ts);
  do_test_her2(ts);
  ts.message("Level 3");
  do_test_gemm(ts);
  do_test_symm(ts);
  do_test_hemm(ts);
  do_test_syrk(ts);
  do_test_syr2k(ts);
  do_test_herk(ts);
  do_test_her2k(ts);
  do_test_trmm(ts);
  
  return ts.summary() ? 0 : 1;
}

//____________________________________________________________________  
//
// EOF
//  
