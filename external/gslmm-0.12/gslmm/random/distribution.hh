//
// $Id: distribution.hh,v 1.4 2006-05-09 07:40:34 cholm Exp $ 
//  
//  gslmm::random
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
#ifndef GSLMM_random_distribution
#define GSLMM_random_distribution

/** @file   random/distribution.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random distribution classes */

#ifndef GSLMM_random
# include <gslmm/random/generator.hh>
#endif
#ifndef __GSL_RANDIST_H__
# include <gsl/gsl_randist.h>
#endif
#ifndef __GSL_CDF_H__
# include <gsl/gsl_cdf.h>
#endif

namespace gslmm 
{
  /** @defgroup dists Distributions 
      @ingroup random 
  */
  /** @class distribution random/distribution.hh <gslmm/random/distribution.hh> 
      @brief Random number distribution interface class. 
      @ingroup dists
   */
  template <typename Return>
  class distribution
  {
  public:
    /** The type returned on sampling */
    typedef Return return_type;
    
    /** Constructor
	@param r The random number generator */
    distribution(generator& r) 
      : _generator(r)
    {}
    /** Destructor  */
    virtual ~distribution() {}
    
    /** Sample the distribution. 
	@return The sample. */
    return_type operator()()  
    {
      return sample();
    }
    /** Sample the distribution. 
	@param v Place result in this variable
	@return The sample. */
    return_type operator()(return_type& v)  
    {
      return sample(v);
    }
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() = 0;
    /** Sample the distribution. 
	@param v Place result in this variable
	@return The sample. */
    virtual return_type& sample(return_type& v) { return v = sample(); };

    /** Evaluate the Probability Density Function (PDF) at @a x 
	@return @f$ p(x)@f$ */
    virtual double pdf(const return_type&) = 0;
    
    /** Evaluate the @e lower @e cumulative @e distribution @e
	function at @f$ x@f$ 
	@warning If the derived class does not overload this member
	function, the error handler will be invoked.  
	@return @f$ P(x) = \int_{-\infty}^{x} dx' p(x')@f$ */
    virtual double lower_cdf(const return_type&) 
    {
      GSL_ERROR_VAL("lower_cdf not defined", GSL_EUNIMPL, 0);
    }
    /** Evaluate the @e upper @e cumulative @e distribution @e
	function at @f$ x@f$ 
	@warning If the derived class does not overload this member
	function, the error handler will be invoked.  
	@return @f$ Q(x) = \int_x^{\infty} dx' p(x')@f$ */
    virtual double upper_cdf(const return_type&)
    {
      GSL_ERROR_VAL("lower_cdf not defined", GSL_EUNIMPL, 0);
    }

    /** Evaluate the inverse of @e lower @e cumulative @e distribution
	@e function at @f$ P@f$ 
	@warning If the derived class does not overload this member
	function, the error handler will be invoked.  
	@return @f$ P^{-1}(P)@f$ */
    virtual return_type lower_invcdf(double)
    {
      GSL_ERROR_VAL("lower_cdf not defined", GSL_EUNIMPL, return_type());
    }
    /** Evaluate the inverse of @e upper @e cumulative @e distribution
	@e function at @f$ P@f$ 
	@warning If the derived class does not overload this member
	function, the error handler will be invoked.  
	@return @f$ Q^{-1}(Q)@f$ */
    virtual return_type upper_invcdf(double)
    {
      GSL_ERROR_VAL("lower_cdf not defined", GSL_EUNIMPL, return_type());
    }
  protected:
    /** The low-level object */
    generator& _generator;
    /** Get the low-level internal object of the generator */
    gsl_rng* rng() 
    {
      return _generator._rng;
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
