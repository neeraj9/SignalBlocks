//
// $Id: linear.hh,v 1.4 2006-04-29 01:05:55 cholm Exp $ 
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
#ifndef GSLMM_fit_linear
#define GSLMM_fit_linear
#ifndef _GSL_FIT_H
# include <gsl/gsl_fit.h>
#endif
#ifndef _GSL_MULTIFIT_H
# include <gsl/gsl_multifit.h>
#endif
#ifndef GSLMM_VECTOR_DOUBLE_HH
# include <gslmm/vectormatrix/vector_double.hh>
#endif
#ifndef GSLMM_MATRIX_DOUBLE_HH
# include <gslmm/vectormatrix/matrix_double.hh>
#endif

/** @file   fit/linear.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  fit class */

namespace gslmm 
{
  namespace 
  {
    //==================================================================
    /** @class basic_linear linear.hh <gslmm/fit/linear.hh>
	@brief Base class for linear fitting classes. 
	@ingroup fit
     */
    class basic_linear 
    {
    public:
      /** Data type */
      typedef double data_type;
    protected:
      /** The covariance matrix */
      gslmm::matrix<data_type> _covar;
      /** Parameter values after fit */
      gslmm::vector<data_type> _param;
      /** The @f$\chi^2 @f$ after fit, -1 before */
      data_type                _chisq;
      /** Number of degrees of freedom (# data points - # of
	  parameters) */
      size_t _ndf;
      /** Protected constructor (objects not constructable)
	  @param np The number of parameters in the fit. */
      basic_linear(size_t np) 
	: _covar(np, np), _param(np), _chisq(-1), _ndf(0)
      {}
      /** Protected copy constructor (objects not copyable)
	  @param o  Object to copy from */
      basic_linear(const basic_linear& o) 
	: _covar(o._covar), _param(o._param), _chisq(o._chisq), _ndf(o._ndf)
      {}
      /** Protected assignment operator (objects not assignable)
	  @param o Oject to assign from
	  @return reference to this object. */
      basic_linear& operator=(const basic_linear& o) 
      {
	_covar = o._covar;
	_param = o._param;
	_chisq = o._chisq;
	_ndf   = o._ndf;
	return *this;
      }
    public:
      /** @return The covariance matrix */
      const gslmm::matrix<data_type>& covarience() const 
      {
	return _covar;
      }
      /** @return The parameters */
      const gslmm::vector<data_type>& parameters() const 
      {
	return _param;
      }
      /** @param i Number of parameter to get
	  @return  The @a i th parameter */
      const data_type parameter(size_t i) const { return _param[i]; }
      /** @return The @f$\chi^2 @f$ of the fit, or -1 in case the fit
	  wasn't performed.  */
      const data_type chi_square() const { return _chisq; }
      /** @return the number of degrees of freedom */
      const size_t degrees_of_freedom() const { return _ndf; }
    };
  }
  
  //==================================================================
  /** @class linear fit/linear.hh <gslmm/fit/linear.hh>
      @brief Class template to do @f$ \chi^2@f$ fits to experimental
      data using linear combinations of functions. 

      The data may be weighted or unweighted.  For weighted data the
      functions compute the best fit parameters and their associated
      covariance matrix.  For unweighted data the covariance matrix is
      estimated from the scatter of the points, giving a
      variance-covariance matrix. 

      This class perform least-squares fits to a general linear model,
      @f$ y = X c@f$ where @f$ y@f$ is a vector of @e n observations,
      @f$ X@f$ is an @f$ n\times p@f$ matrix of predictor variables,
      and the elements of the vector @f$ c @f$ are the @a np unknown
      best-fit parameters which are to be estimated. 

      The best-fit is found by minimizing the weighted sums of squared
      residuals, @f$ \chi^2@f$,
      @f[
      \chi^2 = (y - X c)^T W (y - X c)
      @f]
      with respect to the parameters @f$ c@f$. The weights are
      specified by the diagonal elements of the @f$ n\times n@f$
      matrix @f$ W@f$.  For unweighted data @f$ W@f$ is replaced by
      the identity matrix. 

      This formulation can be used for fits to any number of functions 
      and/or variables by preparing the @f$ n\times p@f$ matrix @f$
      X@f$ appropriately.  For example, to fit to a @f$ p@f$-th order 
      polynomial in @f$ X@f$, use the following matrix,
      @f[
      X_{ij} = x_i^j
      @f]
      where the index @a i runs over the observations and the index @a
      j runs from 0 to @f$ p-1@f$.

      To fit to a set of @f$ p@f$ sinusoidal functions with fixed
      frequencies  @f$ \omega_1, \omega_2, \ldots, \omega_p@f$, use, 
      @f[
      X_{ij} = \sin(\omega_j x_i)
      @f]
      To fit to @f$ p@f$ independent variables @f$ x_1, x_2, \ldots,
      x_p,@f$  use,
      @f[
      X_{ij} = x_j(i)
      @f]
      where @f$ x_j(i)@f$ is the @f$ i@f$-th value of the predictor
      variable @f$ x_j@f$. 

      The solution of the general linear least-squares system requires an
      additional working space for intermediate results, such as the
      singular value decomposition of the matrix @f$ X@f$.

      @include linear-ex.cc 
      @param np The number of parameters in the fit. 
      @ingroup fit
   */
  template <size_t np>
  class linear : public basic_linear
  {
  private:
    /** Workspace */
    gsl_multifit_linear_workspace* _workspace;
    /** Resize the workspace if needed. 
	@param ns Number of samples. 
	@return true on success */
    bool resize(size_t ns) 
    {
      if (_workspace && _workspace->n != ns) {
	gsl_multifit_linear_free(_workspace);
	_workspace = 0;
      }
      if (!_workspace) 
	_workspace = gsl_multifit_linear_alloc(ns, np);
      if (!_workspace) return false;
      return true;
    }
    /** The rank of the single value decomposition */
    size_t _rank;
  public:
    /** Create a fitter  */
    linear() : basic_linear(np), _workspace(0), _rank(0) {}
    /** Destroy a fitter */
    ~linear() { if (_workspace) gsl_multifit_linear_free(_workspace); }
    /** Fit the data points @a y observed at @a x to the model 
	@param x independent variable data
	@param y dependent variable data 
	@return true on success */
    bool fit(const gslmm::matrix<data_type>& x, 
	     const gslmm::vector<data_type>& y)
    {
      if (x.row_size() > y.size() || x.column_size() != np) return false;
      if (!resize(x.row_size())) return false;
      gsl_vector_view yv = 
	gsl_vector_view_array(const_cast<data_type*>(y.element(0)),
			      x.row_size());
      gsl_vector_view pv = 
	gsl_vector_view_array(const_cast<data_type*>(_param.element(0)), np);
      gsl_matrix_view xv = 
	gsl_matrix_view_array(const_cast<data_type*>(x.element(0,0)),
			      x.row_size(), np);
      gsl_matrix_view cv = 
	gsl_matrix_view_array(const_cast<data_type*>(_covar.element(0,0)),
			      np, np);
      int ret = gsl_multifit_linear(&(xv.matrix), &(yv.vector), &(pv.vector), 
				    &(cv.matrix), &_chisq, _workspace);
      _ndf = x.row_size() - np;
      return ret == GSL_SUCCESS;
    }
    /** Fit the data points @a y observed at @a x to the model 
	@param x independent variable data
	@param y dependent variable data 
	@param tolerance If a components ratio of singular values 
	@f$ s_i/s_0@f$  falls below this value, it is discarded
	@return true on success */
    bool fit(const gslmm::matrix<data_type>& x, 
	     const gslmm::vector<data_type>& y, 
	     const data_type& tolerance)
    {
      if (x.row_size() > y.size() || x.column_size() != np) return false;
      if (!resize(x.row_size())) return false;
      if (x.row_size() > y.size() || x.column_size() != np) return false;
      if (!resize(x.row_size())) return false;
      gsl_vector_view yv = 
	gsl_vector_view_array(const_cast<data_type*>(y.element(0)),
			      x.row_size());
      gsl_vector_view pv = 
	gsl_vector_view_array(const_cast<data_type*>(_param.element(0)), np);
      gsl_matrix_view xv = 
	gsl_matrix_view_array(const_cast<data_type*>(x.element(0,0)),
			     x.row_size(), np);
      gsl_matrix_view cv = 
	gsl_matrix_view_array(const_cast<data_type*>(_covar.element(0,0)),
			      np, np);
      int ret = gsl_multifit_linear_svd(&(xv.matrix), &(yv.vector), 
					tolerance, &_rank, &(pv.vector), 
					&(cv.matrix), &_chisq, 
					this->_workspace);
      _ndf = x.row_size() - np;
      return ret == GSL_SUCCESS;
    }
    /** Fit the data points @a y observed at @a x to the model 
	@param x independent variable data
	@param w Weights
	@param y dependent variable data 
	@return true on success */
    bool fit(const gslmm::matrix<data_type>& x, 
	     const gslmm::vector<data_type>& w,
	     const gslmm::vector<data_type>& y)
    {
      if (x.row_size() > y.size() || x.column_size() != np) return false;
      if (!resize(x.row_size())) return false;
      gsl_vector_view yv = 
	gsl_vector_view_array(const_cast<data_type*>(y.element(0)),
			      x.row_size());
      gsl_vector_view pv = 
	gsl_vector_view_array(const_cast<data_type*>(_param.element(0)), np);
      gsl_vector_view wv = 
	gsl_vector_view_array(const_cast<data_type*>(w.element(0)), 
			      x.row_size());
      gsl_matrix_view xv = 
	gsl_matrix_view_array(const_cast<data_type*>(x.element(0,0)),
			      x.row_size(), np);
      gsl_matrix_view cv = 
	gsl_matrix_view_array(const_cast<data_type*>(_covar.element(0,0)),
			      np, np);
      int ret = gsl_multifit_wlinear(&(xv.matrix), &(wv.vector), 
				     &(yv.vector), &(pv.vector), 
				     &(cv.matrix), &_chisq, _workspace);
      _ndf = x.row_size() - np;
      return ret == GSL_SUCCESS;
    }
    /** Fit the data points @a y observed at @a x to the model 
	@param x independent variable data
	@param w Weights
	@param y dependent variable data 
	@param tolerance If a components ratio of singular values 
	@f$ s_i/s_0@f$  falls below this value, it is discarded
	@return true on success */
    bool fit(const gslmm::matrix<data_type>& x, 
	     const gslmm::vector<data_type>& w,
	     const gslmm::vector<data_type>& y, 
	     const data_type& tolerance)
    {
      if (x.row_size() > y.size() || x.column_size() != np) return false;
      if (!resize(x.row_size())) return false;
      gsl_vector_view yv = 
	gsl_vector_view_array(const_cast<data_type*>(y.element(0)),
			      x.row_size());
      gsl_vector_view pv = 
	gsl_vector_view_array(const_cast<data_type*>(_param.element(0)), np);
      gsl_vector_view wv = 
	gsl_vector_view_array(const_cast<data_type*>(w.element(0)), 
			      x.row_size());
      gsl_matrix_view xv = 
	gsl_matrix_view_array(const_cast<data_type*>(x.element(0,0)),
			      np, np);
      gsl_matrix_view cv = 
	gsl_matrix_view_array(const_cast<data_type*>(_covar.element(0,0)),
			      x.row_size(), np);
      int ret = gsl_multifit_wlinear_svd(&(xv.matrix), &(wv.vector), 
					 &(yv.vector), tolerance, &_rank, 
					 &(pv.vector), &(cv.matrix), 
					 &_chisq, this->_workspace);
      _ndf = x.row_size() - np;
      return ret == GSL_SUCCESS;
    }
    /** @return the rank of the single value decomposition, in case a
	tolerance level was supplied. */
    const size_t& rank() const { return _rank; }
  };

  //==================================================================
  /** @brief Class template to do @f$ \chi^2@f$ fits to experimental
      data using linear combinations of functions. 

      The data may be weighted or unweighted.  For weighted data the
      functions compute the best fit parameters and their associated
      covariance matrix.  For unweighted data the covariance matrix is
      estimated from the scatter of the points, giving a
      variance-covariance matrix. 

      This class can be used to perform least-squares fits to a
      straight line model without a constant term, @f$ Y = c_1 X@f$.
      For weighted data the best-fit is found by minimizing the
      weighted sum of squared residuals, @f$ \chi^2@f$, 
      @f[
      \chi^2 = \sum_i w_i (y_i - c_1 x_i)^2
      @f]
      for the parameter @f$ c_1@f$.  For unweighted data the sum is
      computed with @f$ w_i = 1@f$.
      @ingroup fit
  */
  template <>
  class linear<1> : public basic_linear 
  {
  private:
    /** Store result of fit
	@param p0   Parameter 
	@param c00  Covariance  
    */
    void store(data_type p0, data_type c00)
    {
      _param[0]   = p0;
      _covar(0,0) = c00;
    }
  public:
    /** data type */
    typedef basic_linear::data_type data_type;
    /** evaluate type  */
    typedef std::pair<data_type,data_type> return_type;
    /** ctor */
    linear() : basic_linear(1) {}
    /** dtor */
    virtual ~linear() {}
    /** Perform the fit. 
	@param x The independet variable data points
	@param y The dependent variable data points 
	@return true on success */
    bool fit(const std::vector<data_type>& x, 
	     const std::vector<data_type>& y)
    {
      if (x.size() > y.size()) return false;
      data_type c11, p1;
      int ret = gsl_fit_mul(&(x[0]), 1, &(y[0]), 1, x.size(), 
			    &p1, &c11, &_chisq);
      store(p1, c11);
      _ndf = x.size() - 2;
      return ret == GSL_SUCCESS;
    }
    /** Perform the fit. 
	@param x The independet variable data points
	@param w Weights
	@param y The dependent variable data points 
	@return true on success */
    bool fit(const std::vector<data_type>& x, 
	     const std::vector<data_type>& y, 
	     const std::vector<data_type>& w)
    {
      if (x.size() > y.size() || w.size() < y.size()) return false;
      data_type c11, p1;
      int ret = gsl_fit_wmul(&(x[0]), 1, &(y[0]), 1, &(w[0]), 1, 
			     x.size(), &p1, &c11, &_chisq);
      store(p1, c11);
      _ndf = x.size() - 2;
      return ret == GSL_SUCCESS;
    }
    /** This function uses the best-fit linear regression coefficients 
	and their estimated covariance to compute  the fitted function
	@f$ Y@f$ and its standard deviation for the model @f$ Y = c_0
	+ c_1 X@f$  at the point @f$ X@f$. 
	@param x point to evaluate the fit at
	@return A pair of values.  The first being the computed @f$
	Y@f$, the second the standard diviaton on the value */
    return_type eval(const data_type& x) 
    {
      return_type y;
      int ret = gsl_fit_mul_est(x, _param[0], _covar(0,0), 
				&(y.first), &(y.second));
      return y;
    }
  };

  //==================================================================
  /** @brief Class template to do @f$ \chi^2@f$ fits to experimental
      data using linear combinations of functions. 

      The data may be weighted or unweighted.  For weighted data the
      functions compute the best fit parameters and their associated
      covariance matrix.  For unweighted data the covariance matrix is
      estimated from the scatter of the points, giving a
      variance-covariance matrix. 

      This class can be used to perform least-squares fits to a
      straight line model, @f$ Y = c_0 + c_1 X@f$.  For weighted data
      the best-fit is found by minimizing the weighted sum of squared
      residuals, @f$ \chi^2@f$, 
      @f[
      \chi^2 = \sum_i w_i (y_i - (c_0 + c_1 x_i))^2
      @f]
      for the parameters @f$ c_0, c_1.@f$  For unweighted data the sum
      is computed with @f$ w_i = 1@f$.
      @ingroup fit
  */
  template <>
  class linear<2> : public basic_linear
  {
  private:
    /** Store the values of the fit. 
	@param p0 Parameter 0
	@param p1 Parameter 1
	@param c00 Covarience element (0,0)
	@param c01 Covarience element (0,1)
	@param c11 Covarience element (1,1)
    */
    void store(data_type p0, data_type p1, 
	       data_type c00, data_type c01, data_type c11)
    {
      _param[0]   = p0;
      _param[1]   = p1;      
      _covar(0,0) = c00;
      _covar(0,1) = _covar(1,0) = c01;
      _covar(1,1) = c11;
    }
  public:
    /** data type */
    typedef basic_linear::data_type data_type;
    /** evaluate type  */
    typedef std::pair<data_type,data_type> return_type;
    linear() : basic_linear(2) {}
    virtual ~linear() {}
    /** Perform the fit. 
	@param x The independet variable data points
	@param y The dependent variable data points 
	@return true on success */
    bool fit(const std::vector<data_type>& x, 
	     const std::vector<data_type>& y)
    {
      if (x.size() > y.size()) return false;
      data_type c00, c01, c11, p0, p1;
      int ret = gsl_fit_linear(&(x[0]), 1, &(y[0]), 1, x.size(), 
			       &p0, &p1, &c00, &c01, &c11, &_chisq);
      store(p0, p1, c00, c01, c11);
      _ndf = x.size() - 2;
      return ret == GSL_SUCCESS;
    }
    /** Perform the fit. 
	@param x The independet variable data points
	@param w Weights
	@param y The dependent variable data points 
	@return true on success */
    bool fit(const std::vector<data_type>& x, 
	     const std::vector<data_type>& y, 
	     const std::vector<data_type>& w)
    {
      if (x.size() > y.size() || w.size() < y.size()) return false;
      data_type c00, c01, c11, p0, p1;
      int ret = gsl_fit_wlinear(&(x[0]), 1, &(w[0]), 1, &(y[0]), 1, x.size(), 
				&p0, &p1, &c00, &c01, &c11, &_chisq);
      store(p0, p1, c00, c01, c11);
      _ndf        = x.size() - 2;
      return ret;
    }
    /** This function uses the best-fit linear regression coefficients 
	and their estimated covariance to compute  the fitted function
	@f$ Y@f$ and its standard deviation for the model @f$ Y = c_0
	+ c_1 X@f$  at the point @f$ X@f$. 
	@param x point to evaluate the fit at
	@return A pair of values.  The first being the computed @f$
	Y@f$, the second the standard diviaton on the value */
    return_type eval(double x) 
    {
      return_type y;
      int ret = gsl_fit_linear_est(x, _param[0], _param[1], 
				   _covar(0,0), _covar(0,1), _covar(1,1), 
				   &(y.first), &(y.second));
      return y;
    }
  };
  
  //__________________________________________________________________
}

#endif
//____________________________________________________________________
//
// EOF
//
