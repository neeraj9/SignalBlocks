//
// $Id: statistic-test.cc,v 1.6 2006-05-01 14:24:57 cholm Exp $ 
//  
//  statistic/statistic-test.cc
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
#ifndef GSLMM_statistic
#include <gslmm/statistic/statistic.hh>
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

/** @file   statistic-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of statistic classses.  */

using namespace gslmm;

template <typename T> 
void
testit(test_suite& ts, T rel) 
{
  typedef typename dataset<T>::return_type    r_t;
  typedef typename dataset<T>::data_type      d_t;
  typedef typename dataset<T>::container_type c_t;
  
  dataset<T> a(14);
  dataset<T> b(14);
  weighted_dataset<T> wa(14);

  const T rawa[] = { .0421, .0941, .1064, .0242, .1331,
		     .0773, .0243, .0815, .1186, .0356,
		     .0728, .0999, .0614, .0479 };
  const T rawb[] = { .1081, .0986, .1566, .1961, .1125,
		     .1942, .1079, .1021, .1583, .1673,
		     .1675, .1856, .1688, .1512 };

  const T raww[] = {  .000,  .000,  .000, 3.000, .0000,
		      1.000, 1.000, 1.000, 0.000, .5000,
		      7.000, 5.000, 4.000, 0.123};
  for (size_t i = 0; i < 14; i++) {
    a[i]  = rawa[i];
    b[i]  = rawb[i];
    wa[i] = rawa[i];
    wa.weight(i) = raww[i];
  }

  {
    r_t mean = a.mean();
    r_t expected = 0.0728;
    ts.relative(mean, expected, rel,  "mean");
  }

  {
    r_t mean = a.mean();
    r_t var = a.variance(mean, true);
    r_t expected = 0.00113837428571429;
    ts.relative(var, expected, rel,  "variance with fixed mean");
  }


  {
    r_t mean = a.mean();
    r_t var = a.standard_deviation(mean, true);
    r_t expected = 0.0337398026922845;
    ts.relative(var, expected, rel,  "standard deviation with fixed mean");
  }


  {
    r_t var = b.variance();
    r_t expected = 0.00124956615384615;
    ts.relative(var, expected, rel,  "variance");
  }

  {
    r_t sd = a.standard_deviation();
    r_t expected = 0.0350134479659107;
    ts.relative(sd, expected, rel,  "sd");
  }

  {
    r_t absdev = a.absolute_deviation();
    r_t expected = 0.0287571428571429;
    ts.relative(absdev, expected, rel,  "absolute deviation");
  }

  {
    r_t skew = a.skew();
    r_t expected = 0.0954642051479004;
    ts.relative(skew, expected, rel,  "skew");
  }

  {
    r_t kurt = a.kurtosis();
    r_t expected = -1.38583851548909 ;
    ts.relative(kurt, expected, rel,  "kurtosis");
  }

  {
    r_t wmean = wa.mean();
    r_t expected = 0.0678111523670601;
    ts.relative(wmean, expected, rel,  "wmean");
  }

  {
    r_t wmean = wa.mean();
    r_t wvar = wa.variance(wmean, true);
    r_t expected = 0.000615793060878654;
    ts.relative(wvar, expected, rel,  "variance_with_fixed_mean");
  }

  {
    r_t est_wvar = wa.variance();
    r_t expected = 0.000769562962860317;
    ts.relative(est_wvar, expected, rel,  "variance");
  }

  {
    r_t wsd = wa.standard_deviation();
    r_t expected = 0.0277409978706664;
    ts.relative(wsd, expected, rel,  "wsd");
  }

  {
    r_t wabsdev = wa.absolute_deviation();
    r_t expected = 0.0193205027504008;
    ts.relative(wabsdev, expected, rel,  "wabsdev");
  }

  {
    r_t wskew = wa.skew();
    r_t expected = -0.373631000307076;
    ts.relative(wskew, expected, rel,  "wskew");
  }

  {
    r_t wkurt = wa.kurtosis();
    r_t expected = -1.48114233353963;
    ts.relative(wkurt, expected, rel,  "wkurtosis");
  }

  {
    r_t c = a.covariance(b);
    r_t expected = -0.000139021538461539;
    ts.relative(c, expected, rel,  "covariance");
  }


  {
    r_t pv = a.pooled_variance(b);
    r_t expected = 0.00123775384615385;
    ts.relative(pv, expected, rel,  "pooled variance");
  }

  {
    r_t t = a.t_test(b);
    r_t expected = -5.67026326985851;
    ts.relative(t, expected, rel,  "ttest");
  }

  {
    r_t expected = 0.1331;
    r_t max      = a.max();
    ts.test(max, expected, "max");
  }
  
  {
    r_t min      = a.min();
    r_t expected = 0.0242;
    ts.test(min, expected, "min");
  }

  {
    std::pair<r_t,r_t> expected(0.0242, 0.1331);
    std::pair<r_t,r_t> minmax = a.minmax();
    ts.test(minmax.first, expected.first, "minmax min");
    ts.test(minmax.second, expected.second, "minmax max");
  }

  {
    size_t max_index = wa.max_index();
    size_t expected = 4;
    ts.test(max_index, expected, "max_index");
  }

  {
    size_t min_index = wa.min_index();
    size_t expected = 3;
    ts.test(min_index, expected,"min_index");
  }

  {
    std::pair<size_t,size_t> expected(3,4);
    std::pair<size_t,size_t> minmax = a.minmax_index();
    ts.test(minmax.first, expected.first, "minmax_index min");
    ts.test(minmax.second, expected.second, "minmax_index max");
  }


  a.sort();
  dataset<T> a2(13);
  for (size_t i = 0; i < a2.size(); i++) a2[i] = a[i];
  {
    r_t median = a.median();
    r_t expected = 0.07505;
    ts.relative(median,expected, rel,  "median_from_sorted_data (even)");
  }

  {
    r_t median = a2.median();
    r_t expected = 0.0728;
    ts.relative(median,expected, rel,  "median_from_sorted_data");
  }


  {
    r_t zeroth = a.quantile(0.0);
    r_t expected = 0.0242;
    ts.relative(zeroth,expected, rel,  "quantile_from_sorted_data (0)");
  }

  {
    r_t top = a.quantile(1.0);
    r_t expected = 0.1331;
    ts.relative(top,expected, rel,  "quantile_from_sorted_data (100)");
  }

  {
    r_t median = a.quantile(0.5);
    r_t expected = 0.07505;
    ts.relative(median,expected, rel,  "quantile_from_sorted_data (50even)");
  }

  {
    r_t median = a2.quantile(0.5);
    r_t expected = 0.0728;
    ts.relative(median,expected, rel,  "quantile_from_sorted_data (50odd)");
  }
}

//____________________________________________________________________  
template <typename T>
void
testit_int(test_suite& ts, double rel)
{
  typedef typename dataset<T>::return_type    r_t;
  typedef typename dataset<T>::data_type      d_t;
  typedef typename dataset<T>::container_type c_t;
  
  dataset<T> a(20);
  dataset<T> b(20);
  dataset<T> c(6);
  

  const T raw1[] = {1, 2, 3, 4, 5, 6} ;
  for (size_t i = 0; i < 6; i++) c[i] = raw1[i];
  
  
  const T irawa[] = {17, 18, 16, 18, 12, 20, 18, 20, 20, 22,
		     20, 10,  8, 12, 16, 16, 18, 20, 18, 21};
  const T irawb[] = {19, 20, 22, 24, 10, 25, 20, 22, 21, 23,
		     20, 10, 12, 14, 12, 20, 22, 24, 23, 17};
  for (size_t i = 0; i < 20; i++) {
    a[i] = irawa[i];
    b[i] = irawb[i];
  }

  {
    r_t mean = a.mean();
    r_t expected = 17.0;
    ts.relative(mean,expected, rel, "mean (integer)");
  }

  {
    r_t mean = c.mean();
    r_t expected = 3.5;
    ts.relative(mean,expected, rel, "mean (fractional)");
  }

  {
    r_t mean = a.mean();
    r_t var = a.variance(mean, true);
    r_t expected = 13.7;
    ts.relative(var, expected, rel, "variance_with_fixed_mean");
  }

  {
    r_t mean = a.mean();
    r_t sd = a.standard_deviation(mean, true);
    r_t expected = 3.70135110466435;
    ts.relative(sd, expected, rel, "sd_with_fixed_mean");
  }

  {
    r_t var = a.variance();
    r_t expected = 14.4210526315789;
    ts.relative(var, expected, rel, "variance");
  }

  {
    r_t sd_est = a.standard_deviation();
    r_t expected = 3.79750610685209;
    ts.relative(sd_est, expected, rel, "sd");
  }

  {
    r_t absdev = a.absolute_deviation();
    r_t expected = 2.9;
    ts.relative(absdev, expected, rel, "absdev");
  }

  {
    r_t skew = a.skew();
    r_t expected = -0.909355923168064;
    ts.relative(skew, expected, rel, "skew");
  }

  {
    r_t kurt = a.kurtosis();
    r_t expected = -0.233692524908094 ;
    ts.relative(kurt, expected, rel, "kurtosis");
  }

  {
    double c = a.covariance(b);
    r_t expected = 14.5263157894737;
    ts.relative(c, expected, rel, "covariance");
  }


  {
    r_t pv = a.pooled_variance(b);
    r_t expected = 18.8421052631579;
    ts.relative(pv, expected, rel, "pvariance");
  }

  {
    r_t t = a.t_test(b);
    r_t expected = -1.45701922702927;
    ts.relative(t, expected, rel, "ttest");
  }

  {
    size_t max = a.max();
    size_t expected = 22;
    ts.test(max, expected, "max");
  }

  {
    size_t min = a.min();
    size_t expected = 8;
    ts.test(min, expected, "min");
  }

  {
    std::pair<d_t,d_t> expected(8,22);
    std::pair<d_t,d_t> minmax = a.minmax();
    ts.test(minmax.first, expected.first, "minmax min");
    ts.test(minmax.second, expected.second, "minmax max");
  }

  {
    size_t max_index = a.max_index();
    size_t expected = 9 ;
    ts.test(max_index, expected, "max_index");
  }

  {
    size_t min_index = a.min_index();
    size_t expected = 12 ;
    ts.test(min_index, expected, "min_index");
  }

  {
    std::pair<size_t,size_t>  expected(12,9);
    std::pair<size_t,size_t> minmax = a.minmax_index();
    ts.test(minmax.first, expected.first, "minmax_index min");
    ts.test(minmax.second, expected.second, "minmax_index max");
  }


  a.sort();
  dataset<T> a2(a.size()-1);
  for (size_t i = 0; i < a2.size(); i++) a2[i] = a[i];

  {
    r_t median = a.median();
    r_t expected = 18;
    ts.relative(median,expected, rel, "median (even)");
  }

  {
    r_t median = a2.median();
    r_t expected = 18;
    ts.relative(median,expected, rel, "median (odd)");
  }


  {
    r_t zeroth = a.quantile(0.0);
    r_t expected = 8;
    ts.relative(zeroth,expected, rel, "quantile (0)");
  }

  {
    r_t top = a.quantile(1.0);
    r_t expected = 22;
    ts.relative(top,expected, rel, "quantile (100)");
  }

  {
    r_t median = a.quantile(0.5);
    r_t expected = 18;
    ts.relative(median,expected, rel, "quantile (50, even)");
  }

  {
    r_t median = a2.quantile(0.5);
    r_t expected = 18;
    ts.relative(median,expected, rel, "quantile (50, odd)");
  }
}
 

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("statistic", argc, argv);
  double frel = 1e-6;
  double drel = 1e-10;

  testit<double>(ts, drel);
  testit<long double>(ts, drel);
  testit<float>(ts, frel);
  testit_int<short>(ts, drel);
  testit_int<unsigned short>(ts, drel);
  testit_int<int>(ts, drel);
  testit_int<unsigned int>(ts, drel);
  testit_int<long>(ts, drel);
  testit_int<unsigned long>(ts, drel);
  testit_int<char>(ts, drel);
  testit_int<unsigned char>(ts, drel);
  
  return ts.summary() ? 0 : 1;
}

//____________________________________________________________________  
//
// EOF
//  
