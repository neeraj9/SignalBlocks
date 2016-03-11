//// $Id: distribution-test.cc,v 1.9 2008-02-13 15:50:59 cholm Exp $ 
//  
//  random/distribution-test.cc
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
#ifndef GSLMM_math_type_trait
#include <gslmm/math/type_trait.hh>
#endif
#ifndef GSLMM_random_bernoulli
# include <gslmm/random/bernoulli.hh>
#endif
#ifndef GSLMM_random_beta
# include <gslmm/random/beta.hh>
#endif
#ifndef GSLMM_random_binomial
# include <gslmm/random/binomial.hh>
#endif
#ifndef GSLMM_random_bivariate_gaussian
# include <gslmm/random/bivariate_gaussian.hh>
#endif
#ifndef GSLMM_random_cauchy
# include <gslmm/random/cauchy.hh>
#endif
#ifndef GSLMM_random_chisq
# include <gslmm/random/chisq.hh>
#endif
#ifndef GSLMM_random_dir
# include <gslmm/random/dir.hh>
#endif
#ifndef GSLMM_random_dirichlet
# include <gslmm/random/dirichlet.hh>
#endif
#ifndef GSLMM_random_discrete
# include <gslmm/random/discrete.hh>
#endif
#ifndef GSLMM_random_erlang
# include <gslmm/random/erlang.hh>
#endif
#ifndef GSLMM_random_exponential
# include <gslmm/random/exponential.hh>
#endif
#ifndef GSLMM_random_exponential_power
# include <gslmm/random/exponential_power.hh>
#endif
#ifndef GSLMM_random_fdist
# include <gslmm/random/fdist.hh>
#endif
#ifndef GSLMM_random_flat
# include <gslmm/random/flat.hh>
#endif
#ifndef GSLMM_random_gamma
# include <gslmm/random/gamma.hh>
#endif
#ifndef GSLMM_random_gaussian
# include <gslmm/random/gaussian.hh>
#endif
#ifndef GSLMM_random_gaussian_tail
# include <gslmm/random/gaussian_tail.hh>
#endif
#ifndef GSLMM_random_geometric
# include <gslmm/random/geometric.hh>
#endif
#ifndef GSLMM_random_gumbel
# include <gslmm/random/gumbel.hh>
#endif
#ifndef GSLMM_random_hypergeometric
# include <gslmm/random/hypergeometric.hh>
#endif
#ifndef GSLMM_random_landau
# include <gslmm/random/landau.hh>
#endif
#ifndef GSLMM_random_laplace
# include <gslmm/random/laplace.hh>
#endif
#ifndef GSLMM_random_levy
# include <gslmm/random/levy.hh>
#endif
#ifndef GSLMM_random_levy_skew
# include <gslmm/random/levy_skew.hh>
#endif
#ifndef GSLMM_random_logarithmic
# include <gslmm/random/logarithmic.hh>
#endif
#ifndef GSLMM_random_logistic
# include <gslmm/random/logistic.hh>
#endif
#ifndef GSLMM_random_lognormal
# include <gslmm/random/lognormal.hh>
#endif
#ifndef GSLMM_random_multinomial
# include <gslmm/random/multinomial.hh>
#endif
#ifndef GSLMM_random_negative_binomial
# include <gslmm/random/negative_binomial.hh>
#endif
#ifndef GSLMM_random_pareto
# include <gslmm/random/pareto.hh>
#endif
#ifndef GSLMM_random_pascal
# include <gslmm/random/pascal.hh>
#endif
#ifndef GSLMM_random_poisson
# include <gslmm/random/poisson.hh>
#endif
#ifndef GSLMM_random_rayleigh
# include <gslmm/random/rayleigh.hh>
#endif
#ifndef GSLMM_random_rayleigh_tail
# include <gslmm/random/rayleigh_tail.hh>
#endif
#ifndef GSLMM_random_tdist
# include <gslmm/random/tdist.hh>
#endif
#ifndef GSLMM_random_weibull
# include <gslmm/random/weibull.hh>
#endif
#ifndef GSLMM_test_suite
#include <gslmm/test/test_suite.hh>
#endif

/** @file   generator-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of random number generator classses.  */



#define N 100000
#define MULTI_DIM 10


//____________________________________________________________________
void
test_shuffle(gslmm::generator& rg, gslmm::test_suite& ts)
{
  double count[10][10];
  int    status = 0;
  std::vector<int> x(10);
  for (size_t i = 0; i < x.size(); ++i) x[i] = i;
  
  for (size_t i = 0; i < 10; i++)
    for (size_t j = 0; j < 10; j++)
      count[i][j] = 0;
  
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < 10; j++) x[j] = j;
    
    rg.shuffle(x);
    
    for (size_t j = 0; j < 10; j++) count[x[j]][j]++;
  }
  
  for (size_t i = 0; i < 10; i++) {
    for (size_t j = 0; j < 10; j++) {
      double expected = N / 10.0;
      double d        = fabs (count[i][j] - expected);
      double sigma    = d / sqrt (expected);
      if (sigma > 5 && d > 1) {
	status = 1;
	ts.status(status, "shuffle %d,%d (%g observed vs %g expected)", 
		  i, j, count[i][j] / N, 0.1);
      }
    }
  }
  ts.status(status, "shuffle, on {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}");
}

//____________________________________________________________________
void
test_choose(gslmm::generator& rg, gslmm::test_suite& ts)
{
  double count[10];
  std::vector<int> x(10);
  std::vector<int> y(3);
  for (size_t i = 0; i < x.size(); ++i) x[i] = i;

  int    status = 0;

  for (size_t i = 0; i < 10; i++) count[i] = 0;

  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < 10; j++) x[j] = j;

    rg.choose(y, x);
    
    for (size_t j = 0; j < 3; j++) count[y[j]]++;
  }

  for (size_t i = 0; i < 10; i++) {
    double expected = 3.0 * N / 10.0;
    double d        = fabs (count[i] - expected);
    double sigma    = d / sqrt (expected);
    if (sigma > 5 && d > 1) {
      status = 1;
      ts.status(status, "choose, %d (%g observed vs %g expected)",
		i, count[i] / N, 0.1);
    }
  }
  ts.status(status, "choose, (3) on {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}");
}

//____________________________________________________________________
template <typename T1>
void
testMoments(gslmm::test_suite& ts,
	    gslmm::distribution<T1>& d, 
	    const char *name,
	    double a, double b, double p)
{
  double count = 0;
  for (size_t i = 0; i < N; i++) {
    double r = d();
    if (r < b && r > a) count++;
  }

  double expected = p * N;
  double sigma    = fabs(count - expected) / sqrt(expected);
  int    status   = (sigma > 3);

  ts.status(status, "%s [%g,%g]", name, a, b);
}

#define BINS 100

//____________________________________________________________________
void
testPDF(gslmm::test_suite& ts,
	gslmm::distribution<double>& d, 
	const char *name)
{
  double count[BINS], p[BINS];
  double a      = -5.0;
  double b      = +5.0;
  double dx     = (b - a) / BINS;

  for (size_t i = 0; i < BINS; i++) count[i] = 0;
  for (size_t i = 0; i < N; i++) {
    double r = d();
    if (r < b && r > a) {
      size_t j = (int) ((r - a) / dx);
      count[j]++;
    }
  }

  for (size_t i = 0; i < BINS; i++) {
    /* Compute an approximation to the integral of p(x) from x to x+dx
       using Simpson's rule */
    double x = a + i * dx;
#define STEPS 100
    double sum = 0;
    if (fabs(x) < 1e-10) x = 0.0;    /* hit the origin exactly */
        
    for (size_t j = 1; j < STEPS; j++) sum += d.pdf(x + j * dx / STEPS);

    p[i] = 0.5 * (d.pdf(x) + 2 * sum + d.pdf(x + dx - 1e-7)) * dx / STEPS;
  }

  int status = 0;
  for (size_t i = 0; i < BINS; i++) {
    double x        = a + i * dx;
    double d        = fabs(count[i] - N * p[i]);
    int    status_i = 0;
    
    if (p[i] != 0) {
      double s = d / sqrt (N * p[i]);
      status_i = (s > 5) && (d > 1);
    }
    else
      status_i = (count[i] != 0);

    status |= status_i;
    if (status_i)
      ts.status(status_i, "%s [%g,%g) (%g/%d=%g observed vs %g expected)",
		name, x, x + dx, count[i], N, count[i] / N, p[i]);
  }

  ts.status(status, "%s, sampling against pdf over range [%g,%g) ",
	    name, a, b);
}

//____________________________________________________________________
void
testPDF(gslmm::test_suite& ts,
	gslmm::distribution<size_t>& d, 
	const char *name)
{
  std::vector<double> count(BINS), p(BINS);

  for (size_t i = 0; i < BINS; i++) count[i] = 0;
  for (size_t i = 0; i < N; i++) {
    int r = d();
    if (r >= 0 && r < BINS) count[r]++;
  }

  for (size_t i = 0; i < BINS; i++) p[i] = d.pdf(i);

  int status = 0;
  for (size_t i = 0; i < BINS; i++){
    std::string n(name);
    double d        = fabs(count[i] - N * p[i]);
    double s        = 0;
    int    status_i = 0;
    if (p[i] != 0) {
      s        = d / sqrt (N * p[i]);
      status_i = (s > 5) && (d > 1);
    }
    else
      status_i = (count[i] != 0);
#if 0
    if (n == "poisson" || n == "poisson_large") 
      std::cout << std::setw(3)  << i        << "  Counts: " 
		<< std::setw(6)  << count[i] << "  PDF: " 
		<< std::setw(11) << p[i]     << "  Diff: " 
		<< std::setw(8)  << d        << "  Var:   "
		<< std::setw(8)  << s        << std::endl;
#endif
    status |= status_i;
    if (status_i)
      ts.status(status_i, "%s i=%d (%g observed vs %g expected)",
		name, i, count[i] / N, p[i]);
  }

  ts.status(status, "%s, sampling against pdf over range [%d,%d) ",
	    name, 0, BINS);
}

//____________________________________________________________________
void
testPDF(gslmm::test_suite& ts,
	gslmm::distribution<unsigned int>& d, 
	const char *name)
{
  std::vector<double> count(BINS), p(BINS);

  for (size_t i = 0; i < BINS; i++) count[i] = 0;
  for (size_t i = 0; i < N; i++) {
    int r = d();
    if (r >= 0 && r < BINS) count[r]++;
  }

  for (size_t i = 0; i < BINS; i++) p[i] = d.pdf(i);

  int status = 0;
  for (size_t i = 0; i < BINS; i++){
    std::string n(name);
    double d        = fabs(count[i] - N * p[i]);
    double s        = 0;
    int    status_i = 0;
    if (p[i] != 0) {
      s        = d / sqrt (N * p[i]);
      status_i = (s > 5) && (d > 1);
    }
    else
      status_i = (count[i] != 0);
#if 0
    if (n == "poisson" || n == "poisson_large") 
      std::cout << std::setw(3)  << i        << "  Counts: " 
		<< std::setw(6)  << count[i] << "  PDF: " 
		<< std::setw(11) << p[i]     << "  Diff: " 
		<< std::setw(8)  << d        << "  Var:   "
		<< std::setw(8)  << s        << std::endl;
#endif
    status |= status_i;
    if (status_i)
      ts.status(status_i, "%s i=%d (%g observed vs %g expected)",
		name, i, count[i] / N, p[i]);
  }

  ts.status(status, "%s, sampling against pdf over range [%d,%d) ",
	    name, 0, BINS);
}


//____________________________________________________________________
void
test_beta(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::beta d(2.0, 3.0,rg);;
  // testMoments(ts, d, "beta"); 
  testPDF(ts, d, "beta");
}
//____________________________________________________________________
void
test_bernoulli(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::bernoulli d(0.3,rg);;
  // testMoments(ts, d, "bernoulli"); 
  testPDF(ts, d, "bernoulli");
}

//____________________________________________________________________
void
test_binomial(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::binomial d(0.3, 5,rg);;
  // testMoments(ts, d, "binomial"); 
  testPDF(ts, d, "binomial");
}

//____________________________________________________________________
void
test_binomial_tpe(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::binomial d(0.3, 5,rg);;
  // testMoments(ts, d, "binomial"); 
  testPDF(ts, d, "binomial");
}


//____________________________________________________________________
void
test_binomial_large(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::binomial d(0.3, 55,rg);;
  // testMoments(ts, d, "binomial"); 
  testPDF(ts, d, "binomial");
}

//____________________________________________________________________
void
test_binomial_large_tpe(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::binomial d(0.3, 55,rg);;
  // testMoments(ts, d, "binomial"); 
  testPDF(ts, d, "binomial");
}


//____________________________________________________________________
void
test_binomial_huge(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::binomial d(0.3, 5500,rg);;
  // testMoments(ts, d, "binomial"); 
  testPDF(ts, d, "binomial");
}


//____________________________________________________________________
void
test_binomial_huge_tpe(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::binomial d(0.3, 5500,rg);;
  // testMoments(ts, d, "binomial"); 
  testPDF(ts, d, "binomial");
}

//____________________________________________________________________
void
test_cauchy(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::cauchy d(2.0,rg);;
  testMoments(ts, d, "cauchy", 0.0, 10000.0, 0.5);
  testPDF(ts, d, "cauchy");
}
//____________________________________________________________________
void
test_chisq(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::chisq d(13.0,rg);;
  // testMoments(ts, d, "chisq"); 
  testPDF(ts, d, "chisq");
}
//____________________________________________________________________
struct dir2d : public gslmm::distribution<double>
{
public:
  gslmm::dir _dir;
  typedef gslmm::distribution<double> base;
  dir2d(gslmm::generator& rg,bool trig=false) 
    : base(rg), _dir(2, rg, trig) 
  {}
  double sample() 
  {
    gslmm::vector<double> v(2);
    _dir.sample(v);
    double theta = atan2(v[0],v[1]);
    return theta;
  }
  double pdf(const double& x)
  {
    if (x > - M_PI && x <= M_PI) return 1. / (2 * M_PI);
    return 0;
  }
};
  
void
test_dir2d(gslmm::generator& rg, gslmm::test_suite& ts)
{
  dir2d d(rg);
  // testMoments(ts, d, "dir"); 
  testPDF(ts, d, "dir (2D)");
}

//____________________________________________________________________
void
test_dir2d_trig_method(gslmm::generator& rg, gslmm::test_suite& ts)
{
  dir2d d(rg, true);
  // testMoments(ts, d, "dir"); 
  testPDF(ts, d, "dir (2D - trigonometric)");
}

//____________________________________________________________________
struct dir3d : public gslmm::distribution<double>
{
public:
  gslmm::dir _dir;
  size_t _i1, _i2;
  typedef gslmm::distribution<double> base;
  dir3d(gslmm::generator& rg, size_t i1, size_t i2) 
    : base(rg), _dir(3, rg), _i1(i1), _i2(i2)
  {}
  double sample() 
  {
    gslmm::vector<double> v(3);
    _dir.sample(v);
    double theta = atan2(v[_i1],v[_i2]);
    return theta;
  }
  double pdf(const double& x)
  {
    if (x > -M_PI && x <= M_PI) return 1 / (2 * M_PI);
    return 0;
  }
};

void
test_dir3dxy(gslmm::generator& rg, gslmm::test_suite& ts)
{
  dir3d d(rg, 0, 1);
  // testMoments(ts, d, "dir"); 
  testPDF(ts, d, "dir (3D - xy)");
}


//____________________________________________________________________
void
test_dir3dyz(gslmm::generator& rg, gslmm::test_suite& ts)
{
  dir3d d(rg, 1, 2);
  // testMoments(ts, d, "dir"); 
  testPDF(ts, d, "dir (3D - yz)");
}

//____________________________________________________________________
void
test_dir3dzx(gslmm::generator& rg, gslmm::test_suite& ts)
{
  dir3d d(rg, 2, 0);
  // testMoments(ts, d, "dir"); 
  testPDF(ts, d, "dir (3D - zx)");
}

//____________________________________________________________________
struct dirichlet : public gslmm::distribution<double>
{
  gslmm::dirichlet* _d;
  typedef gslmm::distribution<double> base;
  
  dirichlet(gslmm::generator& r) 
    : base(r)
  {
    std::vector<double> alpha(2);
    alpha[0] = 2.5;
    alpha[1] = 5.0;
    _d = new gslmm::dirichlet(alpha, r);
  }
  virtual ~dirichlet() 
  {
    delete _d;
  }
  double sample() 
  {
    std::vector<double> theta(2);
    _d->sample(theta);
    return theta[0];
  }
  double pdf(const double& x) 
  {
    if (x <= 0 || x >= 1) return 0;
    std::vector<double> theta(2);
    theta[0] = x;
    theta[1] = 1 - x;
    return _d->pdf(theta);
  }
};



void
test_dirichlet(gslmm::generator& rg, gslmm::test_suite& ts)
{
  dirichlet d(rg);
  // testMoments(ts, d, "dirichlet"); 
  testPDF(ts, d, "dirichlet");
}


/* Check that the observed means of the Dirichlet variables are
   within reasonable statistical errors of their correct values. */

#define DIRICHLET_K 10

//____________________________________________________________________
void
test_dirichlet_moments(gslmm::generator& rg, gslmm::test_suite& ts)
{

  
  gslmm::exponential e(.1, rg);
  double alpha_sum = 0.0;
  std::vector<double> alpha(DIRICHLET_K);
  std::vector<double> theta(DIRICHLET_K);
  std::vector<double> theta_sum(DIRICHLET_K);
  for (size_t k = 0; k < DIRICHLET_K; k++) {
    alpha[k]     =  e();
    alpha_sum    += alpha[k];
    theta_sum[k] =  0.0;
  }

  gslmm::dirichlet d(alpha,rg);
  for (size_t n = 0; n < N; n++) {
    d(theta);
    for (size_t k = 0; k < DIRICHLET_K; k++) theta_sum[k] += theta[k];
  }

  double mean      = 0;
  double obs_mean  = 0; 
  double sd        = 0;
  double sigma     = 0;
  int status;
  for (size_t k = 0; k < DIRICHLET_K; k++) {
    mean     = alpha[k] / alpha_sum;
    sd       =  sqrt (alpha[k] * (1. - alpha[k] / alpha_sum) 
		      / alpha_sum / (alpha_sum + 1.));
    obs_mean = theta_sum[k] / N;
    sigma    = sqrt (double(N)) * fabs (mean - obs_mean) / sd;
    status   = (sigma > 3.0);
    ts.status(status,
	      "dirichlet: mean (%g observed vs %g expected)",
	      obs_mean, mean);
  }
}



//____________________________________________________________________
/* Check that the observed means of the multinomial variables are
   within reasonable statistical errors of their correct values. */
void
test_multinomial_moments(gslmm::generator& rg, gslmm::test_suite& ts)
{
  const unsigned int sum_n = 100;

  std::vector<double> p(MULTI_DIM);
  p[0] = 0.2;
  p[1] = 0.20;
  p[2] = 0.17;
  p[3] = 0.14;
  p[4] = 0.12;
  p[5] = 0.07;
  p[6] = 0.05;
  p[7] = 0.02;
  p[8] = 0.02;
  p[9] = 0.01; 
  std::vector<unsigned int> x(MULTI_DIM);
  std::vector<double> x_sum(MULTI_DIM);

  gslmm::multinomial d(p, sum_n, rg);
  

  for (size_t k = 0; k < MULTI_DIM; k++) x_sum[k] =0.0;

  for (size_t n = 0; n < N; n++) {
    d(x);
    for (size_t k = 0; k < MULTI_DIM; k++) x_sum[k] += x[k];
  }
  
  double mean     = 0;
  double obs_mean = 0;
  double sigma    = 0;
  double sd       = 0;
  int    status   = 0;
  
  for (size_t k = 0; k < MULTI_DIM; k++) {
    mean     = p[k] * sum_n;
    sd       = p[k] * (1. - p[k]) * sum_n;
    obs_mean = x_sum[k] / N;
    sigma    = sqrt (double(N)) * fabs (mean - obs_mean) / sd;
    status   = (sigma > 3.0);

    ts.status(status, "multinomial:  mean (%g observed vs %g expected)",
	      obs_mean, mean);
  }
}


//____________________________________________________________________
void
test_discrete1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  static double P[3] = { 0.59, 0.4, 0.01 };
  gslmm::discrete d(3, P,rg);;
  testMoments(ts, d, "discrete", -0.5, 0.5, 0.59);
  testMoments(ts, d, "discrete",  0.5, 1.5, 0.40);
  testMoments(ts, d, "discrete",  1.5, 3.5, 0.01);
  testPDF(ts, d, "discrete");
}

//____________________________________________________________________
void
test_discrete2(gslmm::generator& rg, gslmm::test_suite& ts)
{
  static double P[10] = { 1, 9, 3, 4, 5, 8, 6, 7, 2, 0 };
  gslmm::discrete d(10, P,rg);;
  testMoments(ts, d, "discrete", -0.5,  0.5, 1.0/45.0);
  testMoments(ts, d, "discrete",  8.5,  9.5, 0);     
  testPDF(ts, d, "discrete");
}
//____________________________________________________________________
void
test_discrete3(gslmm::generator& rg, gslmm::test_suite& ts)
{
  static double P[20];
  for (size_t i=0; i<20; ++i) P[i]=1.0/20;
  gslmm::discrete d(20, P,rg);;
  testMoments(ts, d, "discrete", -0.5, 0.5, 0.05);
  testMoments(ts, d, "discrete",  0.5, 1.5, 0.05);
  testMoments(ts, d, "discrete", -0.5, 9.5, 0.5);
  testPDF(ts, d, "discrete");
}

//____________________________________________________________________
void
test_erlang(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::erlang d(3.0, 4.0,rg);;
  // testMoments(ts, d, "erlang"); 
  testPDF(ts, d, "erlang");
}
//____________________________________________________________________
void
test_exponential(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::exponential d(2.0,rg);;
  testMoments(ts, d, "exponential", 0.0, 1.0, 1 - exp (-0.5));
  testPDF(ts, d, "exponential");
}
//____________________________________________________________________
void
test_exponential_power0(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::exponential_power d(3.7, 0.3,rg);;
  // testMoments(ts, d, "exponential_power"); 
  testPDF(ts, d, "exponential_power");
}

//____________________________________________________________________
void
test_exponential_power1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::exponential_power d(3.7, 1.0,rg);;
  // testMoments(ts, d, "exponential_power"); 
  testPDF(ts, d, "exponential_power");
}

//____________________________________________________________________
void
test_exponential_power1a(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::exponential_power d(3.7, 1.9,rg);;
  // testMoments(ts, d, "exponential_power"); 
  testPDF(ts, d, "exponential_power");
}

//____________________________________________________________________
void
test_exponential_power2(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::exponential_power d(3.7, 2.0,rg);;
  // testMoments(ts, d, "exponential_power"); 
  testPDF(ts, d, "exponential_power");
}

//____________________________________________________________________
void
test_exponential_power2a(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::exponential_power d(3.7, 7.5,rg);;
  // testMoments(ts, d, "exponential_power"); 
  testPDF(ts, d, "exponential_power");
}
//____________________________________________________________________
void
test_fdist(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::fdist d(3.0, 4.0,rg);;
  // testMoments(ts, d, "fdist"); 
  testPDF(ts, d, "fdist");
}
//____________________________________________________________________
void
test_flat(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::flat d(3.0, 4.0,rg);;
  // testMoments(ts, d, "flat"); 
  testPDF(ts, d, "flat");
}
//____________________________________________________________________
void
test_gamma(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gamma d(2.5, 2.17,rg);;
  // testMoments(ts, d, "gamma"); 
  testPDF(ts, d, "gamma");
}
//____________________________________________________________________
void
test_gamma1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gamma d(1.0, 2.17,rg);;
  // testMoments(ts, d, "gamma"); 
  testPDF(ts, d, "gamma");
}

//____________________________________________________________________
void
test_gamma_int(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gamma d(10.0, 2.17,rg);;
  // testMoments(ts, d, "gamma"); 
  testPDF(ts, d, "gamma");
}

//____________________________________________________________________
void
test_gamma_large(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gamma d(20.0, 2.17,rg);;
  // testMoments(ts, d, "gamma"); 
  testPDF(ts, d, "gamma");
}

//____________________________________________________________________
void
test_gaussian(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gaussian d(3.0,rg);;
  // testMoments(ts, d, "gaussian"); 
  testPDF(ts, d, "gaussian");
}
//____________________________________________________________________
double
test_gaussian_ratio_method(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gaussian d(3.0, rg, true);;
  // testMoments(ts, d, "gaussian");
  testPDF(ts, d, "gaussian");
  return d();
}

//____________________________________________________________________
void
test_gaussian_tail(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gaussian_tail d(1.7, 0.25,rg);;
  // testMoments(ts, d, "gaussian_tail"); 
  testPDF(ts, d, "gaussian_tail");
}
//____________________________________________________________________
void
test_gaussian_tail1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gaussian_tail d(-1.7, 5.0,rg);;
  // testMoments(ts, d, "gaussian_tail"); 
  testPDF(ts, d, "gaussian_tail");
}
//____________________________________________________________________
void
test_gaussian_tail2(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gaussian_tail d(0.1, 2.0,rg);;
  // testMoments(ts, d, "gaussian_tail"); 
  testPDF(ts, d, "gaussian_tail");
}

//____________________________________________________________________
void
test_ugaussian(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gaussian d(1,rg);;
  testMoments(ts, d, "gaussian", 0.0, 100.0, 0.5);
  testMoments(ts, d, "gaussian", -1.0, 1.0, 0.6826895);
  testMoments(ts, d, "gaussian", 3.0, 3.5, 0.0011172689);
  testPDF(ts, d, "gaussian");
}


//____________________________________________________________________
void
test_ugaussian_ratio_method(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gaussian d(1, rg, true);;
  // testMoments(ts, d, "gaussian"); 
  testPDF(ts, d, "gaussian");
}

//____________________________________________________________________
void
test_ugaussian_tail(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gaussian_tail d(3.0,1,rg);;
  testMoments(ts, d, "ugaussian_tail", 3.0, 3.5, 0.0011172689 / 0.0013498981);
  testPDF(ts, d, "ugaussian_tail");
}

//____________________________________________________________________
struct bivariate_gaussian1 : public gslmm::distribution<double>
{
  typedef gslmm::distribution<double> base;
  gslmm::bivariate_gaussian _g1;
  gslmm::gaussian _g2;
  bivariate_gaussian1(gslmm::generator& rg) 
    : base(rg),  _g1(3, 2, .3, rg), _g2(3, rg)
  {}
  double sample() 
  {
    std::pair<double,double> x;
    _g1.sample(x);
    return x.first;
  }
  double pdf(const double& x) 
  {
    return _g2.pdf(x);
  }
};


void
test_bivariate_gaussian1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  bivariate_gaussian1 d(rg);
  // testMoments(ts, d, "bivariate_guassian");
  testPDF(ts, d, "bivariate_guassian (1)");
}

//____________________________________________________________________
struct bivariate_gaussian2 : public gslmm::distribution<double>
{
  typedef gslmm::distribution<double> base;
  gslmm::bivariate_gaussian _g1;

  bivariate_gaussian2(gslmm::generator& rg) 
    : base(rg), _g1(3, 2, .3, rg)
  {}
  double sample() 
  {
    std::pair<double,double> x;
    _g1.sample(x);
    return x.second;
  }
  double pdf(const double& y) 
  {
    size_t n   = 10;
    double sum = 0;
    double a   = -10;
    double b   = 10;
    double dx  = (b - a) / n;
    for (size_t i = 0; i < n; i++) {
      double x = a + i * dx;
      std::pair<double,double> xx(x, y);
      sum += _g1.pdf (xx) * dx;
    }
    return sum;
  }
};


void
test_bivariate_gaussian2(gslmm::generator& rg, gslmm::test_suite& ts)
{
  bivariate_gaussian2 d(rg);
  // testMoments(ts, d, "bivariate_guassian");
  testPDF(ts, d, "bivariate_guassian (2)");
}


//____________________________________________________________________
struct bivariate_gaussian3 : public gslmm::distribution<double>
{
  typedef gslmm::distribution<double> base;
  gslmm::bivariate_gaussian _g1;
  gslmm::gaussian* _g2;
  bivariate_gaussian3(double rho, gslmm::generator& rg) 
    : base(rg),  _g1(3, 2, rho, rg), _g2(0)
  {
    double su    = (3 + rho * 2);
    double sv    = 2 * sqrt (1 - rho * rho);
    _g2 = new gslmm::gaussian(sqrt (su * su + sv * sv), rg);
  }
  virtual ~bivariate_gaussian3() 
  {
    delete _g2;
  }
  double sample() 
  {
    std::pair<double,double> x;
    _g1.sample(x);
    return x.first+x.second;
  }
  double pdf(const double& x) 
  {
    return _g2->pdf(x);
  }
};

void
test_bivariate_gaussian3(gslmm::generator& rg, gslmm::test_suite& ts)
{
  bivariate_gaussian3 d(.3, rg);
  // testMoments(ts, d, "bivariate_guassian");
  testPDF(ts, d, "bivariate_guassian (3)");
}

//____________________________________________________________________
void
test_bivariate_gaussian4(gslmm::generator& rg, gslmm::test_suite& ts)
{
  bivariate_gaussian3 d(-.5, rg);
  // testMoments(ts, d, "bivariate_guassian");
  testPDF(ts, d, "bivariate_guassian (4)");
}

//____________________________________________________________________
void
test_geometric(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::geometric d(0.5,rg);;
  // testMoments(ts, d, "geometric"); 
  testPDF(ts, d, "geometric");
}
//____________________________________________________________________
void
test_geometric1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::geometric d(1.0,rg);;
  // testMoments(ts, d, "geometric"); 
  testPDF(ts, d, "geometric");
}
//____________________________________________________________________
void
test_hypergeometric1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::hypergeometric d(5, 7, 4,rg);;
  // testMoments(ts, d, "hypergeometric"); 
  testPDF(ts, d, "hypergeometric");
}

//____________________________________________________________________
void
test_hypergeometric2(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::hypergeometric d(5, 7, 11,rg);;
  // testMoments(ts, d, "hypergeometric"); 
  testPDF(ts, d, "hypergeometric");
}
//____________________________________________________________________
void
test_hypergeometric3(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::hypergeometric d(5, 7, 1,rg);;
  // testMoments(ts, d, "hypergeometric"); 
  testPDF(ts, d, "hypergeometric");
}
//____________________________________________________________________
void
test_hypergeometric4(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::hypergeometric d(5, 7, 20,rg);;
  // testMoments(ts, d, "hypergeometric"); 
  testPDF(ts, d, "hypergeometric");
}
//____________________________________________________________________
void
test_hypergeometric5(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::hypergeometric d(2, 7, 5,rg);;
  // testMoments(ts, d, "hypergeometric"); 
  testPDF(ts, d, "hypergeometric");
}

//____________________________________________________________________
void
test_hypergeometric6(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::hypergeometric d(2, 10, 3,rg);;
  // testMoments(ts, d, "hypergeometric"); 
  testPDF(ts, d, "hypergeometric");
}



//____________________________________________________________________
void
test_gumbel1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gumbel d(1, 3.12, 4.56, rg);
  // testMoments(ts, d, "gumbel 1"); 
  testPDF(ts, d, "gumbel 1");
}
//____________________________________________________________________
void
test_gumbel2(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::gumbel d(2, 3.12, 4.56, rg);
  // testMoments(ts, d, "gumbel 2"); 
  testPDF(ts, d, "gumbel 2");
}
//____________________________________________________________________
void
test_landau(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::landau d(rg);
  // testMoments(ts, d, "landau"); 
  testPDF(ts, d, "landau");
}

//____________________________________________________________________
void
test_levy1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::levy d(5.0, 1.0,rg);;
  // testMoments(ts, d, "levy"); 
  testPDF(ts, d, "levy");
}

//____________________________________________________________________
void
test_levy2(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::levy d(5.0, 2.0,rg);;
  // testMoments(ts, d, "levy"); 
  testPDF(ts, d, "levy");
}

//____________________________________________________________________
void
test_levy1a(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::levy d(5.0, 1.01,rg);;
  // testMoments(ts, d, "levy"); 
  testPDF(ts, d, "levy");
}

//____________________________________________________________________
void
test_levy2a(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::levy d(5.0, 1.99,rg);;
  // testMoments(ts, d, "levy"); 
  testPDF(ts, d, "levy");
}

//____________________________________________________________________
void
test_levy_skew1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::levy_skew d(5.0, 1.0, 0.0,rg);;
  // testMoments(ts, d, "levy_skew"); 
  testPDF(ts, d, "levy_skew");
}

//____________________________________________________________________
void
test_levy_skew2(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::levy_skew d(5.0, 2.0, 0.0,rg);;
  // testMoments(ts, d, "levy_skew"); 
  testPDF(ts, d, "levy_skew");
}

//____________________________________________________________________
void
test_levy_skew1a(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::levy_skew d(5.0, 1.01, 0.0,rg);;
  // testMoments(ts, d, "levy_skew"); 
  testPDF(ts, d, "levy_skew");
}

//____________________________________________________________________
void
test_levy_skew2a(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::levy_skew d(5.0, 1.99, 0.0,rg);;
  // testMoments(ts, d, "levy_skew"); 
  testPDF(ts, d, "levy_skew");
}

//____________________________________________________________________
void
test_levy_skew1b(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::levy_skew d(5.0, 1.01, 0.0011,rg);;
  // testMoments(ts, d, "levy_skew"); 
  testPDF(ts, d, "levy_skew");
}

//____________________________________________________________________
void
test_levy_skew2b(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::levy_skew d(5.0, 1.99, 0.001,rg);;
  // testMoments(ts, d, "levy_skew"); 
  testPDF(ts, d, "levy_skew");
}

//____________________________________________________________________
void
test_logistic(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::logistic d(3.1,rg);;
  // testMoments(ts, d, "logistic"); 
  testPDF(ts, d, "logistic");
}
//____________________________________________________________________
void
test_logarithmic(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::logarithmic d(0.4,rg);;
  // testMoments(ts, d, "logarithmic"); 
  testPDF(ts, d, "logarithmic");
}

//____________________________________________________________________
void
test_lognormal(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::lognormal d(2.7, 1.3,rg);;
  // testMoments(ts, d, "lognormal"); 
  testPDF(ts, d, "lognormal");
}
//____________________________________________________________________
struct multinomial : public gslmm::distribution<unsigned int>
{
  typedef gslmm::distribution<unsigned int> base;
  gslmm::multinomial* _d;
  gslmm::multinomial* _d_pdf;
  multinomial(gslmm::generator& r) 
    : base(r) 
  {
    std::vector<double> p(3);
    p[0] = 2.; p[1] = 7. ; p[2] = 1.;
    _d = new gslmm::multinomial(p,BINS,r);

    std::vector<double> p_pdf(2);
    p_pdf[0] = 0.4; p_pdf[1] = 1.6;
    _d_pdf = new gslmm::multinomial(p_pdf, BINS, r);
  }
  ~multinomial() 
  {
    delete _d;
    delete _d_pdf;
  }
  unsigned int sample() 
  {
    std::vector<unsigned int> n(3);
    _d->sample(n);
    return n[0];
  }
  double pdf(const unsigned int& x) 
  {
    std::vector<unsigned int> n(2);
    n[0] = x;
    n[1] = BINS - x;
    return _d_pdf->pdf(n);
  }
};

void
test_multinomial(gslmm::generator& rg, gslmm::test_suite& ts)
{
  multinomial d(rg);
  // testMoments(ts,d,"multinomial");
  testPDF(ts,d,"multinomial");
}


//____________________________________________________________________
struct multinomial_large : public gslmm::distribution<unsigned int>
{
  typedef gslmm::distribution<unsigned int> base;
  gslmm::multinomial* _d;
  gslmm::multinomial* _d_pdf;
  multinomial_large(gslmm::generator& r) 
    : base(r) 
  {
    std::vector<double> p(MULTI_DIM);
    p[0] = .2;
    p[1] = .2;
    p[2] = .17;
    p[3] = .14;
    p[4] = .12;
    p[5] = .07;
    p[6] = .05;
    p[7] = .04;
    p[8] = .01;
    p[9] = 0;
    _d = new gslmm::multinomial(p,BINS,r);

    std::vector<double> p_pdf(2);
    p_pdf[0] = 0.4; p_pdf[1] = 1.6;
    _d_pdf = new gslmm::multinomial(p_pdf, BINS, r);
  }
  virtual ~multinomial_large() 
  {
    delete _d;
    delete _d_pdf;
  }
  unsigned int sample() 
  {
    std::vector<unsigned int> n(MULTI_DIM);
    _d->sample(n);
    return n[0];
  }
  double pdf(const unsigned int& x) 
  {
    std::vector<unsigned int> n(2);
    n[0] = x;
    n[1] = BINS - x;
    return _d_pdf->pdf(n);
  }
};

void
test_multinomial_large(gslmm::generator& rg, gslmm::test_suite& ts)
{
  multinomial_large d(rg);
  // testMoments(ts,d,"multinomial");
  testPDF(ts,d,"multinomial");
}


//____________________________________________________________________
void
test_negative_binomial(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::negative_binomial d(0.3, 20.0,rg);;
  // testMoments(ts, d, "negative_binomial"); 
  testPDF(ts, d, "negative_binomial");
}
//____________________________________________________________________
void
test_pascal(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::pascal d(0.8, 3,rg);;
  // testMoments(ts, d, "pascal"); 
  testPDF(ts, d, "pascal");
}

//____________________________________________________________________
void
test_pareto(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::pareto d(1.9, 2.75,rg);;
  // testMoments(ts, d, "pareto"); 
  testPDF(ts, d, "pareto");
}
//____________________________________________________________________
void
test_rayleigh(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::rayleigh d(1.9,rg);;
  // testMoments(ts, d, "rayleigh"); 
  testPDF(ts, d, "rayleigh");
}
//____________________________________________________________________
void
test_rayleigh_tail(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::rayleigh_tail d(2.7, 1.9,rg);;
  // testMoments(ts, d, "rayleigh_tail"); 
  testPDF(ts, d, "rayleigh_tail");
}

//____________________________________________________________________
void
test_poisson(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::poisson d(5.0,rg);;
  // testMoments(ts, d, "poisson"); 
  testPDF(ts, d, "poisson");
}
//____________________________________________________________________
void
test_poisson_large(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::poisson d(30.0,rg);;
  // testMoments(ts, d, "poisson"); 
  testPDF(ts, d, "poisson_large");
}

//____________________________________________________________________
void
test_tdist1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::tdist d(1.75,rg);;
  // testMoments(ts, d, "tdist"); 
  testPDF(ts, d, "tdist");
}
//____________________________________________________________________
void
test_tdist2(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::tdist d(12.75,rg);;
  // testMoments(ts, d, "tdist"); 
  testPDF(ts, d, "tdist");
}

//____________________________________________________________________
void
test_laplace(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::laplace d(2.75,rg);;
  // testMoments(ts, d, "laplace"); 
  testPDF(ts, d, "laplace");
}
//____________________________________________________________________
void
test_weibull(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::weibull d(3.14, 2.75,rg);;
  // testMoments(ts, d, "weibull"); 
  testPDF(ts, d, "weibull");
}

//____________________________________________________________________
void
test_weibull1(gslmm::generator& rg, gslmm::test_suite& ts)
{
  gslmm::weibull d(2.97, 1.0,rg);;
  // testMoments(ts, d, "weibull"); 
  testPDF(ts, d, "weibull");
}  
//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("distribution", argc, argv);

  // Generator
  gslmm::generator r;
  
  // Special
  test_shuffle(r, ts);
  test_choose(r, ts);
  
  // Both tests of momements and PDF
  test_ugaussian(r, ts);
  test_ugaussian_tail(r, ts);
  test_exponential(r, ts);
  test_cauchy(r, ts);
  test_discrete1(r, ts);
  test_discrete2(r, ts);
  test_discrete3(r, ts);

  // Special
  test_dirichlet_moments(r, ts);
  test_multinomial_moments(r, ts);

  // Only test PDF
  test_beta(r, ts);
  test_chisq(r, ts);
  test_dirichlet(r, ts);
  test_erlang(r, ts);

  test_exponential_power0(r, ts);
  test_exponential_power1(r, ts);
  test_exponential_power1a(r, ts);
  test_exponential_power2(r, ts);
  test_exponential_power2a(r, ts);

  test_fdist(r, ts);
  test_flat(r, ts);
  test_gamma(r, ts);
  test_gamma1(r, ts);
  test_gamma_int(r, ts);
  test_gamma_large(r, ts);
  test_gaussian(r, ts);
  test_gaussian_ratio_method(r, ts);
  test_ugaussian_ratio_method(r, ts);
  test_gaussian_tail1(r, ts);
  test_gaussian_tail2(r, ts);
  // test_ugaussian_tail(r, ts);

  test_bivariate_gaussian1(r, ts);
  test_bivariate_gaussian2(r, ts);
  test_bivariate_gaussian3(r, ts);
  test_bivariate_gaussian4(r, ts);

  test_gumbel1(r, ts);
  test_gumbel2(r, ts);
  test_landau(r, ts);
  test_levy1(r, ts);
  test_levy2(r, ts);
  // test_levy1a(r, ts);
  // test_levy2a(r, ts);
  test_levy_skew1(r, ts);
  test_levy_skew2(r, ts);
  // test_levy_skew1a(r, ts);
  // test_levy_skew2a(r, ts);
  // test_levy_skew1b(r, ts);
  // test_levy_skew2b(r, ts);
  test_logistic(r, ts);
  test_lognormal(r, ts);
  test_pareto(r, ts);
  test_rayleigh(r, ts);
  test_rayleigh_tail(r, ts);
  test_tdist1(r, ts);
  test_tdist2(r, ts);
  test_laplace(r, ts);
  test_weibull(r, ts);
  test_weibull1(r, ts);

  test_dir2d(r, ts);
  test_dir2d_trig_method(r, ts);
  test_dir3dxy(r, ts);
  test_dir3dyz(r, ts);
  test_dir3dzx(r, ts);

  gslmm::generator r2;

  test_poisson(r, ts);
  test_poisson_large(r2, ts);
  test_bernoulli(r, ts);
  test_binomial(r, ts);
  test_binomial_tpe(r, ts);
  test_binomial_large(r, ts);
  test_binomial_large_tpe(r, ts);
  test_binomial_huge(r, ts);
  test_binomial_huge_tpe(r, ts);
  test_geometric(r, ts);
  test_geometric1(r, ts);
  test_hypergeometric1(r, ts);
  test_hypergeometric2(r, ts);
  test_hypergeometric3(r, ts);
  test_hypergeometric4(r, ts);
  test_hypergeometric5(r, ts);
  test_hypergeometric6(r, ts);
  test_logarithmic(r, ts);
  test_multinomial(r, ts);
  test_multinomial_large(r, ts);
  test_negative_binomial(r, ts);
  test_pascal(r, ts);

  return ts.summary() ? 0 : 1;
}

//____________________________________________________________________  
//
// EOF
//  
