//
// $Id: linear-test.cc,v 1.8 2006-05-01 14:24:57 cholm Exp $ 
//  
//  linear/linear-test.cc
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
#ifndef GSLMM_math_constant
#include <gslmm/math/constant.hh>
#endif
#ifndef GSLMM_linear
#include <gslmm/linear/linear.hh>
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

/** @file   linear-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of linear classses.  */
#define TEST_SVD_4X4 1

//____________________________________________________________________
bool
check(double x, double actual, double eps)
{
  if (x == actual)
    return true;
  else if (actual == 0)
    return fabs(x) <= eps;
  else
    return (fabs(x - actual)/fabs(actual)) <= eps;
  return true;
}

//____________________________________________________________________
template <typename T>
gslmm::matrix<T>*
create_hilbert_matrix(size_t size)
{
  gslmm::matrix<T>* m = new gslmm::matrix<T>(size, size);
  for (size_t i = 0; i < size; i++) 
    for (size_t j = 0; j < size; j++)
      (*m)(i,j) = 1. / (i + j + 1.);
  return m;
}

//____________________________________________________________________
template <typename T>
gslmm::matrix<T>*
create_general_matrix(size_t size1, size_t size2)
{
  gslmm::matrix<T>* m = new gslmm::matrix<T>(size1, size2);
  for (size_t i = 0; i< size1; i++) 
    for (size_t j = 0; j < size2; j++) 
      (*m)(i, j) =  1. / (i +j + 1.);
    return m;
}

//____________________________________________________________________
template <typename T>
gslmm::matrix<T>*
create_singular_matrix(size_t size1, size_t size2)
{
  gslmm::matrix<T>* m = new gslmm::matrix<T>(size1, size2);
  for(size_t i = 0; i < size1; i++) 
    for(size_t j = 0; j < size2; j++) 
      (*m)(i, j) = 1.0/(i+j+1.0);
  // zero the first column 
  for (size_t j = 0; j < size2; j++) (*m)(0, j) = 0.0;
  return m;
}

//____________________________________________________________________
template <typename T>
gslmm::matrix<T>*
create_vandermonde_matrix(size_t size)
{
  gslmm::matrix<T>* m = new gslmm::matrix<T>(size, size);
  for (size_t i = 0; i<size; i++) 
    for (size_t j = 0; j<size; j++) 
      (*m)(i, j) = pow(i + 1., size - j - 1.);
  return m;
}

//____________________________________________________________________
template <typename T>
gslmm::matrix<T>*
create_moler_matrix(size_t size)
{
  gslmm::matrix<T>* m = new gslmm::matrix<T>(size, size);
  for (size_t i = 0; i < size; i++) 
    for (size_t j = 0; j < size; j++) 
      (*m)(i, j) = std::min(i + 1, j + 1) - 2.;
  return m;
}

//____________________________________________________________________
template <typename T>
gslmm::matrix<gslmm::complex<T> >*
create_complex_matrix(size_t size)
{
  size_t i, j;
  gslmm::matrix<gslmm::complex<T> >* m 
    = new gslmm::matrix<gslmm::complex<T> >(size, size);
  for (size_t i = 0; i < size; i++) 
    for (size_t j = 0; j < size; j++) 
      (*m)(i, j) = gslmm::complex<T>(1/(i + j + 1.), 1/(i * i + j * j + .5));
  return m;
}

//____________________________________________________________________
template <typename T>
gslmm::matrix<T>*
create_row_matrix(size_t size1, size_t size2)
{
  gslmm::matrix<T>* m = new gslmm::matrix<T>(size1, size2);
  for (size_t i = 0; i < size1; i++) 
    (*m)(i, 0) = 1. / (i + 1.);
  return m;
}

//____________________________________________________________________
template <typename T>
gslmm::matrix<T>*
create_2x2_matrix(double a11, double a12, double a21, double a22)
{
  gslmm::matrix<T>* m = new gslmm::matrix<T>(2, 2);
  (*m)(0, 0) =  a11;
  (*m)(0, 1) =  a12;
  (*m)(1, 0) =  a21;
  (*m)(1, 1) =  a22;
  return m;
}

//____________________________________________________________________
template <typename T>
gslmm::matrix<T>*
create_diagonal_matrix(double a[], size_t size)
{
  gslmm::matrix<T>* m = new gslmm::matrix<T>(size, size);
  for (size_t i = 0; i <size; i++) 
    (*m)(i,i) = a[i];
  return m;
}

//____________________________________________________________________
template <typename T>
struct pair
{
  gslmm::matrix<T>* _matrix;
  T*                _actual;
};


//____________________________________________________________________
gslmm::matrix<double>*                  m11      = 0;
gslmm::matrix<double>*                  m51      = 0;
gslmm::matrix<double>*                  m35      = 0;
gslmm::matrix<double>*                  m53      = 0;
gslmm::matrix<double>*                  m97      = 0;
gslmm::matrix<double>*                  s35      = 0;
gslmm::matrix<double>*                  s53      = 0;
gslmm::matrix<double>*                  hilb2    = 0;
gslmm::matrix<double>*                  hilb3    = 0;
gslmm::matrix<double>*                  hilb4    = 0;
gslmm::matrix<double>*                  hilb12   = 0;
gslmm::matrix<double>*                  row3     = 0;
gslmm::matrix<double>*                  row5     = 0;
gslmm::matrix<double>*                  row12    = 0;
gslmm::matrix<double>*                  A22      = 0;
gslmm::matrix<double>*                  A33      = 0;
gslmm::matrix<double>*                  A44      = 0;
gslmm::matrix<double>*                  A55      = 0;
gslmm::matrix<gslmm::complex<double> >* c7       = 0;
gslmm::matrix<double>*                  inf5     = 0; 
gslmm::matrix<double>*                  nan5     = 0;
gslmm::matrix<double>*                  vander2  = 0;
gslmm::matrix<double>*                  vander3  = 0;
gslmm::matrix<double>*                  vander4  = 0;
gslmm::matrix<double>*                  vander12 = 0;
gslmm::matrix<double>*                  moler10  = 0;

//____________________________________________________________________
double inf5_data[]         = { 1.0, 0.0, -3.0, 0.0, -5.0};
double m53_lssolution[]    = {  52.5992295702070, -337.7263113752073, 
			       351.8823436427604};
double hilb2_solution[]    = { -8.0, 18.0} ;
double hilb3_solution[]    = {  27.0, -192.0,   210.0 };
double hilb4_solution[]    = { -64.0,  900.0, -2520.0, 1820.0};
double hilb12_solution[]   = {        -1728.0,       245388.0,     -8528520.0, 
                                  127026900.0,  -1009008000.0,   4768571808.0, 
                               -14202796608.0,  27336497760.0, -33921201600.0,
                                26189163000.0, -11437874448.0,   2157916488.0};
double c7_solution[]       = {  2.40717272023734e+01, -9.84612797621247e+00,
			       -2.69338853034031e+02,  8.75455232472528e+01,
			        2.96661356736296e+03, -1.02624473923993e+03,
			       -1.82073812124749e+04,  5.67384473042410e+03,
			        5.57693879019068e+04, -1.61540963210502e+04,
			       -7.88941207561151e+04,  1.95053812987858e+04,
			        3.95548551241728e+04, -7.76593696255317e+03 };
double vander2_solution[]  = {1.0, 0.0}; 
double vander3_solution[]  = {0.0, 1.0, 0.0}; 
double vander4_solution[]  = {0.0, 0.0, 1.0, 0.0}; 
double vander12_solution[] = {0.0, 0.0, 0.0, 0.0,
			      0.0, 0.0, 0.0, 0.0, 
			      0.0, 0.0, 1.0, 0.0}; 

//____________________________________________________________________
template <typename T>
int
test_LU_solve_dim(gslmm::test_suite& ts, 
		  const gslmm::matrix<T>& m, 
		  const double* actual, double eps)
{
  int s = 0;
  int signum;
  unsigned long i, dim = m.row_size();

  gslmm::lu_decomposition<T> lu(m);
  gslmm::vector<T>           x(dim);
  gslmm::vector<T>           rhs(dim);
  for (size_t i = 0; i < dim; i++) rhs[i] = i + 1.;
  if (!lu.solve(rhs, x)) s += 1;

  for (size_t i = 0; i<dim; i++) 
    if (!check(x[i],actual[i],eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g", dim, i, x[i], actual[i]);
      s += 1;
    }
  gslmm::vector<T> residual(dim);
  if (!lu.refine(rhs, x, residual)) s += 1;
  for (size_t i = 0; i < dim; i++) 
    if (!check(x[i],actual[i],eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g (improved)\n",
		 dim,i,x[i],actual[i]);
      s += 1;
    }
  return s;
}

//____________________________________________________________________
int
test_LU_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_LU_solve_dim(ts, *hilb2, hilb2_solution, 8 * eps);
  s += f;
  ts.status(f,"LU_solve hilbert(2)");
  
  f =  test_LU_solve_dim(ts, *hilb3, hilb3_solution, 64 * eps);
  s += f;
  ts.status(f,"LU_solve hilbert(3)");

  f =  test_LU_solve_dim(ts,*hilb4, hilb4_solution, 2048 * eps);
  s += f;
  ts.status(f, "LU_solve hilbert(4)");

  f =  test_LU_solve_dim(ts,*hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "LU_solve hilbert(12)");

  f =  test_LU_solve_dim(ts,*vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "LU_solve vander(2)");

  f =  test_LU_solve_dim(ts,*vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "LU_solve vander(3)");

  f =  test_LU_solve_dim(ts,*vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "LU_solve vander(4)");

  f =  test_LU_solve_dim(ts,*vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "LU_solve vander(12)");

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_LU_solve_dim(gslmm::test_suite& ts, 
		  const gslmm::matrix<gslmm::complex<T> >& m, 
		  const double* actual, double eps)
{
  int s = 0;
  int signum;
  unsigned long i, dim = m.row_size();

  gslmm::lu_decomposition<gslmm::complex<T> > lu(m);
  gslmm::vector<gslmm::complex<T> >           x(dim);
  gslmm::vector<gslmm::complex<T> >           rhs(dim);
  for (size_t i = 0; i<dim; i++) 
    rhs[i] = gslmm::complex<T>(2. *i + 1., 2. * i + 2.);
  if (!lu.solve(rhs, x)) s += 1;

  for (size_t i = 0; i<dim; i++) {
    if(!check(x[i].real(),actual[2 * i + 0],eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g",dim,i,x[i].real(),actual[2*i]);
      s += 1;
    }
    if (!check(x[i].imag(),actual[2 * i + 1],eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g",
		 dim,i,x[i].imag(),actual[2*i+1]);
      s += 1;
    }
  }
  
  gslmm::vector<gslmm::complex<T> > residual(dim);
  if (!lu.refine(rhs, x, residual)) s += 1;
  for (size_t i = 0; i < dim; i++) {
    if(!check(x[i].real(),actual[2 * i + 0],eps)){
      ts.message("%3lu[%lu]: %22.18g   %22.18g (improved)",
		 dim,i,x[i].real(),actual[2*i]);
      s += 1;
    }
    if (!check(x[i].imag(),actual[2 * i + 1],eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g (improved)",
		 dim,i,x[i].imag(),actual[2*i+1]);
      s += 1;
    }
  }
  return s;
}

//____________________________________________________________________
int 
test_LUc_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_LU_solve_dim(ts, *c7, c7_solution, 1024. * 1024. * eps);
  s += f;
  ts.status(f, "complex_LU_solve complex(7)");

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_QR_solve_dim(gslmm::test_suite& ts, 
		  const gslmm::matrix<T>& m, const double * actual, double eps)
{
  int    s   = 0;
  size_t dim = m.row_size();

  gslmm::vector<T> rhs(dim);
  gslmm::qr_decomposition<T> qr(m);

  for (size_t i = 0; i<dim; i++) rhs[i] =  i+1.0;
  gslmm::vector<T> x(dim);
  qr.solve(rhs, x);
  
  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s += 1;
    }
  }

  return s;
}

//____________________________________________________________________
int
test_QR_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_QR_solve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "QR_solve hilbert(2)");

  f =  test_QR_solve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "QR_solve hilbert(3)");

  f =  test_QR_solve_dim(ts, *hilb4, hilb4_solution, 2 * 1024 * eps);
  s += f;
  ts.status(f, "QR_solve hilbert(4)");

  f =  test_QR_solve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "QR_solve hilbert(12)");

  f =  test_QR_solve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "QR_solve vander(2)");

  f =  test_QR_solve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "QR_solve vander(3)");

  f =  test_QR_solve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "QR_solve vander(4)");

  f =  test_QR_solve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "QR_solve vander(12)");

  return s;
}


//____________________________________________________________________
template <typename T>
int
test_QR_QRsolve_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, 
		    const double * actual, double eps)
{
  int    s   = 0;
  size_t dim = m.row_size();

  gslmm::qr_decomposition<T> qr(m);
  gslmm::matrix<T> q(dim,dim);

  gslmm::vector<T> x(dim);
  gslmm::vector<T> rhs(dim);
  for (size_t i = 0; i<dim; i++) rhs[i] =  i+1.0;
  if (!qr.qr_solve(rhs, x)) s += 1;

  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s += 1;
    }
  }

  return s;
}

//____________________________________________________________________
int
test_QR_QRsolve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_QR_QRsolve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "QR_QRsolve hilbert(2)");

  f =  test_QR_QRsolve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "QR_QRsolve hilbert(3)");

  f =  test_QR_QRsolve_dim(ts, *hilb4, hilb4_solution, 2 * 1024 * eps);
  s += f;
  ts.status(f, "QR_QRsolve hilbert(4)");

  f =  test_QR_QRsolve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "QR_QRsolve hilbert(12)");

  f =  test_QR_QRsolve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "QR_QRsolve vander(2)");

  f =  test_QR_QRsolve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "QR_QRsolve vander(3)");

  f =  test_QR_QRsolve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "QR_QRsolve vander(4)");

  f =  test_QR_QRsolve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "QR_QRsolve vander(12)");

  return s;
}


//____________________________________________________________________
template <typename T>
int
test_QR_lssolve_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, const double * actual, double eps)
{
  int    s = 0;
  size_t M = m.row_size();
  size_t N = m.column_size();

  gslmm::qr_decomposition<T> qr(m);

  gslmm::vector<T> x(N);
  gslmm::vector<T> res(M);
  gslmm::vector<T> rhs(M);
  for (size_t i = 0; i < M; i++) rhs[i] =  i + 1.0;

  if (!qr.solve(rhs,x,res)) s += 1;

  for (size_t i = 0; i<N; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("(%3lu,%3lu)[%lu]: %22.18g   %22.18g\n", M, N, i, x[i], 
		 actual[i]);
      s += 1;
    }
  }

  // compute residual r = b - m x 
  gslmm::vector<T> r(M);
  if (M == N) {
    r.zero();
  } else {
    r = rhs;
    gslmm::matrix_vector_product(-1., m, x, 1., r);
  };

  for (size_t i = 0; i<N; i++) {
    if (!check(res[i], r[i], sqrt(eps))) {
      ts.message("(%3lu,%3lu)[%lu]: %22.18g   %22.18g\n",M,N,i,res[i],r[i]);
      s += 1;
    }
  }


  return s;
}

//____________________________________________________________________
int
test_QR_lssolve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_QR_lssolve_dim(ts, *m53, m53_lssolution, 2 * 64 * eps);
  s += f;
  ts.status(f, "QR_lssolve m(5,3)");

  f =  test_QR_lssolve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "QR_lssolve hilbert(2)");

  f =  test_QR_lssolve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "QR_lssolve hilbert(3)");

  f =  test_QR_lssolve_dim(ts, *hilb4, hilb4_solution, 2 * 1024 * eps);
  s += f;
  ts.status(f, "QR_lssolve hilbert(4)");

  f =  test_QR_lssolve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "QR_lssolve hilbert(12)");

  f =  test_QR_lssolve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "QR_lssolve vander(2)");

  f =  test_QR_lssolve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "QR_lssolve vander(3)");

  f =  test_QR_lssolve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "QR_lssolve vander(4)");

  f =  test_QR_lssolve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "QR_lssolve vander(12)");

  return s;
}


//____________________________________________________________________
template <typename T>
int
test_QR_decomp_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, double eps)
{
  int    s = 0;
  size_t M = m.row_size();
  size_t N = m.column_size();

  gslmm::qr_decomposition<T> qr(m);
  gslmm::matrix<T> q(M,M); q = qr.q();
  gslmm::matrix<T> r(M,N); r = qr.r();
  
  /* compute a = q r */
  gslmm::matrix<T> a(M,N);
  gslmm::matrix_matrix_product(1.,q,r,0.,a);
  

  for (size_t i = 0; i<M; i++) {
    for (size_t j = 0; j<N; j++) {
      double aij = a(i, j);
      double mij = m(i, j);
      if (!check(aij, mij, eps)) {
        ts.message("(%3lu,%3lu)[%lu,%lu]: %22.18g   %22.18g\n", 
		   M, N, i,j, aij, mij);
        s += 1;
      }
    }
  }

  return s;
}

//____________________________________________________________________
int
test_QR_decomp(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_QR_decomp_dim(ts, *m35, 2 * 8 * eps);
  s += f;
  ts.status(f, "QR_decomp m(3,5)");

  f =  test_QR_decomp_dim(ts, *m53, 2 * 64 * eps);
  s += f;
  ts.status(f, "QR_decomp m(5,3)");

  f =  test_QR_decomp_dim(ts, *hilb2, 2 * 8 * eps);
  s += f;
  ts.status(f, "QR_decomp hilbert(2)");

  f =  test_QR_decomp_dim(ts, *hilb3, 2 * 64 * eps);
  s += f;
  ts.status(f, "QR_decomp hilbert(3)");

  f =  test_QR_decomp_dim(ts, *hilb4, 2 * 1024 * eps);
  s += f;
  ts.status(f, "QR_decomp hilbert(4)");

  f =  test_QR_decomp_dim(ts, *hilb12, 2 * 1024 * eps);
  s += f;
  ts.status(f, "QR_decomp hilbert(12)");

  f =  test_QR_decomp_dim(ts, *vander2, 8 * eps);
  s += f;
  ts.status(f, "QR_decomp vander(2)");

  f =  test_QR_decomp_dim(ts, *vander3, 64 * eps);
  s += f;
  ts.status(f, "QR_decomp vander(3)");

  f =  test_QR_decomp_dim(ts, *vander4, 1024 * eps);
  s += f;
  ts.status(f, "QR_decomp vander(4)");

  f =  test_QR_decomp_dim(ts, *vander12, 0.0005); /* FIXME: bad accuracy */
  s += f;
  ts.status(f, "QR_decomp vander(12)");

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_QRPT_solve_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, 
		    const double * actual, double eps)
{
  int s = 0;
  int signum;
  unsigned long i, dim = m.row_size();

  gslmm::qr_decomposition_pivot<T> qr(m);

  gslmm::vector<T> rhs(dim);
  gslmm::vector<T> x(dim);
  for (size_t i = 0; i<dim; i++) rhs[i] =  i+1.0;
  if (!qr.solve(rhs,x)) s++;
  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s++;
    }
  }
  return s;
}

//____________________________________________________________________
int
test_QRPT_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_QRPT_solve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "QRPT_solve hilbert(2)");

  f =  test_QRPT_solve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "QRPT_solve hilbert(3)");

  f =  test_QRPT_solve_dim(ts, *hilb4, hilb4_solution, 2 * 2048 * eps);
  s += f;
  ts.status(f, "QRPT_solve hilbert(4)");

  f =  test_QRPT_solve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "QRPT_solve hilbert(12)");

  f =  test_QRPT_solve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "QRPT_solve vander(2)");

  f =  test_QRPT_solve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "QRPT_solve vander(3)");

  f =  test_QRPT_solve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "QRPT_solve vander(4)");

  f =  test_QRPT_solve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "QRPT_solve vander(12)");

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_QRPT_QRsolve_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, 
		      const double * actual, double eps)
{
  int    s = 0;
  size_t dim = m.row_size();

  gslmm::qr_decomposition_pivot<T> qr(m, true);

  gslmm::vector<T> x(dim);
  gslmm::vector<T> rhs(dim);
  for (size_t i = 0; i<dim; i++) rhs[i] =  i+1.0;

  if (!qr.qr_solve(rhs,x)) s++;
  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g", dim, i, x[i], actual[i]);
      s++;
    }
  }
  return s;
}

//____________________________________________________________________
int
test_QRPT_QRsolve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_QRPT_QRsolve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "QRPT_QRsolve hilbert(2)");

  f =  test_QRPT_QRsolve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "QRPT_QRsolve hilbert(3)");

  f =  test_QRPT_QRsolve_dim(ts, *hilb4, hilb4_solution, 2 * 2048 * eps);
  s += f;
  ts.status(f, "QRPT_QRsolve hilbert(4)");

  f =  test_QRPT_QRsolve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "QRPT_QRsolve hilbert(12)");

  f =  test_QRPT_QRsolve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "QRPT_QRsolve vander(2)");

  f =  test_QRPT_QRsolve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "QRPT_QRsolve vander(3)");

  f =  test_QRPT_QRsolve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "QRPT_QRsolve vander(4)");

  f =  test_QRPT_QRsolve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "QRPT_QRsolve vander(12)");

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_QRPT_decomp_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, double eps)
{
  int s = 0, signum;
  unsigned long i,j, M = m.row_size(), N = m.column_size();

  gslmm::qr_decomposition_pivot<T> qr(m);
  gslmm::matrix<T> q(M,M); q = qr.q();
  gslmm::matrix<T> r(M,N); r = qr.r();
  gslmm::matrix<T> a(M,N);
  gslmm::vector<T> d(GSL_MIN(M,N));
  gslmm::vector<T> norm(N);

  // compute a = q r
  gslmm::matrix_matrix_product(1., q, r, 0., a);

  // Compute QR P^T by permuting the elements of the rows of QR
  const gslmm::permutation& perm = qr.p();
  for (i = 0; i < M; i++) {
    gslmm::vector<T> row(a[i]);
    perm.permute(row, true);
  }

  for (size_t i = 0; i<M; i++) {
    for (size_t j = 0; j<N; j++) {
      double aij = a(i, j);
      double mij = m(i, j);
      if (!check(aij, mij, eps)) {
        ts.message("(%3lu,%3lu)[%lu,%lu]: %22.18g   %22.18g\n", 
		   M, N, i,j, aij, mij);
        s++;
      }
    }
  }
  return s;
}

//____________________________________________________________________
int
test_QRPT_decomp(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_QRPT_decomp_dim(ts, *m35, 2 * 8 * eps);
  s += f;
  ts.status(f, "QRPT_decomp m(3,5)");

  f =  test_QRPT_decomp_dim(ts, *m53, 2 * 8 * eps);
  s += f;
  ts.status(f, "QRPT_decomp m(5,3)");

  f =  test_QRPT_decomp_dim(ts, *s35, 2 * 8 * eps);
  s += f;
  ts.status(f, "QRPT_decomp s(3,5)");

  f =  test_QRPT_decomp_dim(ts, *s53, 2 * 8 * eps);
  s += f;
  ts.status(f, "QRPT_decomp s(5,3)");

  f =  test_QRPT_decomp_dim(ts, *hilb2, 2 * 8 * eps);
  s += f;
  ts.status(f, "QRPT_decomp hilbert(2)");

  f =  test_QRPT_decomp_dim(ts, *hilb3, 2 * 64 * eps);
  s += f;
  ts.status(f, "QRPT_decomp hilbert(3)");

  f =  test_QRPT_decomp_dim(ts, *hilb4, 2 * 1024 * eps);
  s += f;
  ts.status(f, "QRPT_decomp hilbert(4)");

  f =  test_QRPT_decomp_dim(ts, *hilb12, 2 * 1024 * eps);
  s += f;
  ts.status(f, "QRPT_decomp hilbert(12)");

  f =  test_QRPT_decomp_dim(ts, *vander2, 8 * eps);
  s += f;
  ts.status(f, "QRPT_decomp vander(2)");

  f =  test_QRPT_decomp_dim(ts, *vander3, 64 * eps);
  s += f;
  ts.status(f, "QRPT_decomp vander(3)");

  f =  test_QRPT_decomp_dim(ts, *vander4, 1024 * eps);
  s += f;
  ts.status(f, "QRPT_decomp vander(4)");

  f = test_QRPT_decomp_dim(ts, *vander12, 0.0005); /* FIXME: bad accuracy */
  ts.status(f, "QRPT_decomp vander(12)");
  s += f;

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_QR_update_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, double eps)
{
  int s = 0;
  unsigned long i,j,k, M = m.row_size(), N = m.column_size();

  gslmm::matrix<T> qr1(m);
  gslmm::matrix<T> qr2(m);

  gslmm::vector<T> rhs(N);
  gslmm::vector<T> u(M);
  gslmm::vector<T> v(N);
  for (size_t i = 0; i < N; i++) rhs[i] =  i+1.0;
  for (size_t i = 0; i < M; i++) u[i]   =  sin(i+1.0);
  for (size_t i = 0; i < N; i++) v[i]   =  cos(i+2.0) + sin(i*i+3.0);

  for (size_t i = 0; i<M; i++) {
    double ui = u[i];
    for (size_t j = 0; j<N; j++) {
      double vj  = v[j];
      double qij = qr1(i, j);
      qr1(i, j)  =  qij + ui * vj;
    }
  }

  gslmm::qr_decomposition<T> qr(m);

  const gslmm::matrix<T>& q2 = qr.q();
  const gslmm::matrix<T>& r2 = qr.r();

  /* compute w = Q^T u */      
  gslmm::vector<T> w(M);
  for (j = 0; j < M; j++) {
    double sum = 0;
    for (i = 0; i < M; i++) sum += q2(i, j) * u[i];
    w[j] =  sum;
  }

  if (!qr.update(w, v)) s++;

  /* compute qr2 = q2 * r2 */
  for (i = 0; i < M; i++) {
    for (j = 0; j< N; j++) {
      double sum = 0;
      for (k = 0; k <= GSL_MIN(j,M-1); k++) {
	double qik = q2(i, k);
	double rkj = r2(k, j);
	sum += qik * rkj ;
      }
      qr2(i, j) =  sum;
    }
  }

  for (size_t i = 0; i<M; i++) {
    for (size_t j = 0; j<N; j++) {
      double s1 = qr1(i, j);
      double s2 = qr2(i, j);
      if (!check(s1, s2, eps)) {
        ts.message("(%3lu,%3lu)[%lu,%lu]: %22.18g   %22.18g\n", 
		   M, N, i,j, s1, s2);
        s ++;
      }
    }
  }
  return s;
}

//____________________________________________________________________
int
test_QR_update(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_QR_update_dim(ts, *m35, 2 * 512 * eps);
  s += f;
  ts.status(f, "QR_update m(3,5)");

  f =  test_QR_update_dim(ts, *m53, 2 * 512 * eps);
  s += f;
  ts.status(f, "QR_update m(5,3)");

  f =  test_QR_update_dim(ts, *hilb2,  2 * 512 * eps);
  s += f;
  ts.status(f, "QR_update hilbert(2)");

  f =  test_QR_update_dim(ts, *hilb3,  2 * 512 * eps);
  s += f;
  ts.status(f, "QR_update hilbert(3)");

  f =  test_QR_update_dim(ts, *hilb4, 2 * 1024 * eps);
  s += f;
  ts.status(f, "QR_update hilbert(4)");

  f =  test_QR_update_dim(ts, *hilb12, 2 * 1024 * eps);
  s += f;
  ts.status(f, "QR_update hilbert(12)");

  f =  test_QR_update_dim(ts, *vander2, 8 * eps);
  s += f;
  ts.status(f, "QR_update vander(2)");

  f =  test_QR_update_dim(ts, *vander3, 64 * eps);
  s += f;
  ts.status(f, "QR_update vander(3)");

  f =  test_QR_update_dim(ts, *vander4, 1024 * eps);
  s += f;
  ts.status(f, "QR_update vander(4)");

  f = test_QR_update_dim(ts, *vander12, 0.0005); /* FIXME: bad accuracy */
  ts.status(f, "QR_update vander(12)");
  s += f;

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_LQ_solve_dim(gslmm::test_suite& ts,
		  const gslmm::matrix<T>& m, 
		  const double * actual, double eps)
{
  int s = 0;
  unsigned long dim = m.row_size();

  gslmm::matrix<T>           mt(m, true);
  gslmm::lq_decomposition<T> lq(mt);
  
  gslmm::vector<T> rhs(dim);
  gslmm::vector<T> x(dim);
  for (size_t i = 0; i<dim; i++) rhs[i] =  i+1.0;
  if (!lq.solve(rhs, x)) s++;
  
  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s += 1;
    }
  }


  return s;
}

//____________________________________________________________________
int
test_LQ_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_LQ_solve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "LQ_solve hilbert(2)");

  f =  test_LQ_solve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "LQ_solve hilbert(3)");

  f =  test_LQ_solve_dim(ts, *hilb4, hilb4_solution, 4 * 1024 * eps);
  s += f;
  ts.status(f, "LQ_solve hilbert(4)");

  f =  test_LQ_solve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "LQ_solve hilbert(12)");

  f =  test_LQ_solve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "LQ_solve vander(2)");

  f =  test_LQ_solve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "LQ_solve vander(3)");

  f =  test_LQ_solve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "LQ_solve vander(4)");

  f =  test_LQ_solve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "LQ_solve vander(12)");

  return s;
}




//____________________________________________________________________
template <typename T>
int
test_LQ_LQsolve_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, 
		    const double * actual, double eps)
{
  int s = 0;
  unsigned long dim = m.row_size();

  gslmm::matrix<T>           mt(m,true);
  gslmm::lq_decomposition<T> lq(mt);
  const gslmm::matrix<T>& q = lq.q();
  const gslmm::matrix<T>& l = lq.l();
  
  gslmm::vector<T> rhs(dim);
  gslmm::vector<T> x(dim);
  for (size_t i = 0; i<dim; i++) rhs[i] =  i+1.0;

  if (!lq.lq_solve(rhs, x)) s++;
  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s += 1;
    }
  }


  return s;
}

//____________________________________________________________________
int
test_LQ_LQsolve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_LQ_LQsolve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "LQ_LQsolve hilbert(2)");

  f =  test_LQ_LQsolve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "LQ_LQsolve hilbert(3)");

  f =  test_LQ_LQsolve_dim(ts, *hilb4, hilb4_solution, 4 * 1024 * eps);
  s += f;
  ts.status(f, "LQ_LQsolve hilbert(4)");

  f =  test_LQ_LQsolve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "LQ_LQsolve hilbert(12)");

  f =  test_LQ_LQsolve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "LQ_LQsolve vander(2)");

  f =  test_LQ_LQsolve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "LQ_LQsolve vander(3)");

  f =  test_LQ_LQsolve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "LQ_LQsolve vander(4)");

  f =  test_LQ_LQsolve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "LQ_LQsolve vander(12)");

  return s;
}


//____________________________________________________________________
template <typename T>
int
test_LQ_lssolve_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, 
		    const double * actual, double eps)
{
  int s = 0;
  unsigned long M = m.row_size(), N = m.column_size();

  gslmm::matrix<T>           mt(m,true);
  gslmm::lq_decomposition<T> lq(mt);
  
  gslmm::vector<T> res(M);
  gslmm::vector<T> rhs(M);
  gslmm::vector<T> x(N);
  for (size_t i = 0; i<M; i++) rhs[i] =  i+1.0;

  if (!lq.solve(rhs,x,res)) s++;
  for (size_t i = 0; i<N; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("(%3lu,%3lu)[%lu]: %22.18g   %22.18g\n", 
		 M, N, i, x[i], actual[i]);
      s++;
    }
  }


  /* compute residual r = b - m x */
  gslmm::vector<T> r(M);
  if (M != N) {
    r = rhs;
    gslmm::matrix_vector_product(-1., m, x, 1., r);
  };

  for (size_t i = 0; i<N; i++) {
    if (!check(res[i], r[i], sqrt(eps))) {
      ts.message("(%3lu,%3lu)[%lu]: %22.18g   %22.18g\n", M, N, i, 
		 res[i], r[i]);
      s++;
    }
  }

  return s;
}

//____________________________________________________________________
int
test_LQ_lssolve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_LQ_lssolve_dim(ts, *m53, m53_lssolution, 2 * 64 * eps);
  s += f;
  ts.status(f, "LQ_lssolve m(5,3)");

  f =  test_LQ_lssolve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "LQ_lssolve hilbert(2)");

  f =  test_LQ_lssolve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "LQ_lssolve hilbert(3)");

  f =  test_LQ_lssolve_dim(ts, *hilb4, hilb4_solution, 4 * 1024 * eps);
  s += f;
  ts.status(f, "LQ_lssolve hilbert(4)");

  f =  test_LQ_lssolve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "LQ_lssolve hilbert(12)");

  f =  test_LQ_lssolve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "LQ_lssolve vander(2)");

  f =  test_LQ_lssolve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "LQ_lssolve vander(3)");

  f =  test_LQ_lssolve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "LQ_lssolve vander(4)");

  f =  test_LQ_lssolve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "LQ_lssolve vander(12)");

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_LQ_decomp_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, double eps)
{
  int s = 0;
  unsigned long M = m.row_size(), N = m.column_size();

  gslmm::lq_decomposition<T> lq(m);

  const gslmm::matrix<T>& q = lq.q();
  const gslmm::matrix<T>& l = lq.l();
  
  /* compute a = q r */
  gslmm::matrix<T> a(l * q);
  for (size_t i = 0; i<M; i++) {
    for (size_t j = 0; j<N; j++) {
      double aij = a(i, j);
      double mij = m(i, j);
      if (!check(aij, mij, eps)) {
        ts.message("(%3lu,%3lu)[%lu,%lu]: %22.18g   %22.18g\n", 
		   M, N, i,j, aij, mij);
        s++;
      }
    }
  }
  return s;
}

//____________________________________________________________________
int
test_LQ_decomp(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_LQ_decomp_dim(ts, *m35, 2 * 8 * eps);
  s += f;
  ts.status(f, "LQ_decomp m(3,5)");

  f =  test_LQ_decomp_dim(ts, *m53, 2 * 64 * eps);
  s += f;
  ts.status(f, "LQ_decomp m(5,3)");

  f =  test_LQ_decomp_dim(ts, *hilb2, 2 * 8 * eps);
  s += f;
  ts.status(f, "LQ_decomp hilbert(2)");

  f =  test_LQ_decomp_dim(ts, *hilb3, 2 * 64 * eps);
  s += f;
  ts.status(f, "LQ_decomp hilbert(3)");

  f =  test_LQ_decomp_dim(ts, *hilb4, 4 * 1024 * eps);
  s += f;
  ts.status(f, "LQ_decomp hilbert(4)");

  f =  test_LQ_decomp_dim(ts, *hilb12, 2 * 1024 * eps);
  s += f;
  ts.status(f, "LQ_decomp hilbert(12)");

  f =  test_LQ_decomp_dim(ts, *vander2, 8 * eps);
  s += f;
  ts.status(f, "LQ_decomp vander(2)");

  f =  test_LQ_decomp_dim(ts, *vander3, 64 * eps);
  s += f;
  ts.status(f, "LQ_decomp vander(3)");

  f =  test_LQ_decomp_dim(ts, *vander4, 1024 * eps);
  s += f;
  ts.status(f, "LQ_decomp vander(4)");

  f = test_LQ_decomp_dim(ts, *vander12, 0.0005);  /* FIXME: bad accuracy */
  ts.status(f, "LQ_decomp vander(12)");
  s += f;

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_LQ_update_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, double eps)
{
  int s = 0;
  unsigned long M = m.row_size(), N = m.column_size();

  gslmm::vector<T> u(M);
  gslmm::vector<T> v(N);
  for (size_t i = 0; i<M; i++) u[i] =  sin(i+1.0);
  for (size_t i = 0; i<N; i++) v[i] =  cos(i+2.0) + sin(i*i+3.0);

  /* lq1 is updated */
  gslmm::matrix<T>          mt(m, true);
  gslmm::matrix<T>          lq1(m, true);
  rank_1_update(1., v, u, lq1);

  /* lq2 is first decomposed, updated later */
  gslmm::lq_decomposition<T> lq2(mt);

  /* compute w = Q^T u */
  gslmm::vector<T> w(M);
  gslmm::matrix_vector_product(1., lq2.q(), u, 0., w);

  /* now lq2 is updated */
  lq2.update(v,w);
  
  /* multiply q2*l2 */
  gslmm::matrix<T> ql(lq2.l() * lq2.q());

  /*  check lq1==lq2 */
  for (size_t i = 0; i<N; i++) {
    for (size_t j = 0; j<M; j++) {
      double s1 = lq1(i, j);
      double s2 = ql(i, j);
      if (!check(s1, s2, eps)) s += 1;
    }
  }


  return s;
}

//____________________________________________________________________
int
test_LQ_update(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_LQ_update_dim(ts, *m35, 2 * 512 * eps);
  s += f;
  ts.status(f, "LQ_update m(3,5)");

  f =  test_LQ_update_dim(ts, *m53, 2 * 512 * eps);
  s += f;
  ts.status(f, "LQ_update m(5,3)");

  f =  test_LQ_update_dim(ts, *hilb2,  2 * 512 * eps);
  s += f;
  ts.status(f, "LQ_update hilbert(2)");

  f =  test_LQ_update_dim(ts, *hilb3,  2 * 512 * eps);
  s += f;
  ts.status(f, "LQ_update hilbert(3)");

  f =  test_LQ_update_dim(ts, *hilb4, 2 * 1024 * eps);
  s += f;
  ts.status(f, "LQ_update hilbert(4)");

  f =  test_LQ_update_dim(ts, *hilb12, 2 * 1024 * eps);
  s += f;
  ts.status(f, "LQ_update hilbert(12)");

  f =  test_LQ_update_dim(ts, *vander2, 8 * eps);
  s += f;
  ts.status(f, "LQ_update vander(2)");

  f =  test_LQ_update_dim(ts, *vander3, 64 * eps);
  s += f;
  ts.status(f, "LQ_update vander(3)");

  f =  test_LQ_update_dim(ts, *vander4, 1024 * eps);
  s += f;
  ts.status(f, "LQ_update vander(4)");

  f = test_LQ_update_dim(ts, *vander12, 0.0005);  /* FIXME: bad accuracy */
  ts.status(f, "LQ_update vander(12)");
  s += f;

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_PTLQ_solve_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, 
		    const double * actual, double eps)
{
  int s = 0;
  unsigned long dim = m.row_size();

  gslmm::matrix<T>                 mt(m,true);
  gslmm::lq_decomposition_pivot<T> ptlq(mt);

  gslmm::vector<T> rhs(dim);
  gslmm::vector<T> x(dim);
  for (size_t i = 0; i<dim; i++) rhs[i] =  i+1.0;
  if (!ptlq.solve(rhs,x)) s++;
  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s++;
    }
  }

  return s;
}

//____________________________________________________________________
int
test_PTLQ_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_PTLQ_solve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "PTLQ_solve hilbert(2)");

  f =  test_PTLQ_solve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "PTLQ_solve hilbert(3)");

  f =  test_PTLQ_solve_dim(ts, *hilb4, hilb4_solution, 2 * 2048 * eps);
  s += f;
  ts.status(f, "PTLQ_solve hilbert(4)");

  f =  test_PTLQ_solve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "PTLQ_solve hilbert(12)");

  f =  test_PTLQ_solve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "PTLQ_solve vander(2)");

  f =  test_PTLQ_solve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "PTLQ_solve vander(3)");

  f =  test_PTLQ_solve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "PTLQ_solve vander(4)");

  f =  test_PTLQ_solve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "PTLQ_solve vander(12)");

  return s;
}


//____________________________________________________________________
template <typename T>
int
test_PTLQ_LQsolve_dim(gslmm::test_suite& ts,
		      const gslmm::matrix<T>& m, 
		      const double * actual, double eps)
{
  int s = 0;
  unsigned long dim = m.row_size();

  gslmm::matrix<T>                 mt(m,true);
  gslmm::lq_decomposition_pivot<T> ptlq(mt, true);

  gslmm::vector<T> rhs(dim);
  gslmm::vector<T> x(dim);
  for (size_t i = 0; i<dim; i++) rhs[i] =  i+1.0;
  if (!ptlq.lq_solve(rhs, x)) s++;
  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s++;
    }
  }

  return s;
}

//____________________________________________________________________
int
test_PTLQ_LQsolve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_PTLQ_LQsolve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "PTLQ_LQsolve hilbert(2)");

  f =  test_PTLQ_LQsolve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "PTLQ_LQsolve hilbert(3)");

  f =  test_PTLQ_LQsolve_dim(ts, *hilb4, hilb4_solution, 2 * 2048 * eps);
  s += f;
  ts.status(f, "PTLQ_LQsolve hilbert(4)");

  f =  test_PTLQ_LQsolve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "PTLQ_LQsolve hilbert(12)");

  f =  test_PTLQ_LQsolve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "PTLQ_LQsolve vander(2)");

  f =  test_PTLQ_LQsolve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "PTLQ_LQsolve vander(3)");

  f =  test_PTLQ_LQsolve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "PTLQ_LQsolve vander(4)");

  f =  test_PTLQ_LQsolve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "PTLQ_LQsolve vander(12)");

  return s;
}


//____________________________________________________________________
template <typename T>
int
test_PTLQ_decomp_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, double eps)
{
  int s = 0;
  unsigned long M = m.row_size(), N = m.column_size();

  gslmm::matrix<T>                 mt(m,true);
  gslmm::lq_decomposition_pivot<T> ptlq(mt, true);
  const gslmm::matrix<T>&          l = ptlq.l();
  const gslmm::matrix<T>&          q = ptlq.q();
  gslmm::matrix<T>                 a(N,M);
  
  /* compute a = l q */
  matrix_matrix_product(1., l, q, 0., a);


  /* Compute P LQ  by permuting the rows of LQ */
  const gslmm::permutation&     p = ptlq.p();
  for (size_t i = 0; i < M; i++) {
    gslmm::vector<T> col(a.column(i));
    p.permute(col, true);
  }

  for (size_t i = 0; i<M; i++) {
    for (size_t j = 0; j<N; j++) {
      double aij = a(j, i);
      double mij = m(i, j);
      if (!check(aij, mij, eps)) {
        ts.message("(%3lu,%3lu)[%lu,%lu]: %22.18g   %22.18g\n", 
		   M, N, i,j, aij, mij);
        s++;
      }
    }
  }

  return s;
}

//____________________________________________________________________
int
test_PTLQ_decomp(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_PTLQ_decomp_dim(ts, *m35, 2 * 8 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp m(3,5)");

  f =  test_PTLQ_decomp_dim(ts, *m53, 2 * 8 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp m(5,3)");

  f =  test_PTLQ_decomp_dim(ts, *s35, 2 * 8 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp s(3,5)");

  f =  test_PTLQ_decomp_dim(ts, *s53, 2 * 8 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp s(5,3)");

  f =  test_PTLQ_decomp_dim(ts, *hilb2, 2 * 8 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp hilbert(2)");

  f =  test_PTLQ_decomp_dim(ts, *hilb3, 2 * 64 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp hilbert(3)");

  f =  test_PTLQ_decomp_dim(ts, *hilb4, 2 * 1024 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp hilbert(4)");

  f =  test_PTLQ_decomp_dim(ts, *hilb12, 2 * 1024 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp hilbert(12)");

  f =  test_PTLQ_decomp_dim(ts, *vander2, 8 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp vander(2)");

  f =  test_PTLQ_decomp_dim(ts, *vander3, 64 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp vander(3)");

  f =  test_PTLQ_decomp_dim(ts, *vander4, 1024 * eps);
  s += f;
  ts.status(f, "PTLQ_decomp vander(4)");

  f = test_PTLQ_decomp_dim(ts, *vander12, 0.0005);  /* FIXME: bad accuracy */
  ts.status(f, "PTLQ_decomp vander(12)");
  s += f;

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_SV_solve_dim(gslmm::test_suite& ts,
		  const gslmm::matrix<T>& m, 
		  const double * actual, double eps)
{
  int s = 0;
  size_t dim = m.row_size();

  gslmm::sv_decomposition<T> u(m,gslmm::sv_decomposition<T>::normal);
  
  gslmm::vector<T> rhs(dim);
  gslmm::vector<T> x(dim);
  for (size_t i = 0; i<dim; i++) rhs[i] =  i+1.0;

  if (!u.solve(rhs, x)) s++;

  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s += 1;
    }
  }

  return s;
}

//____________________________________________________________________
int
test_SV_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_SV_solve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "SV_solve hilbert(2)");

  f =  test_SV_solve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "SV_solve hilbert(3)");

  f =  test_SV_solve_dim(ts, *hilb4, hilb4_solution, 2 * 1024 * eps);
  s += f;
  ts.status(f, "SV_solve hilbert(4)");

  f =  test_SV_solve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "SV_solve hilbert(12)");

  f =  test_SV_solve_dim(ts, *vander2, vander2_solution, 64 * eps);
  s += f;
  ts.status(f, "SV_solve vander(2)");

  f =  test_SV_solve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "SV_solve vander(3)");

  f =  test_SV_solve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "SV_solve vander(4)");

  f =  test_SV_solve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "SV_solve vander(12)");

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_SV_decomp_dim(gslmm::test_suite& ts,
		   const gslmm::matrix<T>& m, 
		   gslmm::sv_decomposition<double>::algorithm_t type, 
		   double eps,
		   gslmm::matrix<T>* r=0)
{
  int s = 0;
  unsigned long i,j, M = m.row_size(), N = m.column_size();
  
  gslmm::sv_decomposition<T> u(m, type);
  gslmm::matrix<T> v(u.v());
  gslmm::vector<T> sv(u.s());
  
  /* Check that singular values are non-negative and in non-decreasing
     order */  
  double di1 = 0.0;
  for (size_t i = 0; i < N; i++) {
    double di = sv[i];
    if (gsl_isnan(di)) continue;  /* skip NaNs */
    if (di < 0) {
      s++;
      ts.message("singular value %lu = %22.18g < 0\n", i, di);
    }
    if(i > 0 && di > di1) {
      s++;
      ts.message("singular value %lu = %22.18g vs previous %22.18g\n",
		 i, di, di1);
    }
    di1 = di;
  }      
  
  /* Scale dqt = S V^T */
  gslmm::matrix<T> svt(N,N);
  for (size_t i = 0; i < N ; i++) {
    double si = sv[i];
    for (size_t j = 0; j < N; j++) {
      double vji = v(j, i);
      svt(i, j) =  vji * si;
    }
  }
            
  /* compute a = v dqt */
  gslmm::matrix<T> a(M,N);
  matrix_matrix_product(1., u, svt, 0., a);

  for (size_t i = 0; i<M; i++) {
    for (size_t j = 0; j<N; j++) {
      double aij = a(i, j);
      double mij = m(i, j);
      if (!check(aij, mij, eps)) {
        ts.message("(%3lu,%3lu)[%lu,%lu]: %22.18g   %22.18g\n", 
		   M, N, i,j, aij, mij);
        s += 1;
      }
    }
  }
  if (r) *r = u;
  return s;
}

//____________________________________________________________________
template <typename T>
int
test_SV_decomp_2x2(gslmm::test_suite& ts,
		   gslmm::sv_decomposition<double>::algorithm_t type, 
		   double eps)
{
  int s = 0;
  int f = 0;
  double lower = -2, upper = 2;
  gslmm::matrix<T> a(2,2);
  for (double i1 = lower; i1 <= upper; i1++) {
    for (double i2 = lower; i2 <= upper; i2++) {
      for (double i3 = lower; i3 <= upper; i3++) {
	for (double i4 = lower; i4 <= upper; i4++) {
	  a(0, 0) =  i1;
	  a(0, 1) =  i2;
	  a(1, 0) =  i3;
	  a(1, 1) =  i4;
	  f =  test_SV_decomp_dim(ts, a, type, 16 * eps);
	  ts.status(f, "SV_decomp (2x2) A=[%g, %g; %g, %g]", i1,i2,i3,i4);
	  s += f;
	}
      }
    }
  }
  return s;
}

//____________________________________________________________________
template <typename T>
int
test_SV_decomp_nxn(gslmm::test_suite& ts, size_t n,
		   gslmm::sv_decomposition<double>::algorithm_t type, 
		   double eps)
{
  int f = 0;
  double carry = 0, lower = 0, upper = 1;
  gslmm::matrix<double> a(n,n);
  gslmm::matrix<double> r(n,n);
  
  for (size_t i = 0; i < n; i++) 
    for (size_t j = 0; j < n; j++) 
      a(i,j) = lower;
    
  while (carry == 0.0) {
    f = test_SV_decomp_dim(ts, a, type, 64 * eps, &r);
    ts.status(f, "SV_decomp (%3lux%3lu)", n, n);
    if (f) {
      for (size_t i = 0; i < n; i++) 
	for (size_t j = 0; j < n; j++) 
	  ts.message("A[%3lu,%3lu]=%g", i, j, a(i,j));
    }
    /* increment */
    carry=1.0;
    for (size_t i = n; i > 0 && i--;) {
      for (size_t j = n; j > 0 && j--;) {
	double v = a(i,j) + carry;
	carry    = (v > upper) ? 1. : 0.;
	a(i,j)   = (v > upper) ? lower : v;
      }
    }
  }
  return 0;
}


//____________________________________________________________________
int
test_SV_decomp(gslmm::test_suite& ts, 
	       gslmm::sv_decomposition<double>::algorithm_t type)
{
  int    f    = 0;
  int    s    = 0;
  double eps  = gslmm::type_trait<double>::epsilon();

  f =  test_SV_decomp_dim(ts, *m11, type, 2 * eps);
  s += f;
  ts.status(f, "SV_decomp m(1,1)");

  f =  test_SV_decomp_dim(ts, *m51, type, 2 * 64 * eps);
  s += f;
  ts.status(f, "SV_decomp m(5,1)");

  /* M<N not implemented yet */
#if 0
  f =  test_SV_decomp_dim(ts, *m35, type, 2 * 8 * eps);
  s += f;
  ts.status(f, "SV_decomp m(3,5)");
#endif
  f =  test_SV_decomp_dim(ts, *m53, type, 2 * 64 * eps);
  s += f;
  ts.status(f, "SV_decomp m(5,3)");

  f =  test_SV_decomp_dim(ts, *moler10, type, 2 * 64 * eps);
  s += f;
  ts.status(f, "SV_decomp moler(10)");

  f =  test_SV_decomp_dim(ts, *hilb2, type, 2 * 8 * eps);
  s += f;
  ts.status(f, "SV_decomp hilbert(2)");

  f =  test_SV_decomp_dim(ts, *hilb3, type, 2 * 64 * eps);
  s += f;
  ts.status(f, "SV_decomp hilbert(3)");

  f =  test_SV_decomp_dim(ts, *hilb4, type, 2 * 1024 * eps);
  s += f;
  ts.status(f, "SV_decomp hilbert(4)");

  f =  test_SV_decomp_dim(ts, *hilb12, type, 2 * 1024 * eps);
  s += f;
  ts.status(f, "SV_decomp hilbert(12)");

  f =  test_SV_decomp_dim(ts, *vander2, type, 8 * eps);
  s += f;
  ts.status(f, "SV_decomp vander(2)");

  f =  test_SV_decomp_dim(ts, *vander3, type, 64 * eps);
  s += f;
  ts.status(f, "SV_decomp vander(3)");

  f =  test_SV_decomp_dim(ts, *vander4, type, 1024 * eps);
  s += f;
  ts.status(f, "SV_decomp vander(4)");

  f =  test_SV_decomp_dim(ts, *vander12, type, 1e-4);
  s += f;
  ts.status(f, "SV_decomp vander(12)");

  f =  test_SV_decomp_dim(ts, *row3, type, 10 * eps);
  s += f;
  ts.status(f, "SV_decomp row3");

  f =  test_SV_decomp_dim(ts, *row5, type, 128 * eps);
  s += f;
  ts.status(f, "SV_decomp row5");

  f =  test_SV_decomp_dim(ts, *row12, type, 1024 * eps);
  s += f;
  ts.status(f, "SV_decomp row12");

#ifdef TEST_INF
  f =  test_SV_decomp_dim(ts, *inf5, type, 1024 * eps);
  s += f;
  ts.status(f, "SV_decomp inf5");
  
  f =  test_SV_decomp_dim(ts, *nan5, type, 1024 * eps);
  s += f;
  ts.status(f, "SV_decomp nan5");
#endif

#if 1
  f =  test_SV_decomp_2x2<double>(ts, type, eps);
  s += f;
  ts.status(f, "SV_decomp 2x2");

  f =  test_SV_decomp_nxn<double>(ts, 3, type, eps);
  s += f;
  ts.status(f, "SV_decomp 3x3");

#ifdef TEST_SVD_4X4
  f =  test_SV_decomp_nxn<double>(ts, 4, type, eps);
  s += f;
  ts.status(f, "SV_decomp 3x3");
#endif
#endif

  return s;
}

//____________________________________________________________________
int
test_SV_decomp_normal(gslmm::test_suite& ts)
{
  gslmm::sv_decomposition<double>::algorithm_t type = 
    gslmm::sv_decomposition<double>::normal;
  return test_SV_decomp(ts, type);
}

//____________________________________________________________________
int
test_SV_decomp_mod(gslmm::test_suite& ts)
{
  gslmm::sv_decomposition<double>::algorithm_t type = 
    gslmm::sv_decomposition<double>::modified;
  return test_SV_decomp(ts, type);
}

//____________________________________________________________________
int
test_SV_decomp_jacobi(gslmm::test_suite& ts)
{
  gslmm::sv_decomposition<double>::algorithm_t type = 
    gslmm::sv_decomposition<double>::jacobi;
  return test_SV_decomp(ts, type);
}

//____________________________________________________________________
template <typename T>
int
test_cholesky_solve_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, const double * actual, double eps)
{
  int s = 0;
  unsigned long i, dim = m.row_size();

  gslmm::cholesky_decomposition<T> v(m);  
  
  gslmm::vector<T> x(dim);
  gslmm::vector<T> rhs(dim);
  for (size_t i = 0; i<dim; i++) rhs[i] =  i+1.0;

  if (!v.solve(rhs,x)) s++;

  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i], actual[i], eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s ++;
    }
  }

  return s;
}

//____________________________________________________________________
int
test_cholesky_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_cholesky_solve_dim(ts, *hilb2, hilb2_solution, 2 * 8 * eps);
  s += f;
  ts.status(f, "cholesky_solve hilbert(2)");

  f =  test_cholesky_solve_dim(ts, *hilb3, hilb3_solution, 2 * 64 * eps);
  s += f;
  ts.status(f, "cholesky_solve hilbert(3)");

  f =  test_cholesky_solve_dim(ts, *hilb4, hilb4_solution, 2 * 1024 * eps);
  s += f;
  ts.status(f, "cholesky_solve hilbert(4)");

  f =  test_cholesky_solve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "cholesky_solve hilbert(12)");

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_cholesky_decomp_dim(gslmm::test_suite& ts,
			 const gslmm::matrix<T>& m, double eps)
{
  int s = 0;
  unsigned long i,j, M = m.row_size(), N = m.column_size();

  gslmm::cholesky_decomposition<T> v(m);  
  gslmm::matrix<T> l(M,N);
  gslmm::matrix<T> lt(N,N);
  // Compute L LT   
  for (size_t i = 0; i < N ; i++)  {
    for (size_t j = 0; j < N; j++) {
      double vij = v(i, j);
      l(i, j)  =  i >= j ? vij : 0;
      lt(i, j) =  i <= j ? vij : 0;
    }
  }
            
  /* compute a = l lt */
  gslmm::matrix<T> a(M,N);
  matrix_matrix_product(1., l, lt, 0., a);
  
  for (size_t i = 0; i<M; i++) {
    for (size_t j = 0; j<N; j++) {
      double aij = a(i, j);
      double mij = m(i, j);
      if (!check(aij, mij, eps)) {
        ts.message("(%3lu,%3lu)[%lu,%lu]: %22.18g   %22.18g\n", 
		   M, N, i,j, aij, mij);
        s += 1;
      }
    }
  }
  return s;
}

//____________________________________________________________________
int
test_cholesky_decomp(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_cholesky_decomp_dim(ts, *hilb2, 2 * 8 * eps);
  s += f;
  ts.status(f, "cholesky_decomp hilbert(2)");

  f =  test_cholesky_decomp_dim(ts, *hilb3, 2 * 64 * eps);
  s += f;
  ts.status(f, "cholesky_decomp hilbert(3)");

  f =  test_cholesky_decomp_dim(ts, *hilb4, 2 * 1024 * eps);
  s += f;
  ts.status(f, "cholesky_decomp hilbert(4)");

  f =  test_cholesky_decomp_dim(ts, *hilb12, 2 * 1024 * eps);
  s += f;
  ts.status(f, "cholesky_decomp hilbert(12)");

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_cholesky_decomp_unit_dim(gslmm::test_suite& ts,
			      const gslmm::matrix<T>& m, double eps)
{
  int s = 0;
  const unsigned long M = m.row_size();
  const unsigned long N = m.column_size();

  gslmm::cholesky_decomposition<T> llt(m, true);
  
  // put L and transpose(L) into separate matrices 
  gslmm::matrix<T> l(M,N);
  gslmm::matrix<T> lt(N,N);
  for (size_t i = 0; i < N ; i++) {
    for (size_t j = 0; j < N; j++) {
      const double vij = llt(i, j);
      l(i, j)  =  i >= j ? vij : 0;
      lt(i, j) =  i <= j ? vij : 0;
    }
  }

  // put D into its own matrix 
  gslmm::matrix<T> dm(M,N);
  dm.zero();
  const gslmm::vector<T>& dv = llt.d();
  for (size_t i = 0; i < M; ++i) dm(i, i) =  dv[i];

  // compute a = L * D * transpose(L); uses v for temp space 
  gslmm::matrix<T> v(M,N);
  gslmm::matrix<T> a(M,N);
  gslmm::matrix_matrix_product(1., dm, lt, 0., v);
  gslmm::matrix_matrix_product(1., l,  v,  0., a);

  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      const double aij = a(i, j);
      const double mij = m(i, j);
      if (!check(aij, mij, eps)) {
        ts.message("(%3lu,%3lu)[%lu,%lu]: %22.18g   %22.18g\n", 
		   M, N, i,j, aij, mij);
        s++;
      }
    }
  }

  return s;
}

//____________________________________________________________________
int
test_cholesky_decomp_unit(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_cholesky_decomp_unit_dim(ts, *hilb2, 2 * 8 * eps);
  s += f;
  ts.status(f, "cholesky_decomp_unit hilbert(2)");

  f =  test_cholesky_decomp_unit_dim(ts, *hilb3, 2 * 64 * eps);
  s += f;
  ts.status(f, "cholesky_decomp_unit hilbert(3)");

  f =  test_cholesky_decomp_unit_dim(ts, *hilb4, 2 * 1024 * eps);
  s += f;
  ts.status(f, "cholesky_decomp_unit hilbert(4)");

  f =  test_cholesky_decomp_unit_dim(ts, *hilb12, 2 * 1024 * eps);
  s += f;
  ts.status(f, "cholesky_decomp_unit hilbert(12)");

  return s;
}

#if 0
//____________________________________________________________________
template <typename T>
int
test_HH_solve_dim(gslmm::test_suite& ts,
                    const gslmm::matrix<T>& m, const double * actual, double eps)
{
  int s = 0;
  unsigned long i, dim = m.row_size();

  gslmm::permutation perm(dim);
  gslmm::matrix<T> hh(dim,dim);
  gslmm::vector<T> d(dim);
  gslmm::vector<T> x(dim);
  gsl_matrix_memcpy(hh,m);
  for (size_t i = 0; i<dim; i++) x[i] =  i+1.0;
  s += gsl_linalg_HH_svx(hh, x);
  for (size_t i = 0; i<dim; i++) {
    if (!check(x[i],actual[i],eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s += 1;
    }
  }
  return s;
}

//____________________________________________________________________
int
test_HH_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_HH_solve_dim(ts, *hilb2, hilb2_solution, 8 * eps);
  s += f;
  ts.status(f, "HH_solve hilbert(2)");

  f =  test_HH_solve_dim(ts, *hilb3, hilb3_solution, 128 * eps);
  s += f;
  ts.status(f, "HH_solve hilbert(3)");

  f =  test_HH_solve_dim(ts, *hilb4, hilb4_solution, 2.0 * 1024 * eps);
  s += f;
  ts.status(f, "HH_solve hilbert(4)");

  f =  test_HH_solve_dim(ts, *hilb12, hilb12_solution, 0.5);
  s += f;
  ts.status(f, "HH_solve hilbert(12)");

  f =  test_HH_solve_dim(ts, *vander2, vander2_solution, 8 * eps);
  s += f;
  ts.status(f, "HH_solve vander(2)");

  f =  test_HH_solve_dim(ts, *vander3, vander3_solution, 64 * eps);
  s += f;
  ts.status(f, "HH_solve vander(3)");

  f =  test_HH_solve_dim(ts, *vander4, vander4_solution, 1024 * eps);
  s += f;
  ts.status(f, "HH_solve vander(4)");

  f =  test_HH_solve_dim(ts, *vander12, vander12_solution, 0.05);
  s += f;
  ts.status(f, "HH_solve vander(12)");

  return s;
}


int
test_TDS_solve_dim(unsigned long dim, double d, double od, const double * actual, double eps)
{
  int s = 0;
  unsigned long i;

  gslmm::vector<T> offdiag(dim-1);
  gslmm::vector<T> diag(dim);
  gslmm::vector<T> rhs(dim);
  gslmm::vector<T> x(dim);

  for (size_t i = 0; i<dim; i++) {
    diag[i] =  d;
    rhs[i] =  i + 1.0;
  }
  for (size_t i = 0; i<dim-1; i++) {
    offdiag[i] =  od;
  }

  s += gsl_linalg_solve_symm_tridiag(diag, offdiag, rhs, x);

  for (size_t i = 0; i<dim; i++) {
    double si = x[i];
    double ai = actual[i];
    if (!check(si, ai, eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s += 1;
    }
  }

  return s;
}


//____________________________________________________________________
int
test_TDS_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  {
    double actual[] =  {0.0, 2.0};
    f =  test_TDS_solve_dim(ts, *2, 1.0, 0.5, actual, 8 * eps);
    s += f;
    ts.status(f, "solve_TDS dim=2 A");
  }

  {
    double actual[] =  {3.0/8.0, 15.0/8.0};
    f =  test_TDS_solve_dim(ts, *2, 1.0, 1.0/3.0, actual, 8 * eps);
    s += f;
    ts.status(f, "solve_TDS dim=2 B");
  }

  {
    double actual[] =  {5.0/8.0, 9.0/8.0, 2.0, 15.0/8.0, 35.0/8.0};
    f =  test_TDS_solve_dim(ts, *5, 1.0, 1.0/3.0, actual, 8 * eps);
    s += f;
    ts.status(f, "solve_TDS dim=5");
  }

  return s;
}

int
test_TDS_cyc_solve_one(const unsigned long dim, const double * d, const double * od,
                      const double * r, const double * actual, double eps)
{
  int s = 0;
  unsigned long i;

  gslmm::vector<T> offdiag(dim);
  gslmm::vector<T> diag(dim);
  gslmm::vector<T> rhs(dim);
  gslmm::vector<T> x(dim);

  for (size_t i = 0; i<dim; i++) {
    diag[i] =  d[i];
    rhs[i] =  r[i];
    offdiag[i] =  od[i];
  }

  s += gsl_linalg_solve_symm_cyc_tridiag(diag, offdiag, rhs, x);

  for (size_t i = 0; i<dim; i++) {
    double si = x[i];
    double ai = actual[i];
    if (!check(si, ai, eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s += 1;
    }
  }

  return s;
}

//____________________________________________________________________
int
test_TDS_cyc_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

#ifdef SUPPORT_UNDERSIZE_CYC
  {
    unsigned long dim = 1;
    double diag[] = {  2 };
    double offdiag[] = { 3 };
    double rhs[] = { 7 };
    double actual[] = { 3.5 };
    
    f =  test_TDS_cyc_solve_one(ts, *dim, diag, offdiag, rhs, actual, 28 * eps);
    s += f;
    ts.status(f, "solve_TDS_cyc dim=%lu A", dim);
  }

  {
    unsigned long dim = 2;
    double diag[] = { 1, 2 };
    double offdiag[] = { 3, 4 };
    double rhs[] = { 7, -7 };
    double actual[] = { -5, 4 };
    
    f =  test_TDS_cyc_solve_one(ts, *dim, diag, offdiag, rhs, actual, 28 * eps);
    s += f;
    ts.status(f, "solve_TDS_cyc dim=%lu A", dim);
  }
#endif

  {
    unsigned long dim = 3;
    double diag[] = { 1, 1, 1 };
    double offdiag[] = { 3, 3, 3 };
    double rhs[] = { 7, -7, 7 };
    double actual[] = { -2, 5, -2 };
    
    f =  test_TDS_cyc_solve_one(ts, *dim, diag, offdiag, rhs, actual, 28 * eps);
    s += f;
    ts.status(f, "solve_TDS_cyc dim=%lu A", dim);
  }

  {
    unsigned long dim = 5;
    double diag[] = { 4, 2, 1, 2, 4 };
    double offdiag[] = { 1, 1, 1, 1, 1 };
    double rhs[] = { 30, -24, 3, 21, -30 };
    double actual[] = { 12, 3, -42, 42, -21 };

    /*  f = test_TDS_cyc_solve_one(dim, diag, offdiag, rhs, actual, 7 * eps);
        FIXME: bad accuracy */
    f =  test_TDS_cyc_solve_one(ts, *dim, diag, offdiag, rhs, actual, 35 * eps);
    s += f;
    ts.status(f, "solve_TDS_cyc dim=%lu B", dim);
  }

  return s;
}

int
test_TDN_solve_dim(unsigned long dim, double d, double a, double b, const double * actual, double eps)
{
  int s = 0;
  unsigned long i;

  gslmm::vector<T> abovediag(dim-1);
  gslmm::vector<T> belowdiag(dim-1);
  gslmm::vector<T> diag(dim);
  gslmm::vector<T> rhs(dim);
  gslmm::vector<T> x(dim);

  for (size_t i = 0; i<dim; i++) {
    diag[i] =  d;
    rhs[i] =  i + 1.0;
  }
  for (size_t i = 0; i<dim-1; i++) {
    abovediag[i] =  a;
    belowdiag[i] =  b;
  }

  s += gsl_linalg_solve_tridiag(diag, abovediag, belowdiag, rhs, x);

  for (size_t i = 0; i<dim; i++) {
    double si = x[i];
    double ai = actual[i];
    if (!check(si, ai, eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s += 1;
    }
  }

  return s;
}


//____________________________________________________________________
int
test_TDN_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();
  double actual[16];

  actual[0] =  -7.0/3.0;
  actual[1] =  5.0/3.0;
  actual[2] =  4.0/3.0;
  f =  test_TDN_solve_dim(ts, *3, 1.0, 2.0, 1.0, actual, 2 * eps);
  s += f;
  ts.status(f, "solve_TDN dim=2 A");

  actual[0] =  0.75;
  actual[1] =  0.75;
  actual[2] =  2.625;
  f =  test_TDN_solve_dim(ts, *3, 1.0, 1.0/3.0, 1.0/2.0, actual, 2 * eps);
  s += f;
  ts.status(f, "solve_TDN dim=2 B");

  actual[0] =  99.0/140.0;
  actual[1] =  41.0/35.0;
  actual[2] =  19.0/10.0;
  actual[3] =  72.0/35.0;
  actual[4] =  139.0/35.0;
  f =  test_TDN_solve_dim(ts, *5, 1.0, 1.0/4.0, 1.0/2.0, actual, 35.0/8 * eps);
  s += f;
  ts.status(f, "solve_TDN dim=5");

  return s;
}

int
test_TDN_cyc_solve_dim(unsigned long dim, double d, double a, double b, const double * actual, double eps)
{
  int s = 0;
  unsigned long i;

  gslmm::vector<T> abovediag(dim);
  gslmm::vector<T> belowdiag(dim);
  gslmm::vector<T> diag(dim);
  gslmm::vector<T> rhs(dim);
  gslmm::vector<T> x(dim);

  for (size_t i = 0; i<dim; i++) {
    diag[i] =  d;
    rhs[i] =  i + 1.0;
  }
  for (size_t i = 0; i<dim; i++) {
    abovediag[i] =  a;
    belowdiag[i] =  b;
  }

  s += gsl_linalg_solve_cyc_tridiag(diag, abovediag, belowdiag, rhs, x);

  for (size_t i = 0; i<dim; i++) {
    double si = x[i];
    double ai = actual[i];
    if (!check(si, ai, eps)) {
      ts.message("%3lu[%lu]: %22.18g   %22.18g\n", dim, i, x[i], actual[i]);
      s += 1;
    }
  }
  return s;
}


//____________________________________________________________________
int
test_TDN_cyc_solve(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();
  double actual[16];

  actual[0] =  3.0/2.0;
  actual[1] = -1.0/2.0;
  actual[2] =  1.0/2.0;
  f =  test_TDN_cyc_solve_dim(ts, *3, 1.0, 2.0, 1.0, actual, 32 * eps);
  s += f;
  ts.status(f, "solve_TDN_cyc dim=2 A");

  actual[0] = -5.0/22.0;
  actual[1] = -3.0/22.0;
  actual[2] =  29.0/22.0;
  actual[3] = -9.0/22.0;
  actual[4] =  43.0/22.0;
  f =  test_TDN_cyc_solve_dim(ts, *5, 3.0, 2.0, 1.0, actual, 66 * eps);
  s += f;
  ts.status(f, "solve_TDN_cyc dim=5");

  return s;
}

//____________________________________________________________________
template <typename T>
int
test_bidiag_decomp_dim(gslmm::test_suite& ts,
		       const gslmm::matrix<T>& m, double eps)
{
  int s = 0;
  unsigned long i,j,k,r, M = m.row_size(), N = m.column_size();

  gslmm::matrix<T> A(M,N);
  gslmm::matrix<T> a(M,N);
  gslmm::matrix<T> b(N,N);

  gslmm::matrix<T> u(M,N);
  gslmm::matrix<T> v(N,N);

  gslmm::vector<T> tau1(N);
  gslmm::vector<T> tau2(N-1);
  gslmm::vector<T> d(N);
  gslmm::vector<T> sd(N-1);

  gsl_matrix_memcpy(A,m);

  s += gsl_linalg_bidiag_decomp(A, tau1, tau2);
  s += gsl_linalg_bidiag_unpack(A, tau1, u, tau2, v, d, sd);

  b.zero();
  for (i = 0; i < N; i++) b(i, i) =  d(i);
  for (i = 0; i < N-1; i++) b(i, i+1) =  sd[i];
  
  /* Compute A = U B V^T */
  
  for (i = 0; i < M ; i++) {
    for (j = 0; j < N; j++) {
      double sum = 0;
      for (k = 0; k < N; k++) {
	for (r = 0; r < N; r++) {
	  sum += u(i, k) * b(k, r) * v(j, r);
	}
      }
      a(i, j) =  sum;
    }
  }

  for (size_t i = 0; i<M; i++) {
    for (size_t j = 0; j<N; j++) {
      double aij = a(i, j);
      double mij = m(i, j);
      if (!check(aij, mij, eps)) {
        ts.message("(%3lu,%3lu)[%lu,%lu]: %22.18g   %22.18g\n", M, N, i,j, aij, mij);
        s += 1;
      }
    }
  }

  return s;
}

//____________________________________________________________________
int
test_bidiag_decomp(gslmm::test_suite& ts)
{
  int    f   = 0;
  int    s   = 0;
  double eps = gslmm::type_trait<double>::epsilon();

  f =  test_bidiag_decomp_dim(ts, *m53, 2 * 64 * eps);
  s += f;
  ts.status(f, "bidiag_decomp m(5,3)");

  f =  test_bidiag_decomp_dim(ts, *m97, 2 * 64 * eps);
  s += f;
  ts.status(f, "bidiag_decomp m(9,7)");

  f =  test_bidiag_decomp_dim(ts, *hilb2, 2 * 8 * eps);
  s += f;
  ts.status(f, "bidiag_decomp hilbert(2)");

  f =  test_bidiag_decomp_dim(ts, *hilb3, 2 * 64 * eps);
  s += f;
  ts.status(f, "bidiag_decomp hilbert(3)");

  f =  test_bidiag_decomp_dim(ts, *hilb4, 2 * 1024 * eps);
  s += f;
  ts.status(f, "bidiag_decomp hilbert(4)");

  f =  test_bidiag_decomp_dim(ts, *hilb12, 2 * 1024 * eps);
  s += f;
  ts.status(f, "bidiag_decomp hilbert(12)");

  return s;
}


//____________________________________________________________________  
template <typename T>
testit(gslmm::test_suite& ts)
{
  pair m11<T>          = { create_general_matrix<T>(1,1), 0 };
  pair m51<T>          = { create_general_matrix<T>(5,1), 0 };
  pair m35<T>          = { create_general_matrix<T>(3,5), 0 };
  pair m53<T>          = { create_general_matrix<T>(5,3), 
    {  52.5992295702070, -337.7263113752073, 351.8823436427604 } };
  pair<T> m97          = { create_general_matrix<T>(9,7), 0 };
  pair<T> s35          = { create_singular_matrix<T>(3,5), 0 };
  pair<T> s53          = { create_singular_matrix<T>(5,3), 0 };
  pair<T> hilb2        = { create_hilbert_matrix<T>(2), 
			   { -8.0, 18.0} };
  pair<T> hilb3        = { create_hilbert_matrix<T>(3), 
			   { 27.0,-192.0,210.0} };
  pair<T> hilb4        = { create_hilbert_matrix<T>(4), 
			   { -64.0,  900.0, -2520.0, 1820.0} };
  pair<T> hilb12       = { create_hilbert_matrix<T>(12), 
			   {        -1728.0,       245388.0,     -8528520.0, 
                                127026900.0,  -1009008000.0,   4768571808.0, 
                             -14202796608.0,  27336497760.0, -33921201600.0,
                              26189163000.0, -11437874448.0,   2157916488.0}};
  pair<T> vander2      = { create_vandermonde_matrix<T>(2), 
			   {1.0, 0. } };
  pair<T> vander3      = { create_vandermonde_matrix<T>(3), 
			   {0.0, 1.0, 0.0} };
  pair<T> vander4      = { create_vandermonde_matrix<T>(4), 
			   {0.0, 0.0, 1.0, 0.0} };
  pair<T> vander12     = { create_vandermonde_matrix<T>(12), 
			   { 0.0, 0.0, 0.0, 0.0,
			     0.0, 0.0, 0.0, 0.0, 
			     0.0, 0.0, 1.0, 0.0 }};
  pair<T> moler10      = { create_moler_matrix<T>(10), 0 };
  pair<T> row3         = { create_row_matrix<T>(3,3), 0 };
  pair<T> row5         = { create_row_matrix<T>(5,5), 0 };
  pair<T> row12        = { create_row_matrix<T>(12,12), 0 };
  pair<T> A22          = { create_2x2_matrix<T>(0.0, 0.0, 0.0, 0.0), 0 };
  pair<T> A33          = { new gslmm::matrix<T>(3,3), 0 };
  pair<T> A44          = { new gslmm::matrix<T>(4,4), 0 };
  pair<T> A55          = { new gslmm::matrix<T>(5,5), 0 };
  pair<T> inf5         = { create_diagonal_matrix<T>(inf5_data, 5), 
			   { 1.0, 0.0, -3.0, 0.0, -5.0} };
  (*(inf5._matrix))(3,3) = gslmm::constant::plus_infinity; //  GSL_POSINF;
  pair<T> nan5         = { create_diagonal_matrix<T>(inf5_data, 5), 0 };
  (*(nan5._matrix))(3,3) = gslmm::constant::not_a_number; //  GSL_NAN;
  
}
#endif



//____________________________________________________________________  
int 
main(int argc, char** argv) 
{
  gslmm::test_suite ts("linear", argc, argv);

  m11          = create_general_matrix<double>(1,1);
  m51          = create_general_matrix<double>(5,1);
  m35          = create_general_matrix<double>(3,5);
  m53          = create_general_matrix<double>(5,3);
  m97          = create_general_matrix<double>(9,7);
  s35          = create_singular_matrix<double>(3,5);
  s53          = create_singular_matrix<double>(5,3);
  hilb2        = create_hilbert_matrix<double>(2);
  hilb3        = create_hilbert_matrix<double>(3);
  hilb4        = create_hilbert_matrix<double>(4);
  hilb12       = create_hilbert_matrix<double>(12);
  vander2      = create_vandermonde_matrix<double>(2);
  vander3      = create_vandermonde_matrix<double>(3);
  vander4      = create_vandermonde_matrix<double>(4);
  vander12     = create_vandermonde_matrix<double>(12);
  moler10      = create_moler_matrix<double>(10);
  c7           = create_complex_matrix<double>(7);
  row3         = create_row_matrix<double>(3,3);
  row5         = create_row_matrix<double>(5,5);
  row12        = create_row_matrix<double>(12,12);
  A22          = create_2x2_matrix<double>(0.0, 0.0, 0.0, 0.0);
  A33          = new gslmm::matrix<double>(3,3);
  A44          = new gslmm::matrix<double>(4,4);
  A55          = new gslmm::matrix<double>(5,5);
  inf5         = create_diagonal_matrix<double>(inf5_data, 5);
  (*inf5)(3,3) = gslmm::constant::plus_infinity(); //  GSL_POSINF;
  nan5         = create_diagonal_matrix<double>(inf5_data, 5);
  (*nan5)(3,3) = gslmm::constant::not_a_number(); //  GSL_NAN;



  //ts.status(test_bidiag_decomp(ts),   "Bidiagonal Decomposition");
  ts.status(test_LU_solve(ts),        "LU Decomposition and Solve");
  ts.status(test_LUc_solve(ts),       "Complex LU Decomposition and Solve");

  ts.status(test_QR_decomp(ts),       "QR Decomposition");
  ts.status(test_QR_solve(ts),        "QR Solve");
  ts.status(test_QR_QRsolve(ts),      "QR QR Solve");
  ts.status(test_QR_lssolve(ts),      "QR LS Solve");
  ts.status(test_QR_update(ts),       "QR Rank-1 Update");
  ts.status(test_QRPT_decomp(ts),     "QRPT Decomposition");
  ts.status(test_QRPT_solve(ts),      "QRPT Solve");
  ts.status(test_QRPT_QRsolve(ts),    "QRPT QR Solve");

  ts.status(test_LQ_decomp(ts),        "LQ Decomposition");
  ts.status(test_LQ_solve(ts),         "LQ Solve");
  ts.status(test_LQ_LQsolve(ts),       "LQ LQ Solve");
  ts.status(test_LQ_lssolve(ts),       "LQ LS Solve");
  ts.status(test_LQ_update(ts),        "LQ Rank-1 Update");
  ts.status(test_PTLQ_solve(ts),       "PTLQ Solve");
  ts.status(test_PTLQ_LQsolve(ts),     "PTLQ LQ Solve");
  ts.status(test_PTLQ_decomp(ts),      "PTLQ Decomposition");

  ts.status(test_SV_decomp_normal(ts),"Singular Value Decomposition");
  ts.status(test_SV_decomp_jacobi(ts),"Singular Value Decomposition (Jacobi)");
  ts.status(test_SV_decomp_mod(ts),   "Singular Value Decomposition (Mod)");
  ts.status(test_SV_solve(ts),        "SVD Solve");

  ts.status(test_cholesky_decomp(ts), "Cholesky Decomposition");
  ts.status(test_cholesky_decomp_unit(ts),  
	    "Cholesky Decomposition [unit triangular]");
  ts.status(test_cholesky_solve(ts),  "Cholesky Solve");

  //ts.status(test_HH_solve(ts),         "Householder solve");

  //ts.status(test_TDS_solve(ts),        "Tridiagonal symmetric solve");
  //ts.status(test_TDS_cyc_solve(ts),    "Tridiagonal symmetric cyclic solve");

  //ts.status(test_TDN_solve(ts),        "Tridiagonal nonsymmetric solve");
  //ts.status(test_TDN_cyc_solve(ts),    "Tridiagonal nonsymmetric cyclic solve");


  return ts.summary() ? 0 : 1;
}

//____________________________________________________________________  
//
// EOF
//  
