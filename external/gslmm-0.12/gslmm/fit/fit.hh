//
// $Id: fit.hh,v 1.6 2007-04-21 09:03:11 cholm Exp $ 
//  
//  gslmm::fit
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
#ifndef GSLMM_fit_fit
#define GSLMM_fit_fit
#ifndef _GSL_MULTIFIT_NLIN_H
# include <gsl/gsl_multifit_nlin.h>
#endif
#ifndef GSLMM_VECTOR_DOUBLE_HH
# include <gslmm/vectormatrix/vector_double.hh>
#endif
#ifndef GSLMM_MATRIX_DOUBLE_HH
# include <gslmm/vectormatrix/matrix_double.hh>
#endif

/** @file   fit/fit.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  fit class */

/** @defgroup fit Function Fit (1 and multi-dimensional)

    This chapter describes functions for multidimensional linear and
    nonlinear least-squares fitting.  The library provides low level
    components for a variety of iterative solvers and convergence
    tests.  These can be combined by the user to achieve the desired
    solution, with full access to the intermediate steps of the
    iteration.  Each class of methods uses the same framework, so that
    you can switch between solvers at runtime without needing to
    recompile your program.  Each instance of a solver keeps track of
    its own state, allowing the solvers to be used in multi-threaded
    programs.

    @todo Check these classes  
    @todo Implement test-suite 
*/
namespace gslmm 
{
  namespace  
  {
    /** Helper function for fits
	@ingroup fit
	@param x The current values the parameters
	@param params A pointer to the object to fit
	@param f On return, the function to minimize evaluated at all
	data points 
	@return If the evaluation worked, 0, otherwise an appropriate
	error code */
    template <typename Client>
    int fit_f_function(const gsl_vector* x, void* params, gsl_vector* f)
    {
      if (!params) return GSL_EINVAL;
      Client* client = static_cast<Client*>(params);
      const gslmm::vector<double> xv(x->data, x->size);
      gslmm::vector<double> fv(f->data, f->size);
      int ret = client->function(xv, fv);
      return ret;
    }
    /** Helper function for fits
	@ingroup fit
	@param x The current values the parameters
	@param params A pointer to the object to fit
	@param j On return, the Jacobian matrix 
	@f$ j_{ij} = d f_i(x) / d x_j @f$ 
	@return If the evaluation worked, 0, otherwise an appropriate
	error code */
    template <typename Client>
    int fit_df_function(const gsl_vector* x, void* params, gsl_matrix* j)
    {
      if (!params) return GSL_EINVAL;
      Client* client = static_cast<Client*>(params);
      const gslmm::vector<double> xv(x->data, x->size);
      gslmm::matrix<double> jv(j->data, j->size1,j->size2);
      return client->jacobian(xv, jv);

    }
    /** Helper function for fits
	@ingroup fit
	@param x The current values the parameters
	@param params A pointer to the object to fit
	@param f On return, the function to minimize evaluated at all
	data points 
	@param j On return, the Jacobian matrix 
	@f$ j_{ij} = d f_i(x) / d x_j @f$ 
	@return If the evaluation worked, 0, otherwise an appropriate
	error code */
    template <typename Client>
    int fit_fdf_function(const gsl_vector* x, void* params, 
			 gsl_vector* f, gsl_matrix* j)
    {
      if (!params) return GSL_EINVAL;
      Client* client = static_cast<Client*>(params);
      const gslmm::vector<double> xv(x->data, x->size);
      gslmm::vector<double> fv(f->data, f->size);
      gslmm::matrix<double> jv(j->data, j->size1,j->size2);
      int ret = GSL_SUCCESS;
      if ((ret = client->function(xv, fv)) != GSL_SUCCESS) return ret;
      ret = client->jacobian(xv, jv);
      return ret;
    }
  }
  
  //__________________________________________________________________
  /** @class fitter fit/fit.hh <gslmm/fit/fit.hh>
      @brief Class template for fitting arbitriary objects. 
      @ingroup fit

      The problem of multidimensional nonlinear least-squares fitting
      requires the minimization of the squared residuals of @f$ n@f$
      functions, @f$ f_i@f$, in @f$ p@f$ parameters, @f$ x_i@f$,
      @f{eqnarray*}
      \Phi(x) &=& (1/2) \| F(x) \|^2\\
              &=& (1/2) \sum_{i=1}^{n} f_i\left(x_1, \ldots, x_p\right)^2
      @f}	     
      All algorithms proceed from an initial guess using the
      linearization, 
      @f[ 
      \psi(p) = \| F(x+p) \| \approx \| F(x) + J p \|
      @f]
      where @f$ x@f$ is the initial point, @f$ p@f$ is the proposed
      step and @f$ J@f$ is the Jacobian matrix @f$ J_{ij} = \partial
      f_i / \partial x_j@f$.  Additional strategies are used to
      enlarge the region of convergence.  These include requiring a
      decrease in the norm @f$ \|F\|@f$ on each step or using a trust
      region to avoid steps which fall outside the linear regime.

      To perform a weighted least-squares fit of a nonlinear model @f$
      Y(x,t)@f$ to data @f$ (t_i, y_i)@f$ with independent gaussian
      errors @f$ \sigma_i@f$, use function components of the following
      form, 
      @f[ 
      f_i = (Y(x, t_i) - y_i) / \sigma_i
      @f]
      Note that the model parameters are denoted by @f$ x@f$ in this
      module since the non-linear least-squares algorithms are
      described geometrically (i.e. finding the minimum of a surface).
      The independent variable of any data to be fitted is denoted by
      @f$ t@f$. 

      With the definition above the Jacobian is 
      @f[ J_{ij} =(1 / \sigma_i)  \frac{\partial Y_i}{\partial
      x_j}\quad, 
      @f]
      where @f$ Y_i = Y(x,t_i)@f$.

      @include fit-ex.cc 
      @include fit-gaussian-ex.cc 
   */
  template <typename Client> 
  class fitter 
  {
  public:
    /** Type of client */
    typedef Client client_type;
    /** Types of fitters */
    enum type_enum {
      scaled_levenberg_marquardt, 
      levenberg_marquardt
    };
    /** Constructor x*/
    fitter();
    /** Destructor */
    virtual ~fitter();
    /** This member function initializes of a solver of type @a t for
	@a ns observations and with initial parameters @a guess.  The
	number of observations @a ns must be greater than or equal to
	parameters in @a guess. 
	@return @c false on error */
    bool initialize(client_type& c, type_enum t, 
		    gslmm::vector<double>& guess, size_t ns);
    /** Get the name of the current solver used */
    const char* name() const;
    /** This member functions drive the iteration of each algorithm.
	It performs one iteration to update the state of the solver.
	
	This member function perform a single iteration of the solver.
	If the iteration encounters an unexpected problem then an
	error code will be returned.  The solver maintains a current
	estimate of the  best-fit parameters at all times.
	@return 0 on success, error code otherwise */
    int iterate();
    /** @{ 
	@name Current state member functions */
    /** @return  The current parameter values  */
    gslmm::vector<double> position() const;
    /** @return  The current function values */
    gslmm::vector<double> values() const;
    /** @return  The change in the parameters from the last iteration */
    gslmm::vector<double> change() const;
    /** @return  The jacobian of the parameters from the last iteration */
    gslmm::matrix<double> jacobian() const;
    /** Compute the gradient of @f$ \Phi(x) = 1/2 ||F(x)||^2@f$ given
	by @f$ g = J^T f@f$ where @f$ J@f$ is the Jacobian, and 
	@f$ f@f$ is the function values.  
	@param g The gradient
	@return  true on success */
    bool gradient(gslmm::vector<double>& g) const;
    /** @} */
    /** Use the Jacobian matrix @f$ J@f$ to compute the covariance 
	matrix of the best-fit parameters.  The parameter
	@a epsrel is used to remove linear-dependent columns when 
	@f$ J@f$ is  rank deficient. 

	The covariance matrix is given by,
	@f[
	c = (J^T J)^{-1}
	@f]
	and is computed by QR decomposition of @f$ J@f$ with
	column-pivoting.  Any columns of @f$ R@f$ which satisfy
	@f[
	|R_{kk}| <= \epsilon_{rel} |R_{11}|
	@f]
	are considered linearly-dependent and are excluded from the 
	covariance matrix (the corresponding rows and columns of the 
	covariance matrix are set to zero).
	@param c On return the covariance matrix.  The matrix should
	be square, and have the same dimensions as the number of
	parameters. 

	The error of parameter @a i, is given by @f$ \sqrt{c_{ii}}@f$ 
	@param epsrel @f$ \epsilon_{rel}@f$ in the above equation. 
	@return  true on success */
    bool covariance(gslmm::matrix<double>& c, double epsrel) const;
    /** @{ 
	@name End-of-minimization conditions.
	A minimization procedure should stop when one of the following
	conditions is true:

	- A minimum has been found to within the user-specified
          precision. 
	- A user-specified maximum number of iterations has been
          reached. 
	- An error has occurred.

	The handling of these conditions is under user control.  The 
	member functions below allow the user to test the current 
	estimate of the best-fit parameters in several standard
	ways. */ 
    /** Test if, in the last iteration, that the sequence converges.
	The test returns true if 
	@f[
	|dx_i| < \epsilon_{abs} + \epsilon_{rel} |x_i|
	@f]
	@param epsabs @f$ \epsilon_{abs}@f$ in the above equation.
	@param epsrel @f$ \epsilon_{rel}@f$ in the above equation.
	@return true if the above relation is true */
    bool test_delta(double epsabs, double epsrel) const;
    /** Test if the fit is close enough to the minimum.
	Mathematically, the gradient should be exactly 0 at the
	minimum.  This member function return true if 
	@f[
	\sum_i |g_i| < \epsilon_{abs} 
	@f]
	where @f$ g_i@f$ is the elements of the gradient of the
	residuals. 
	@param epsabs @f$ \epsilon_{abs}@f$ in the above expression
	@return true if the above relation holds */
    bool test_gradient(double epsabs) const;
    /** @} */
  protected:
    /** Pointer to the current fitter solver */
    struct solver_t
    {
      gsl_multifit_fsolver*   _f;
      gsl_multifit_fdfsolver* _fdf;
    } _solver;
    /** Pointer to the current fitter type */
    struct type_t
    {
      const gsl_multifit_fsolver_type*   _f;
      const gsl_multifit_fdfsolver_type* _fdf;
    } _type;
    /** Pointer to the current fit function */
    struct func_t
    {
      gsl_multifit_function     _f;
      gsl_multifit_function_fdf _fdf;
    } _function;
    /** Pointer to client */
    client_type* _client;
    /** Whether we use derivatives explicitly */
    bool _use_derivative;
    /** Initialize a fiter that does not use derivatives 
	@param t The type 
	@param g the initial guess  
	@param ns The number of data samples
	@return  true if the initialization went well */
    bool init_f(const gsl_multifit_fsolver_type* t, gsl_vector* g, size_t ns);
    /** Initialize a fiter that uses derivatives 
	@param t The type 
	@param g the initial guess  
	@param ns The number of data samples
	@return  true if the initialization went well */
    bool init_fdf(const gsl_multifit_fdfsolver_type* t, gsl_vector* g, 
		  size_t ns);
  };

  //__________________________________________________________________
  template <typename Client> 
  inline
  fitter<Client>::fitter() 
    : _client(0), _use_derivative(false)
  {
    _solver._f   = 0;
    _solver._fdf = 0;
    _type._f     = 0;
    _type._fdf   = 0;
  }
  
  //__________________________________________________________________
  template <typename Client> 
  inline
  fitter<Client>::~fitter() 
  {
    if (!_use_derivative) gsl_multifit_fsolver_free(_solver._f);
    else                  gsl_multifit_fdfsolver_free(_solver._fdf);
  }
  
  //__________________________________________________________________
  template <typename Client> 
  inline bool
  fitter<Client>::initialize(client_type& c, type_enum t, 
			     gslmm::vector<double>& guess, size_t ns) 
  {
    bool ret = true;
    _client = &c;
    gsl_vector_view g(gsl_vector_view_array(guess.element(0), 
					    guess.size()));
    switch (t) {
    case scaled_levenberg_marquardt:
      ret = init_fdf(gsl_multifit_fdfsolver_lmsder, &(g.vector), ns);
      break;
      
    case levenberg_marquardt:
      ret = init_fdf(gsl_multifit_fdfsolver_lmder, &(g.vector), ns);
      break;
    }    
  }
  //__________________________________________________________________
  template <typename Client> 
  inline bool
  fitter<Client>::init_f(const gsl_multifit_fsolver_type* t, 
			 gsl_vector* guess, size_t ns)
  {
    
    if (_solver._f) gsl_multifit_fsolver_free(_solver._f);
    _use_derivative     = false;
    _function._f.f      = &fit_f_function<Client>;
    _function._f.n      = ns;
    _function._f.p      = guess->size;
    _function._f.params = _client;
    _type._f            = t;
    _solver._f          = gsl_multifit_fsolver_alloc(_type._f, ns, 
						      guess->size);
    if (!_solver._f) return false;
    return GSL_SUCCESS ==
      gsl_multifit_fsolver_set(_solver._f, &(_function.f), guess);
  }
  
  //__________________________________________________________________
  template <typename Client> 
  inline bool
  fitter<Client>::init_fdf(const gsl_multifit_fdfsolver_type* t, 
			   gsl_vector* guess, size_t ns)
  {
    if (_solver._fdf) gsl_multifit_fdfsolver_free(_solver._fdf);
    _use_derivative       = true;
    _function._fdf.f      = &fit_f_function<Client>;
    _function._fdf.df     = &fit_df_function<Client>;
    _function._fdf.fdf    = &fit_fdf_function<Client>;
    _function._fdf.n      = ns;
    _function._fdf.p      = guess->size;
    _function._fdf.params = _client;
    _type._fdf            = t;
    _solver._fdf          = gsl_multifit_fdfsolver_alloc(_type._fdf, ns, 
							 guess->size);
    if (!_solver._fdf) return false;
    return GSL_SUCCESS ==
      gsl_multifit_fdfsolver_set(_solver._fdf, &(_function._fdf), guess);
  }
  //__________________________________________________________________
  template <typename Client> 
  inline const char*
  fitter<Client>::name() const
  {
    if (_use_derivative && _solver._fdf) 
      return gsl_multifit_fdfsolver_name(_solver._fdf);
    else if (_solver.f)
      return gsl_multifit_fdfsolver_name(_solver._f);
    return 0;
  }
  //__________________________________________________________________
  template <typename Client> 
  inline int
  fitter<Client>::iterate() 
  {
    if (!_client) return GSL_EINVAL;
    int ret = 0;
    if (!_use_derivative) ret = gsl_multifit_fsolver_iterate(_solver._f);
    else                  ret = gsl_multifit_fdfsolver_iterate(_solver._fdf);
    if (ret != GSL_SUCCESS) return ret;
    _client->iterate();
    return ret;
  }
  //__________________________________________________________________
  template <typename Client> 
  inline gslmm::vector<double>
  fitter<Client>::position() const
  {
    gsl_vector* p = (!_use_derivative ? _solver._f->x : _solver._fdf->x);
    return gslmm::vector<double>(p->data, p->size, p->stride);
  }
  //__________________________________________________________________
  template <typename Client> 
  inline gslmm::vector<double>
  fitter<Client>::values() const
  {
    gsl_vector* f = (!_use_derivative ? _solver._f->f : _solver._fdf->f);
    return gslmm::vector<double>(f->data, f->size, f->stride);
  }
  //__________________________________________________________________
  template <typename Client> 
  inline gslmm::vector<double>
  fitter<Client>::change() const 
  {
    gsl_vector* dx = (!_use_derivative ? _solver._f->dx : _solver._fdf->dx);
    return gslmm::vector<double>(dx->data, dx->size, dx->stride);
  }
  //__________________________________________________________________
  template <typename Client> 
  inline gslmm::matrix<double>
  fitter<Client>::jacobian() const
  {
    gsl_matrix* j = (!_use_derivative ? _solver._f->J : _solver._fdf->J);
    return gslmm::matrix<double>(j->data, j->size1, j->size2, j->tda);
  }
  //__________________________________________________________________
  template <typename Client> 
  inline bool
  fitter<Client>::gradient(gslmm::vector<double>& g) const
  {
    gsl_matrix* j = (!_use_derivative ? _solver._f->J : _solver._fdf->J);
    gsl_vector* f = (!_use_derivative ? _solver._f->f : _solver._fdf->f);
    if (g.size() != f->size) return false;
    gsl_vector_view gv = gsl_vector_view_array(g.element(0), g.size());
    return GSL_SUCCESS == gsl_multifit_gradient(j, f, &(gv.vector));
  }
  //__________________________________________________________________
  template <typename Client> 
  inline bool
  fitter<Client>::covariance(gslmm::matrix<double>& c, double epsrel) const
  {
    if (!_use_derivative) return true;
    gsl_matrix* j = _solver._fdf->J;
    if (c.row_size() != j->size2 || c.column_size() != j->size2) 
      return false;
    gsl_matrix_view cv = gsl_matrix_view_array(c.element(0,0), 
					       c.row_size(), c.column_size());
    return GSL_SUCCESS == gsl_multifit_covar(j, epsrel, &(cv.matrix));
  }
  //__________________________________________________________________
  template <typename Client> 
  inline bool
  fitter<Client>::test_delta(double epsabs, double epsrel) const
  {
    gsl_vector* p = (!_use_derivative ? _solver._f->x : _solver._fdf->x);
    gsl_vector* dx = (!_use_derivative ? _solver._f->dx : _solver._fdf->dx);
    int ret = gsl_multifit_test_delta(dx, p, epsabs, epsrel);
    return GSL_SUCCESS == ret;
  }
  //__________________________________________________________________
  template <typename Client> 
  inline bool
  fitter<Client>::test_gradient(double epsabs) const
  {
    if (!_use_derivative) return true;
    gsl_matrix* j = _solver._fdf->J;
    gsl_vector* f = (!_use_derivative ? _solver._f->f : _solver._fdf->f);
    gsl_vector* g = gsl_vector_alloc(f->size);
    gsl_multifit_gradient(j, f, g);
    int ret = gsl_multifit_test_gradient(g, epsabs);
    gsl_vector_free(g);
    return GSL_SUCCESS == ret;
  }
}

#endif
//____________________________________________________________________
//
// EOF
//
