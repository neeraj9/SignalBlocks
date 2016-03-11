//
// $Id: generator-test.cc,v 1.6 2006-05-01 14:24:57 cholm Exp $ 
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
#ifndef GSLMM_random_generator
#include <gslmm/random/generator.hh>
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

/** @file   generator-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of random number generator classses.  */

#define N2 200000
#define N  10000

//____________________________________________________________________  
void
random_test(gslmm::test_suite& ts,
	    gslmm::generator::backend t, 
	    unsigned long seed, 
	    size_t n, 
	    unsigned long result) 
{
  gslmm::generator r(t);
  if (seed) r.set(seed);
  unsigned long int k = 0;
  for (size_t i = 0; i < n; i++) k = r.get();
  ts.test(k, result, "%s, %u steps", r.name(), n);
}

//____________________________________________________________________  
void
float_test(gslmm::test_suite& ts,
	   gslmm::generator::backend_type t)
{
  gslmm::generator ri(t);
  gslmm::generator rf(t);
  
  unsigned long int k = 0;
  double            u = 0;
  
  do {
    k = ri.get();
    u = rf.get();
  }
  while (k == 0);
  
  bool status = true;
  
  double c = k / u;
  for (size_t i = 0; i < N2; i++) {
    k = ri.get();
    u = rf.get();
    if (c * k != u) status = false;
    break;
  }
  ts.test(c*k, u, "%s, ratio of int to double", ri.name());
}

//____________________________________________________________________  
void
state_test(gslmm::test_suite& ts,
	   gslmm::generator::backend_type t)
{
  unsigned long int test_a[N], test_b[N];
  
  gslmm::generator r(t);
  gslmm::generator r_save(t);

  for (size_t i = 0; i < N; i++) r.get();
  r_save = r;
  for (size_t i = 0; i < N; i++) test_a[i] = r.get();
  r = r_save;
  for (size_t i = 0; i < N; i++) test_b[i] = r.get();

  int status = 0;
  for (size_t i = 0; i < N; i++) status |= (test_b[i] != test_a[i]);
  
  ts.status(status, "%s, random number state consistency", 
	    r.name());
}


//____________________________________________________________________  
void
parallel_state_test(gslmm::test_suite& ts,
		    gslmm::generator::backend_type t)
{
  unsigned long int test_a[N], test_b[N];
  unsigned long int test_c[N], test_d[N];
  double test_e[N], test_f[N];
  
  gslmm::generator r1(t);
  gslmm::generator r2(t);

  for (size_t i = 0; i < N; i++) r1.get();
  r2 = r1;
  for (size_t i = 0; i < N; i++) {
    test_a[i] = r1.get();
    test_b[i] = r2.get();
    test_c[i] = r1.integer(1234);
    test_d[i] = r2.integer(1234);
    test_e[i] = r1.uniform();
    test_f[i] = r2.uniform();
  }
  
  int status = 0;
  for (size_t i = 0; i < N; i++) {
    status |= (test_b[i] != test_a[i]);
    status |= (test_d[i] != test_c[i]);
    status |= (test_e[i] != test_f[i]);
  }
  
  ts.status(status, "%s, parallel random number state consistency", 
	    r1.name());
}

//____________________________________________________________________  
int
max_test(gslmm::generator& r,
	 unsigned long int& kmax, 
	 unsigned long int  ran_max)
{
  unsigned long int max = 0;
  for (size_t i = 0; i < N2; ++i) {
    unsigned long int k = r.get();
    if (k > max) max = k;
  }
  kmax = max;

  unsigned long int actual_uncovered = ran_max - max;
  double            expect_uncovered = (double) ran_max / (double) N2;

  return  (max > ran_max) || (actual_uncovered > 7 * expect_uncovered);
}

//____________________________________________________________________  
int
min_test(gslmm::generator& r,
	 unsigned long int& kmin, 
	 unsigned long int  ran_min,
	 unsigned long int  ran_max)
{
  unsigned long int min = 1000000000UL;

  for (size_t i = 0; i < N2; ++i) {
    unsigned long int k = r.get();
    if (k < min) min = k;
  }
  kmin = min;

  unsigned long int actual_uncovered = min - ran_min;
  double            expect_uncovered = (double) ran_max / (double) N2;

  bool ret = ((min < ran_min) || (actual_uncovered > 7 * expect_uncovered));
#if 0
  if (!ret) 
    std::cout << " min:\t" << min 
	      << " ran_min:\t" << ran_min
	      << " actual:\t" << actual_uncovered 
	      << " expect:\t" << expect_uncovered
	      << std::endl;
#endif
  return  ret;
  
}

//____________________________________________________________________  
bool
sum_test(gslmm::test_suite& ts,
	 gslmm::generator& r) 
{
  double sum = 0;

  for (size_t i = 0; i < N2; ++i) {
    double x = r.uniform() - 0.5;
    sum += x;
  }
  sum /= N2;
 
  /* expect the average to have a variance of 1/(12 n) */
  double sigma = fabs(sum * std::sqrt(12.0 * N2)); 
  return ts.range(sigma, .003, 3., "%s, sum test within acceptable sigma",
		  r.name());

}

#define BINS 17
#define EXTRA 10
//____________________________________________________________________  
bool
bin_test(gslmm::test_suite& ts,
	 gslmm::generator& r) 
{
  int count[BINS+EXTRA];
 
  for (size_t i = 0; i < BINS+EXTRA; i++) count[i] = 0 ;
  for (size_t i = 0; i < N2; i++) {
    int j = r.integer(BINS);
    count[j]++ ;
  }

  double chisq = 0 ;
  for (size_t i = 0; i < BINS; i++) {
    double x =  (double)N2/(double)BINS ;
    double d =  (count[i] - x) ;
    chisq    += (d * d) / x;
  }

  bool ret;
  double sigma = sqrt(chisq/BINS) ;
  /* more than 3 sigma is an error */
  // status = (fabs (sigma) > 3 || fabs(sigma) < 0.003);
  ret = ts.factor(fabs(sigma), 1, 3, "%s, bin test within acceptable chisq", 
		  r.name());
  for (size_t i = BINS; i < BINS+EXTRA; i++) {
    ret = ts.test(count[i], 0, 
		  "%s, wrote outside range in bin test ", r.name());
  }
  return ret;
}

//____________________________________________________________________  
void
generic_test(gslmm::test_suite& ts,
	     gslmm::generator::backend_type t)
{
  gslmm::generator r(t);
  unsigned long int kmax = 0, kmin = 1000;
  const unsigned long int ran_max = r.max();
  const unsigned long int ran_min = r.min();

  int status;
  
  status = max_test(r, kmax, ran_max);
  ts.status(status, "%s, observed vs theoretical maximum",
	    r.name(), kmax, ran_max);

  status = min_test(r, kmin, ran_min, ran_max);
  ts.status(status, "%s, observed vs theoretical minimum",
	    r.name(), kmin, ran_min);

  sum_test(ts,r);
 
  bin_test (ts, r);
 
  status = 0;
  
  r.set(1);   
  status |= max_test(r, kmax, ran_max);
 
  r.set(1);   
  status |= min_test(r, kmin, ran_min, ran_max);
 
  r.set(1);   
  status |= (sum_test(ts,r) ? 0 : 1);
 
  r.set(12345);
  status |= max_test(r, kmax, ran_max);
 
  r.set(12345);
  status |= min_test(r, kmin, ran_min, ran_max);
 
  r.set(12345);
  status |= (sum_test(ts,r) ? 0 : 1);
 
  ts.status(status, "%s, maximum and sum tests for non-default seeds", 
	    r.name()); 
}
  

  
//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("generator", argc, argv);

  gslmm::generator::backend_type_list rngs = 
    gslmm::generator::types();
  
  /* specific tests of known results for 10000 iterations with seed = 1 */
  random_test(ts,gslmm::generator::rand, 1, 10000, 1910041713);
  random_test(ts,gslmm::generator::randu, 1, 10000, 1623524161);
  random_test(ts,gslmm::generator::cmrg, 1, 10000, 719452880);
  random_test(ts,gslmm::generator::minstd, 1, 10000, 1043618065);
  random_test(ts,gslmm::generator::mrg, 1, 10000, 2064828650);
  random_test(ts,gslmm::generator::taus, 1, 10000, 2733957125UL);
  random_test(ts,gslmm::generator::taus113, 1, 1000, 1925420673UL);
  random_test(ts,gslmm::generator::transputer, 1, 10000, 1244127297UL);
  random_test(ts,gslmm::generator::vax, 1, 10000, 3051034865UL);

  /* Borosh13 test value from PARI: (1812433253^10000)%(2^32) */
  random_test(ts,gslmm::generator::borosh13, 1, 10000, 2513433025UL);

  /* Fishman18 test value from PARI: (62089911^10000)%(2^31-1) */
  random_test(ts,gslmm::generator::fishman18, 1, 10000, 330402013UL);

  /* Fishman2x test value from PARI: 
     ((48271^10000)%(2^31-1) - (40692^10000)%(2^31-249))%(2^31-1) */
  random_test(ts,gslmm::generator::fishman2x, 1, 10000, 540133597UL);

  /* Knuthran2 test value from PARI: 
     { xn1=1; xn2=1; for (n=1,10000, 
            xn = (271828183*xn1 - 314159269*xn2)%(2^31-1);
            xn2=xn1; xn1=xn; print(xn); ) } */
  random_test(ts,gslmm::generator::knuthran2, 1, 10000, 1084477620UL);

  /* Knuthran test value taken from p188 in Knuth Vol 2. 3rd Ed */
  random_test(ts,gslmm::generator::knuthran, 310952, 1009 * 2009 + 1, 461390032);

  /* Lecuyer21 test value from PARI: (40692^10000)%(2^31-249) */
  random_test(ts,gslmm::generator::lecuyer21, 1, 10000, 2006618587UL);

  /* Waterman14 test value from PARI: (1566083941^10000)%(2^32) */
  random_test(ts,gslmm::generator::waterman14, 1, 10000, 3776680385UL);

  /* specific tests of known results for 10000 iterations with seed = 6 */

  /* Coveyou test value from PARI:
     x=6; for(n=1,10000,x=(x*(x+1))%(2^32);print(x);) */
  random_test(ts,gslmm::generator::coveyou, 6, 10000, 1416754246UL);

  /* Fishman20 test value from PARI: (6*48271^10000)%(2^31-1) */
  random_test(ts,gslmm::generator::fishman20, 6, 10000, 248127575UL);

  /* FIXME: the ranlux tests below were made by running the fortran code and
     getting the expected value from that. An analytic calculation
     would be preferable. */
  random_test(ts,gslmm::generator::ranlux, 314159265, 10000, 12077992);
  random_test(ts,gslmm::generator::ranlux389, 314159265, 10000, 165942);
  random_test(ts,gslmm::generator::ranlxs0, 1, 10000, 11904320);
  /* 0.709552764892578125 * ldexp(1.0,24) */
  random_test(ts,gslmm::generator::ranlxs1, 1, 10000, 8734328);
  /* 0.520606517791748047 * ldexp(1.0,24) */
  random_test(ts,gslmm::generator::ranlxs2, 1, 10000, 6843140); 
  /* 0.407882928848266602 * ldexp(1.0,24) */
  random_test(ts,gslmm::generator::ranlxd1, 1, 10000, 1998227290UL);
  /* 0.465248546261094020 * ldexp(1.0,32) */
  random_test(ts,gslmm::generator::ranlxd2, 1, 10000, 3949287736UL);
  /* 0.919515205581550532 * ldexp(1.0,32) */

  /* FIXME: the tests below were made by running the original code in
     the ../random directory and getting the expected value from
     that. An analytic calculation would be preferable. */
  random_test(ts,gslmm::generator::slatec, 1, 10000, 45776);
  random_test(ts,gslmm::generator::uni, 1, 10000, 9214);
  random_test(ts,gslmm::generator::uni32, 1, 10000, 1155229825);
  random_test(ts,gslmm::generator::zuf, 1, 10000, 3970);

  /* The tests below were made by running the original code and
     getting the expected value from that. An analytic calculation
     would be preferable. */
  random_test(ts,gslmm::generator::r250, 1, 10000, 1100653588);
  random_test(ts,gslmm::generator::mt19937, 4357, 1000, 1186927261);
  random_test(ts,gslmm::generator::mt19937_1999, 4357, 1000, 1030650439);
  random_test(ts,gslmm::generator::mt19937_1998, 4357, 1000, 1309179303);
  random_test(ts,gslmm::generator::tt800, 0, 10000, 2856609219UL);

  random_test(ts,gslmm::generator::ran0, 0, 10000, 1115320064);
  random_test(ts,gslmm::generator::ran1, 0, 10000, 1491066076);
  random_test(ts,gslmm::generator::ran2, 0, 10000, 1701364455);
  random_test(ts,gslmm::generator::ran3, 0, 10000, 186340785);

  random_test(ts,gslmm::generator::ranmar, 1, 10000, 14428370);

  random_test(ts,gslmm::generator::rand48, 0, 10000, 0xDE095043UL);
  random_test(ts,gslmm::generator::rand48, 1, 10000, 0xEDA54977UL);

  random_test(ts,gslmm::generator::random_glibc2, 0, 10000, 1908609430);
  random_test(ts,gslmm::generator::random8_glibc2, 0, 10000, 1910041713);
  random_test(ts,gslmm::generator::random32_glibc2, 0, 10000, 1587395585);
  random_test(ts,gslmm::generator::random64_glibc2, 0, 10000, 52848624);
  random_test(ts,gslmm::generator::random128_glibc2, 0, 10000, 1908609430);
  random_test(ts,gslmm::generator::random256_glibc2, 0, 10000, 179943260);

  random_test(ts,gslmm::generator::random_bsd, 0, 10000, 1457025928);
  random_test(ts,gslmm::generator::random8_bsd, 0, 10000, 1910041713);
  random_test(ts,gslmm::generator::random32_bsd, 0, 10000, 1663114331);
  random_test(ts,gslmm::generator::random64_bsd, 0, 10000, 864469165);
  random_test(ts,gslmm::generator::random128_bsd, 0, 10000, 1457025928);
  random_test(ts,gslmm::generator::random256_bsd, 0, 10000, 1216357476);

  random_test(ts,gslmm::generator::random_libc5, 0, 10000, 428084942);
  random_test(ts,gslmm::generator::random8_libc5, 0, 10000, 1910041713);
  random_test(ts,gslmm::generator::random32_libc5, 0, 10000, 1967452027);
  random_test(ts,gslmm::generator::random64_libc5, 0, 10000, 2106639801);
  random_test(ts,gslmm::generator::random128_libc5, 0, 10000, 428084942);
  random_test(ts,gslmm::generator::random256_libc5, 0, 10000, 116367984);

  random_test(ts,gslmm::generator::ranf, 0, 10000, 2152890433UL);
  random_test(ts,gslmm::generator::ranf, 2, 10000, 339327233);

  /* Test constant relationship between int and double functions */
  for (gslmm::generator::backend_type_list::iterator r = rngs.begin(); 
       r != rngs.end(); ++r) float_test(ts,*r);

  /* Test save/restore functions */
  for (gslmm::generator::backend_type_list::iterator r = rngs.begin(); 
       r != rngs.end(); ++r) state_test(ts,*r);
  for (gslmm::generator::backend_type_list::iterator r = rngs.begin(); 
       r != rngs.end(); ++r) parallel_state_test(ts,*r);

  /* generic statistical tests (these are just to make sure that we
     don't get any crazy results back from the generator, i.e. they
     aren't a test of the algorithm, just the implementation) */
  for (gslmm::generator::backend_type_list::iterator r = rngs.begin(); 
       r != rngs.end(); ++r) generic_test(ts,*r);

  return ts.summary() ? 0 : 1;
}

//____________________________________________________________________  
//
// EOF
//  
