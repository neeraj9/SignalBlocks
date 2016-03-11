//
// $Id: histogram-test.cc,v 1.10 2006-05-01 14:24:57 cholm Exp $ 
//  
//  histogram/histogram-test.cc
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
#ifndef GSLMM_histogram
#include <gslmm/histogram/histogram.hh>
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
#define ts gslmm::test_suite::instance()

/** @file   histogram-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of histogram classses.  */

typedef gslmm::histogram<double,size_t> histogram_1d;
typedef gslmm::histogram<std::pair<double,double>,std::pair<size_t,size_t> > 
  histogram_2d;

template <typename T>
std::ostream& operator<<(std::ostream& o, const std::pair<T,T>& p) 
{
  return o << "{" << p.first << "," << p.second << "}";
}

//____________________________________________________________________  
double urand (void) { 
  static unsigned long int x = 1;
  x = (1103515245 * x + 12345) & 0x7fffffffUL ;
  return x / 2147483648.0  ;
}

//____________________________________________________________________  
void test_basic_1d(size_t n) 
{
  //__________________________________________________________________
  // Range allocation 
  std::vector<double> axis(n+1);
  for (size_t i = 0; i < n+1; i++) axis[i] = double(i);

  histogram_1d h1(n);
  
  bool status = true;
  for (size_t i = 0; i < n; i++)
    if (h1.bin_min(i) != axis[i] || h1.bin_max(i) != axis[i+1]) 
      status = false;
  ts.check(status, "gslmm::histogram_1d::histogram creates range");

  h1.reset();
  status = h1.range(axis);

  for (size_t i = 0; i < n; i++)
    if (h1.bin_min(i) != axis[i] || h1.bin_max(i) != axis[i+1]) 
      status = false;
  ts.check(status, "gslmm::histogram_1d::range sets range");

  //__________________________________________________________________
  // Default allocation and fill
  histogram_1d h2(n);
  for (size_t i = 0; i < n; i++) 
    h2.fill(double(i), double(i));

  status = true;
  for (size_t i = 0; i < n; i++) 
    if (h2.bin_content(i) !=  double(i))
      status = false;
  ts.check(status, "gslmm::histogram_1d::fill writes into array");

  //__________________________________________________________________
  // Assignment
  for (size_t i = 0; i <= n; i++) axis[i] = 100.0 + i;
  histogram_1d h3(n);
  h3.range(axis);
  
  h2 = h3;

  status = true;
  for (size_t i = 0; i < n; i++)
    if (h2.bin_min(i) != h3.bin_min(i) || h2.bin_max(i) != h3.bin_max(i))
      status = false;
  ts.check(status, "gslmm::histogram_1d::operator= "
	   "copies bin ranges");

  status = true;
  for (size_t i = 0; i < n; i++)
    if (h2.bin_content(i) != h3.bin_content(i))
      status = false;
  ts.check(status, "gslmm::histogram_1d::operator= "
	   "copies bin values");

  //__________________________________________________________________
  // copy construction
  histogram_1d h4(h2);
  
  status = true;
  for (size_t i = 0; i < n; i++)
    if (h2.bin_min(i) != h4.bin_min(i) || h2.bin_max(i) != h4.bin_max(i))
      status = false;
  ts.check(status, "gslmm::histogram_1d::histogram "
	   "copies bin ranges");

  status = true;
  for (size_t i = 0; i < n; i++)
    if (h2.bin_content(i) != h4.bin_content(i))
      status = false;
  ts.check(status, "gslmm::histogram_1d::histogram "
	   "copies bin values");

  //__________________________________________________________________
  // Reset
  h2.reset();
  for (size_t i = 0; i < n; i++)
    if (h2.bin_content(i) != 0)
      status = false;
  ts.check(status, "gslmm::histogram_1d::reset zeros array");


  //__________________________________________________________________
  // fill
  status = true;
  h2.range(0,n);
  for (size_t i = 0; i < n; i++) {
    if (!h2.fill(double(i))) 
      status = false;

    for (size_t j = 0; j <= i; j++)
      if (h2.bin_content(j) != 1)
	status = false;
    for (size_t j = i + 1; j < n; j++)
      if (h2.bin_content(j) != 0)
	status = false;
  }
  ts.check(status, "gslmm::histogram_1d::fill "
	   "increases bin value");

  //__________________________________________________________________
  // bin ranges
  status = true;
  for (size_t i = 0; i < n; i++) {
    double x0 = 0, x1 = 0;
    h2.bin_range(i, x0, x1);
    if (x0 != i || x1 != i + 1)
      status = false;
  }
  ts.check(status, "gslmm::histogram_1d::bin_range "
	   "returns bin range");

  //__________________________________________________________________
  // Stats
  status = true;
  if (h2.max() != n) status = false;
  ts.check(status, "gslmm::histogram_1d::max returns maximum");

  status = true;
  if (h2.min() != 0) status = false;
  ts.check(status, "gslmm::histogram_1d::min returns minimum");

  status = true;
  if (h2.bins() != n) status = false;
  ts.check(status, "gslmm::histogram_1d::bins returns "
	   "number of bins");
  

  h2[2] = 123456.0;
  h2[4] = -654321;

  double max = h2.max_content();
  ts.test(max ,123456.0, "gslmm::histogram_1d::max_content "
	  "finds maximum value");

  double min = h2.min_content();
  ts.test(min ,-654321.0, "gslmm::histogram_1d::min_content "
	  "finds minimum value");

  size_t imax = h2.max_content_bin();
  ts.test(imax ,size_t(2), "gslmm::histogram_1d::max_content_bin "
	  "finds maximum value bin");

  size_t imin = h2.min_content_bin();
  ts.test(imin ,size_t(4), "gslmm::histogram_1d::min_content_bin "
	  "find minimum value bin");

  //__________________________________________________________________
  // Sum
  for (size_t i = 0; i < n; i++) 
    h2[i] = i + 27;

  double sum = h2.sum();
  ts.test(sum ,n*27+((n-1)*n)/2, "gslmm::histogram_1d::sum "
	  "sums all bin values");

  //__________________________________________________________________
  // Operations
  histogram_1d h5(n);
  for (size_t i = 0; i < n; i++) 
    h5[i] = (i + 27) * (i + 1);

  h3 =  h5;
  h3 += h2;
  status = true;
  for (size_t i = 0; i < n; i++)
    if (h3[i] != h5[i] + h2[i])
      status = false;
  ts.check(status, "gslmm::histogram_1d::operator+= "
	   "histogram addition");


  h3 =  h5;
  h3 -= h2;
  status = true;
  for (size_t i = 0; i < n; i++)
    if (h3[i] != h5[i] - h2[i])
      status = false;
  ts.check(status, "gslmm::histogram_1d::operator-= "
	   "histogram subtraction");

  h3 =  h5;
  h3 *= h2;
  status = true;
  for (size_t i = 0; i < n; i++)
    if (h3[i] != h5[i] * h2[i])
      status = false;
  ts.check(status, "gslmm::histogram_1d::operator*= "
	   "histogram multiplication");

  h3 =  h5;
  h3 /= h2;
  status = true;
  for (size_t i = 0; i < n; i++)
    if (h3[i] != h5[i] / h2[i])
      status = false;
  ts.check(status, "gslmm::histogram_1d::operator/= "
	   "histogram division");

  h3 =  h5;
  h3 *= .5;
  status = true;
  for (size_t i = 0; i < n; i++)
    if (h3[i] != h5[i] * .5)
      status = false;
  ts.check(status, "gslmm::histogram_1d::*= histogram scaling");

  h3 =  h5;
  h3 += .25;
  status = true;
  for (size_t i = 0; i < n; i++)
    if (h3[i] != h5[i] + .25)
      status = false;
  ts.check(status, "gslmm::histogram_1d::+= histogram shift");


  //__________________________________________________________________
  // Finding bins
  histogram_1d h6(n, 0, 1);
  h6.fill( 0, 1);
  h6.fill(.1, 2);
  h6.fill(.2, 3);
  h6.fill(.3, 4);

  const size_t i1 = h6.bin_number( 0);
  const size_t i2 = h6.bin_number(.1);
  const size_t i3 = h6.bin_number(.2);
  const size_t i4 = h6.bin_number(.3);

  double expected;
  for (size_t i = 0; i < n; i++) {
    if (i == i1)      expected = 1;
    else if (i == i2) expected = 2;
    else if (i == i3) expected = 3;
    else if (i == i4) expected = 4;
    else              expected = 0;
    if (h6[i] != expected)
      status = false;
  }
  ts.check(status, "gslmm::histogram_1d::bin_number "
	   "returns index");


  //__________________________________________________________________
  // I/O of histogram 
#if 0
  FILE *f = fopen ("test.txt", "w");
  gsl_histogram_fprintf (f, h, "%.19e", "%.19e");
  fclose (f);

  FILE *f = fopen ("test.txt", "r");
  gsl_histogram *hh = gsl_histogram_calloc (n);
  status = true;

  gsl_histogram_fscanf (f, hh);

  for (size_t i = 0; i < n; i++) {
    if (h.bin_min(i) != hh.bin_min(i))
      status = false;
    if (h[i] != hh[i])
      status = false;
  }
  if (h.bin_max(n-1) != hh.bin-max(n-1))
    status = false;

  ts.check(status, "gslmm::histogram_1d::fprintf and fscanf");

  fclose (f);

  FILE *f = fopen ("test.dat", "wb");
  gsl_histogram_fwrite (f, h);
  fclose (f);

  FILE *f = fopen ("test.dat", "rb");
  gsl_histogram *hh = gsl_histogram_calloc (n);
  status = true;
    
  gsl_histogram_fread (f, hh);

  for (size_t i = 0; i < n; i++) {
    if (h.bin_min(i) != hh.bin_min(i))
      status = false;
    if (h[i] != hh[i])
      status = false;
  }
  if (h.bin_max(n-1) != hh.bin-max(n-1))
    status = false;
  ts.check(status, "gslmm::histogram_1d::fwrite and fread");

  gsl_histogram_free (hh);
  fclose (f);
#endif
}

//____________________________________________________________________  
void test_resample_1d() 
{
  bool status = true;

  histogram_1d h(10, 0, 1);
  h.fill(.1);
  h.fill(.2);
  h.fill(.2);
  h.fill(.3);
  
  histogram_1d hh(100, 0, 1);
  histogram_1d::pdf p(h);
  for (size_t i = 0; i < 100000; i++) {
    double u = urand();
    double x = p.sample(u);
    if (!hh.fill(x)) 
      std::cerr << u << " out side of range [" << hh.min() << ","
		<< hh.max() << std::endl;
  }

  for (size_t i = 0; i < 100; i++) {
    double y = hh[i] / 2500;
    double x, xmax;
    
    hh.bin_range(i, x, xmax);
    size_t k  = h.bin_number(x);    
    double ya = h[k];
    if (ya == 0 && y != 0) {
      std::cerr << i << ": " << y << " vs " << ya << std::endl;
      status = false;
    }
    else if (y != 0) {
      double err   = 1 / sqrt(hh[i]);
      double sigma = fabs ((y - ya) / (ya * err));
      if (sigma > 3) {
	std::cerr  << y << " vs " << ya << " error=" << err 
		   << " sigma=" << sigma << std::endl;
	status = false;
      }
    }
  }
  ts.check(status, "gslmm::histogram_1d::pdf::sample "
	   "within statistical errors");
}

//____________________________________________________________________  
void test_basic_2d() 
{
  std::pair<size_t,size_t> n1(107,239);
  std::pair<size_t,size_t> n2(17,23);
  std::pair<size_t,size_t> n3(10,5);
  
  std::vector<double> xaxis(n3.first+1); 
  std::vector<double> yaxis(n3.second+1); 
  for (size_t i = 0; i < xaxis.size(); i++) xaxis[i] = double(i);
  for (size_t i = 0; i < yaxis.size(); i++) yaxis[i] = double(i) + 90;
  
  //__________________________________________________________________
  // Range allocation 
  histogram_2d h1(xaxis,yaxis);

  bool status = true;
  std::pair<size_t,size_t> i(0,0);
  for (i.first = 0; i.first < n3.first; i.first++) 
    if (h1.bin_min(i).first != xaxis[i.first] || 
	h1.bin_max(i).first != xaxis[i.first+1]) 
      status = false;
  i.first = 0;
  for (i.second = 0; i.second < n3.second; i.second++) 
    if (h1.bin_min(i).second != yaxis[i.second] || 
	h1.bin_max(i).second != yaxis[i.second+1]) 
      status = false;
  ts.check(status, "gslmm::histogram_2d::histogram creates range");

  histogram_2d h2(n3);
  h2.range(xaxis,yaxis);
  status = true;
  i.second = 0;
  for (i.first = 0; i.first < n3.first; i.first++) 
    if (h1.bin_min(i).first != xaxis[i.first] || 
	h1.bin_max(i).first != xaxis[i.first+1]) 
      status = false;
  i.first = 0;
  for (i.second = 0; i.second < n3.second; i.second++) 
    if (h1.bin_min(i).second != yaxis[i.second] || 
	h1.bin_max(i).second != yaxis[i.second+1]) 
      status = false;
  ts.check(status, "gslmm::histogram_2d::range sets range");

  //__________________________________________________________________
  // Default allocation and fill
  histogram_2d h3(n1);
  size_t k = 0;
  for (i.first = 0; i.first < n1.first; i.first++) {
    for (i.second = 0; i.second < n1.second; i.second++) {
      k++;
      h3.fill(i, k);
    }
  }
  
  status = true;
  k = 0;
  for (i.first = 0; i.first < n1.first; i.first++) {
    for (i.second = 0; i.second < n1.second; i.second++) {
      k++;
      if (h3.bin_content(i) !=  double(k)) {
	std::cout << h3[i] << " vs " << k << std::endl;
	status = false;
      }
    }
  }
  ts.check(status, "gslmm::histogram_2d::fill writes into array");

  //__________________________________________________________________
  // Assignment
  std::vector<double> xaxis2(n1.first+1);
  std::vector<double> yaxis2(n1.second+1);
  for (size_t i = 0; i <= n1.first;  i++) xaxis2[i] = 100 + i;
  for (size_t i = 0; i <= n1.second; i++) yaxis2[i] = 900 + i * i;
  histogram_2d h4(xaxis2,yaxis2);
  
  h4 = h3;

  status = true;
  i.second = 0;
  for (i.first = 0; i.first < n3.first; i.first++) 
    if (h4.bin_min(i).first != h3.bin_min(i).first || 
	h4.bin_max(i).first != h3.bin_max(i).first) 
      status = false;
  i.first = 0;
  for (i.second = 0; i.second < n3.second; i.second++) 
    if (h4.bin_min(i).second != h3.bin_min(i).second || 
	h4.bin_max(i).second != h3.bin_max(i).second) 
      status = false;
  ts.check(status, "gslmm::histogram_2d::operator= "
	   "copies bin ranges");

  status = true;
  for (i.first = 0; i.second < n1.first; i.first++) 
    for (i.second = 0; i.second < n1.second; i.second++) 
      if (h4.bin_content(i) !=  h3.bin_content(i))
	status = false;
  ts.check(status, "gslmm::histogram_2d::operator= writes into array");

  //__________________________________________________________________
  // copy construction
  histogram_2d h5(h3);
  
  status = true;
  i.second = 0;
  for (i.first = 0; i.first < n3.first; i.first++) 
    if (h5.bin_min(i).first != h3.bin_min(i).first || 
	h5.bin_max(i).first != h3.bin_max(i).first) 
      status = false;
  i.first = 0;
  for (i.second = 0; i.second < n3.second; i.second++) 
    if (h5.bin_min(i).second != h3.bin_min(i).second || 
	h5.bin_max(i).second != h3.bin_max(i).second) 
      status = false;
  ts.check(status, "gslmm::histogram_2d::histogram "
	   "copies bin ranges");

  status = true;
  for (i.first = 0; i.second < n1.first; i.first++) 
    for (i.second = 0; i.second < n1.second; i.second++) 
      if (h5.bin_content(i) !=  h3.bin_content(i))
	status = false;
  ts.check(status, "gslmm::histogram_2d::histogram "
	   "copies bin values");

  //__________________________________________________________________
  // Reset
  h3.reset();
  for (i.first = 0; i.second < n1.first; i.first++) 
    for (i.second = 0; i.second < n1.second; i.second++) 
      if (h3.bin_content(i) != 0)
	status = false;
  ts.check(status, "gslmm::histogram_2d::reset zeros array");


  //__________________________________________________________________
  // fill
  status = true;
  histogram_2d hh(n3);
  hh.range(std::pair<size_t,size_t>(0,0),n3);
  for (i.first = 0; i.first < n3.first; i.first++) {
    for (i.second = 0; i.second < n3.second; i.second++) {
      if (!hh.fill(i)) {
	std::cerr << i << " out side of range " << hh.bins() << std::endl;
	status = false;
      }

      std::pair<size_t,size_t> j;
      for (j.first = 0; j.first <= i.first; j.first++)
	for (j.second = 0; j.second <= i.second; j.second++) 
	  if (hh.bin_content(j) != 1)
	    status = false;
      for (j.first = i.first+1; j.first < n3.first; j.first++)
	for (j.second = i.second+1; j.second < n3.second; j.second++) 
	  if (hh.bin_content(j) != 0)
	    status = false;
    }
  }
  ts.check(status, "gslmm::histogram_2d::fill "
	   "increases bin value");

  //__________________________________________________________________
  // bin ranges
  status = true;
  for (i.first = 0; i.first < n1.first; i.first++) {
    for (i.second = 0; i.second < n1.second; i.second++) {
      std::pair<double,double> x0;
      std::pair<double,double> x1;
      h3.bin_range(i,x0,x1);
      if (x0.first != i.first || x0.second != i.second ||
	  x1.first != i.first + 1 || x1.second != i.second + 1)
	status = false;
    }
  }
  ts.check(status, "gslmm::histogram_2d::bin_range "
	   "returns bin range");

  //__________________________________________________________________
  // Stats
  status = true;
  if (h3.max() != std::pair<double,double>(n1)) status = false;
  ts.check(status, "gslmm::histogram_2d::max returns maximum");

  status = true;
  if (h3.min() != std::pair<double,double>(0,0)) status = false;
  ts.check(status, "gslmm::histogram_2d::min returns minimum");

  status = true;
  if (h3.bins() != n1) status = false;
  ts.check(status, "gslmm::histogram_2d::bins returns "
	   "number of bins");
  
  std::pair<size_t,size_t> imax(3,2);
  std::pair<size_t,size_t> imin(4,3);
  h3[imax] = 123456.0;
  h3[imin] = -654321;

  double max = h3.max_content();
  ts.test(max ,123456.0, "gslmm::histogram_2d::max_content "
	  "finds maximum value");

  double min = h3.min_content();
  ts.test(min ,-654321.0, "gslmm::histogram_2d::min_content "
	  "finds minimum value");

  i = h3.max_content_bin();
  ts.test(i ,imax, "gslmm::histogram_2d::max_content_bin "
	  "finds maximum value bin");

  i = h3.min_content_bin();
  ts.test(i , imin, "gslmm::histogram_2d::min_content_bin "
	  "find minimum value bin");

  //__________________________________________________________________
  // Sum
  k = 0;
  for (i.first = 0; i.first < n1.first; i.first++) {
    for (i.second = 0; i.second < n1.second; i.second++) {
      h3[i] = k + 27;
      k++;
    }
  }

  double sum = h3.sum();
  size_t nm  = n1.first * n1.second;
  ts.test(sum ,nm*27+((nm-1)*nm)/2, "gslmm::histogram_2d::sum "
	  "sums all bin values");

  //__________________________________________________________________
  // Operations
  histogram_2d h6(n1);
  k = 0;
  for (i.first = 0; i.first < n1.first; i.first++) {
    for (i.second = 0; i.second < n1.second; i.second++) {
      h6[i] = (k + 27) * (k + 1);
      k++;
    }
  }

  h4 =  h6;
  h4 += h3;
  status = true;
  for (i.first = 0; i.first < n1.first; i.first++) 
    for (i.second = 0; i.second < n1.second; i.second++) 
      if (h4[i] != h6[i] + h3[i])
	status = false;
  ts.check(status, "gslmm::histogram_2d::operator+= "
	   "histogram addition");


  h4 =  h6;
  h4 -= h3;
  status = true;
  for (i.first = 0; i.first < n1.first; i.first++) 
    for (i.second = 0; i.second < n1.second; i.second++) 
      if (h4[i] != h6[i] - h3[i])
	status = false;
  ts.check(status, "gslmm::histogram_2d::operator-= "
	   "histogram subtraction");

  h4 =  h6;
  h4 *= h3;
  status = true;
  for (i.first = 0; i.first < n1.first; i.first++) 
    for (i.second = 0; i.second < n1.second; i.second++) 
      if (h4[i] != h6[i] * h3[i])
	status = false;
  ts.check(status, "gslmm::histogram_2d::operator*= "
	   "histogram multiplication");

  h4 =  h6;
  h4 /= h3;
  status = true;
  for (i.first = 0; i.first < n1.first; i.first++) 
    for (i.second = 0; i.second < n1.second; i.second++) 
      if (h4[i] != h6[i] / h3[i])
	status = false;
  ts.check(status, "gslmm::histogram_2d::operator/= "
	   "histogram division");

  h4 =  h6;
  h4 *= .5;
  status = true;
  for (i.first = 0; i.first < n1.first; i.first++) 
    for (i.second = 0; i.second < n1.second; i.second++) 
      if (h4[i] != h6[i] * .5)
	status = false;
  ts.check(status, "gslmm::histogram_2d::*= histogram scaling");

  h4 =  h6;
  h4 += .25;
  status = true;
  for (i.first = 0; i.first < n1.first; i.first++) 
    for (i.second = 0; i.second < n1.second; i.second++) 
      if (h4[i] != h6[i] + .25)
	status = false;
  ts.check(status, "gslmm::histogram_2d::+= histogram shift");


  //__________________________________________________________________
  // Finding bins
  histogram_2d h7(n2, 
		  std::pair<double,double>(0, 0),
		  std::pair<double,double>(5, 5));
  std::pair<double,double> x1( 0,3.01);
  std::pair<double,double> x2(.1,2.01);
  std::pair<double,double> x3(.2,1.01);
  std::pair<double,double> x4(.3,0.01);
  h7.fill(x1, 1);
  h7.fill(x2, 2);
  h7.fill(x3, 3);
  h7.fill(x4, 4);
  std::pair<size_t,size_t> i1 = h7.bin_number(x1);
  std::pair<size_t,size_t> i2 = h7.bin_number(x2);
  std::pair<size_t,size_t> i3 = h7.bin_number(x3);
  std::pair<size_t,size_t> i4 = h7.bin_number(x4);
  double expected;
  for (i.first = 0; i.first < n2.first; i.first++) {
    for (i.second = 0; i.second < n2.second; i.second++) {
      if      (i == i1) expected = 1;
      else if (i == i2) expected = 2;
      else if (i == i3) expected = 3;
      else if (i == i4) expected = 4;
      else              expected = 0;
      if (h7[i] != expected)
	status = false;
    }
  }
  ts.check(status, "gslmm::histogram_2d::bin_number "
	   "returns index");

  //__________________________________________________________________
  // Mean
  std::pair<double,double> pos(.6,.85);
  histogram_2d h8(n1, 
		  std::pair<double,double>(0,0),
		  std::pair<double,double>(1,1));
  h8.fill(pos);
  std::pair<size_t,size_t> ipos = h8.bin_number(pos);
  std::pair<double,double> mean = h8.mean();
  
  
  std::pair<double,double> expected_mean((h8.bin_min(ipos).first + 
					  h8.bin_max(ipos).first) / 2,
					 (h8.bin_min(ipos).second +
					  h8.bin_max(ipos).second) / 2);
  ts.absolute(mean.first, expected_mean.first, 
	      100 * gslmm::type_trait<double>::epsilon(), 
	      "gslmm::histogram_2d::mean of X");
  ts.absolute(mean.second, expected_mean.second, 
	      100 * gslmm::type_trait<double>::epsilon(), 
	      "gslmm::histogram_2d::mean of Y");
  
  //__________________________________________________________________
  // Mean & sigma - test it with bivariate normal distribution
  mean.first  = .7;
  mean.second = .7;
  std::pair<double,double> sigma(.1,.1);
  const double correl = 0.5;
  const double norm = (10 / M_PI / sigma.first / sigma.second 
		       / sqrt (1.0 - correl * correl)); 
  histogram_2d h9(n1, 
		  std::pair<double,double>(0,0),
		  std::pair<double,double>(1,1));
  i.first = i.second = 0;
  for (i.first = 0; i.first < h9.bins().first; i.first++) {
    double xi = ((h9.bin_min(i).first + h9.bin_max(i).first) 
		 / 2 - mean.first) / sigma.first;
    for (i.second = 0; i.second < h9.bins().second; i.second++) {
      double yi = ((h9.bin_min(i).second + h9.bin_max(i).second) 
		   / 2 - mean.second) / sigma.second;
      double prob = norm * exp (-(xi * xi + yi * yi - 2 * correl * xi * yi) 
				/ 2 / (1 - correl * correl));
      h9[i] = prob;
    }
    i.second = 0;
  }

  std::pair<double,double> hsigma = h9.sigma();
  std::pair<double,double> hmean = h9.mean();
  /* evaluate results and compare with parameters */
  ts.absolute(hmean.first, mean.first, 2./h9.bins().first,
	      "gslmm::histogram_2d::mean histogram mean(x)");
  ts.absolute(hmean.second, mean.second, 2./h9.bins().second,
	      "gslmm::histogram_2d::mean histogram mean(x)");
  ts.absolute(hsigma.first, sigma.first, 2./h9.bins().first,
	      "gslmm::histogram_2d::sigma histogram stdev(x)");
  ts.absolute(hsigma.second, sigma.second, 2./h9.bins().second,
	      "gslmm::histogram_2d::sigma histogram stdev(x)");
  ts.absolute(h9.covariance() / hsigma.first / hsigma.second, 
	      correl, 2./(n1.first > n1.second ? n1.first : n1.second),
	      "gslmm::histogram2d::covariance histogram covariance");

  //__________________________________________________________________
  // I/O of histogram 
#if 0
  FILE *f = fopen ("test.txt", "w");
  gsl_histogram_fprintf (f, h, "%.19e", "%.19e");
  fclose (f);

  FILE *f = fopen ("test.txt", "r");
  gsl_histogram *hh = gsl_histogram_calloc (n);
  status = true;

  gsl_histogram_fscanf (f, hh);

  for (size_t i = 0; i < n; i++) {
    if (h.bin_min(i) != hh.bin_min(i))
      status = false;
    if (h[i] != hh[i])
      status = false;
  }
  if (h.bin_max(n-1) != hh.bin-max(n-1))
    status = false;

  ts.check(status, "gslmm::histogram_2d::fprintf and fscanf");

  fclose (f);

  FILE *f = fopen ("test.dat", "wb");
  gsl_histogram_fwrite (f, h);
  fclose (f);

  FILE *f = fopen ("test.dat", "rb");
  gsl_histogram *hh = gsl_histogram_calloc (n);
  status = true;
    
  gsl_histogram_fread (f, hh);

  for (size_t i = 0; i < n; i++) {
    if (h.bin_min(i) != hh.bin_min(i))
      status = false;
    if (h[i] != hh[i])
      status = false;
  }
  if (h.bin_max(n-1) != hh.bin-max(n-1))
    status = false;
  ts.check(status, "gslmm::histogram_2d::fwrite and fread");

  gsl_histogram_free (hh);
  fclose (f);
#endif
}

//____________________________________________________________________  
void test_resample_2d() 
{
  bool status = true;
  const size_t n = 200000;
  histogram_2d h(std::pair<size_t,size_t>(10,10), 
		 std::pair<double,double>( 0, 0),
		 std::pair<double,double>( 1, 1));

  std::pair<size_t,size_t> i;
  std::pair<double,double> j;
  double total = 0;
  for (i.first = 0; i.first < h.bins().first; i.first++) {
    for (i.second = 0; i.second < h.bins().second; i.second++) {
      double w = 10 * i.first + i.second;
      total += w;
      j.first  = .1 * i.first;
      j.second = .1 * i.second;
      h.fill(j, w);
    }
  }
  
  
  histogram_2d hh(std::pair<size_t,size_t>(20,20), 
		  std::pair<double,double>( 0, 0),
		  std::pair<double,double>( 1, 1));
  histogram_2d::pdf p(h);
  
  for (size_t ii = 0; ii < n; ii++) {
    std::pair<double,double> u(urand(),urand());
    std::pair<double,double> x = p.sample(u);	
    if (!hh.fill(x)) 
      std::cerr << u << " out side of range [" << hh.min() << ","
		<< hh.max() << std::endl;
  }

  for (i.first = 0; i.first < hh.bins().first; i.first++) {
    for (i.second = 0; i.second < hh.bins().second; i.second++) {
      double y = 4 * total * hh[i] / n;
      std::pair<double,double> x, xmax;
      hh.bin_range(i, x, xmax);
      std::pair<size_t,size_t> k = h.bin_number(x);    
      double ya = h[k];
      if (ya == 0 && y != 0) {
	std::cerr << i << ": " << y << " vs " << ya << std::endl;
	status = false;
      }
      else if (y != 0) {
	double err   = 1 / sqrt(hh[i]);
	double sigma = fabs ((y - ya) / (ya * err));
	if (sigma > 3.6) {
	  std::cerr  << i << ": " << y << " vs " << ya << " error=" << err 
		     << " sigma=" << sigma << std::endl;
	  status = false;
	}
      }
    }
  }
  ts.check(status, "gslmm::histogram_2d::pdf::sample "
	   "within statistical errors");
}

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts1("histogram", argc, argv);


  test_basic_1d(10);
  test_resample_1d();
  test_basic_2d();
  test_resample_2d();
  return ts.summary() ? 0 : 1;
}

//____________________________________________________________________  
//
// EOF
//  
