//
// $Id: chebyshev.hh,v 1.7 2006-05-09 07:40:34 cholm Exp $ 
//  
//  gslmm::chebyshev
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
#ifndef GSLMM_chebyshev
#define GSLMM_chebyshev
#ifndef __GSL_chebyshev_h
# include <gsl/gsl_chebyshev.h>
#endif
#ifndef __CASSERT__
# include <cassert>
#endif

/** @file   chebyshev/chebyshev.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  chebyshev class */

/** @defgroup chebyshev Chebyshev Approximations. 

    This modulle describes routines for computing Chebyshev
    approximations to univariate functions.  A Chebyshev approximation
    is a truncation of the series 
    @f[ 
    f(x) = \sum c_n T_n(x)
    @f] 
    where the Chebyshev polynomials @f$ T_n(x) = \cos(n \arccos x)@f$
    provide an orthogonal basis of polynomials on the interval @f$
    [-1,1]@f$ with the weight function @f$ 1 / \sqrt{1-x^2}@f$.  The  
    first few Chebyshev polynomials are, 
    @f[ 
    T_0(x) = 1,\quad T_1(x) = x,\quad  T_2(x) = 2 x^2 - 1\quad.  
    @f]
    For further information see Abramowitz & Stegun, Chapter 22. */
namespace gslmm 
{
  /** @brief Chebyshev series approximation.
      @ingroup chebyshev
      Class representing a Chebyshev series approximation to a
      real-valued function over the real interval @f$ [a,b]@f$. 
      
      The approximation is made over the range @f$ [a,b]@f$ using
      order @f$ o@f$ terms, including the coefficient @f$ c_0@f$.  The 
      series is computed using the following convention,
      @f[ 
      f(x) = (c_0 / 2) + \sum_{n=1} c_n T_n(x)
      @f]
      which is needed when accessing the coefficients directly.

      The Chebyshev polynomials @f$ T_n(x) = \cos(n \arccos x)@f$ 
      provide an orthogonal basis of polynomials on the interval 
      @f$ [-1,1]@f$ with the weight function @f$ 1 / \sqrt{1-x^2}@f$.
      The first few Chebyshev polynomials are, 
      @f[ 
      T_0(x) = 1,\quad T_1(x) = x,\quad  T_2(x) = 2 x^2 - 1\quad.  
      @f]

      A functor 
      @dontinclude chebyshev-ex.cc
      @skip struct 
      @until };

      A function 
      @skip double 
      @until }

      The program
      @skip main 
      @until return 0;

      Below are a plot of this data. 
      @image html chebyshev-ex.png

      The plots were made, doing 
      @verbatim 
      ./chebyshev-ex > c.data
      cat c.data | awk 'BEGIN {FS=" "} { printf "%g %g\n", $1, $3}' > c.real
      cat c.data | awk 'BEGIN {FS=" "} { printf "%g %g %g\n",$1,$4,$6}' > c.10
      cat c.data | awk 'BEGIN {FS=" "} { printf "%g %g %g\n",$1,$7,$9}' > c.40
      graph -T X -X x -C c.real -I e c.10 -I e c.40 \
        -Y "f (red), f\sb10\eb (green), f\sb40\eb (blue)"
      @endverbatim 

  */
  struct chebyshev_series
  {
  public:
    /** Constructor. This member function computes the Chebyshev
	approximation for the functor @a f over the range @f$ (a,b)@f$
	to the specified order @a n. The computation of the
	Chebyshev approximation is an @f$ O(n^2)@f$ process, and
	requires @f$ n@f$ function evaluations. 
	@param f     Functor object to evaluate 
	@param n     Order of the expansion.
	@param a     Lower bound of range
	@param b     Upper bound of range */
    template <typename T>
    chebyshev_series(T& f, const size_t n, const double a, const double b) 
    {
      _cheb_series  = gsl_cheb_alloc(n);
      gsl_function eval;
      eval.params   = &f;
      eval.function = &eval_functor<T>;
      gsl_cheb_init(_cheb_series, &eval, a, b);
    }
    /** Constructor. This member function computes the Chebyshev
	approximation for the functor @a f over the range @f$ (a,b)@f$
	to the specified order @a n. The computation of the
	Chebyshev approximation is an @f$ O(n^2)@f$ process, and
	requires @f$ n@f$ function evaluations. 
	@param f     Function object to evaluate 
	@param n     Order of the expansion.
	@param a     Lower bound of range
	@param b     Upper bound of range */
    chebyshev_series(double (*f)(double), const size_t n, 
		     const double a, const double b) 
    {
      _cheb_series  = gsl_cheb_alloc(n);
      gsl_function eval;
      typedef double (*func_t)(double);
      eval.params   = (void*)(f);
      eval.function = &eval_f;
      gsl_cheb_init(_cheb_series, &eval, a, b);
    }
    /** Destructor. */
    virtual ~chebyshev_series() 
    {
      if (_cheb_series) gsl_cheb_free(_cheb_series);
      _cheb_series = 0;
    }
    /** This member function computes the Chebyshev series at a
	given point @a x.
	@return The Chebyshev series evaluated at @a x */
    double operator()(const double x) 
    {
      double v = gsl_cheb_eval(_cheb_series, x);
      return v;
    }
    /** This member function computes the Chebyshev series at a
	given point @a x, estimating both the series result and its
	absolute error (returned in @a e). The error estimate is made
	from the first neglected term in the series.
	@param x Where to evaluate the series. 
	@param e on return, it contains the absolute error of the
	evaluation. 
	@return The Chebyshev series evaluated at @a x */
    double operator()(const double x, double& e) 
    {
      double v;
      int ret = gsl_cheb_eval_err(_cheb_series, x, &v, &e);
      (void)ret;
      return v;
    }
    /** This member function evaluates the Chebyshev series at a
	given point @a x, to (at most) the given order @a n.
	@param n Maximum order to evaluate to. 
	@param x Where to evaluate the series. 
	@return The Chebyshev series evaluated at @a x */
    double operator()(const size_t n, const double x) 
    {
      double v = gsl_cheb_eval_n(_cheb_series, n, x);
      return v;
    }
    /** This member function evaluates a Chebyshev series at a given
	point @a x,  estimating both the series result and its
	absolute error (returned in @a e), to (at most) the given
	order @a n.  The  error estimate is made from the first
	neglected term in the series. 
	@param n Maximum order to evaluate to. 
	@param x Where to evaluate the series. 
	@param e on return, it contains the absolute error of the
	evaluation. 
	@return The Chebyshev series evaluated at @a x */
    double operator()(const size_t n, const double x, double& e) 
    {
      double v;
      int ret = gsl_cheb_eval_n_err(_cheb_series, n, x, &v, &e);
      (void)ret;
      return v;
    }
    /** This member function computes the derivative of the series,
	returning a new series of the derivative coefficients.  The 
	returned series has the same order as this series. 
	@return The chebyshev series representing the derivative of
	this series. */
    chebyshev_series derivative() const 
    {
      chebyshev_series r(_cheb_series->order);
      int ret = gsl_cheb_calc_deriv(r._cheb_series, _cheb_series);
      return r;
    }
    /** This member function computes the integral of the series,
	returning the integral coefficients in the a new series
	representation. The returned series has the same order as this
	series. The lower limit of the integration is taken to be the
	left hand end of the range A.
	@return The chebyshev series representing the integral of this
	series. */
    chebyshev_series integral() const 
    {
      chebyshev_series r(_cheb_series->order);
      int ret = gsl_cheb_calc_integ(r._cheb_series, _cheb_series);
      return r;
    }
    /** @return Array of @e order number of coefficents. */
    const double* coefficents() const 
    {
      assert(_cheb_series);
      return _cheb_series->c;
    }
    /** @param i The coefficent number to get, starting from 0.  Note,
	any number coefficent with @f$ i > o@f$ (where @f$ o@f$ is the
	order of the expansion) is considered to be exactly zero. 
	@return order of the expansion */
    double coefficent(size_t i) const
    {
      assert(_cheb_series);
      return (i >= _cheb_series->order + 1) ? 0 : _cheb_series->c[i];
    }
    /** @return order of the expansion */
    size_t order() const 
    {
      assert(_cheb_series);
      return _cheb_series->order;
    }
    /** @return lower bound of validity interval */
    double a() const 
    {
      assert(_cheb_series);
      return _cheb_series->a;
    }
    /** @return upper bound of validity interval */
    double b() const 
    {
      assert(_cheb_series);
      return _cheb_series->b;
    }
  protected:
    /** The chebyshev series */
    gsl_cheb_series* _cheb_series;
    /** evaluated a functor or function */
    template <typename T> 
    static double eval_functor(double x, void* params) 
    {
      T* f = static_cast<T*>(params);
      return (*f)(x);
    }
    static double eval_f(double x, void* params) 
    {
      typedef double (*func_t)(double);
      func_t f = (func_t)(params);
      return (*f)(x);
    }
  private:
    /** Default constructor - hidden */
    chebyshev_series() 
      : _cheb_series(0)
    {}
    /** Constructor.  Only allocates space for an @a n order
	expansion. 
	@param order Order of the expansion. */
    chebyshev_series(size_t order) 
    {
      _cheb_series = gsl_cheb_alloc(order);
    }
  };
}
  
#endif
//____________________________________________________________________
//
// EOF
//
