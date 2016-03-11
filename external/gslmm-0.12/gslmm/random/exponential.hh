//
// $Id: exponential.hh,v 1.2 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::exponential
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
#ifndef GSLMM_random_exponential
#define GSLMM_random_exponential

/** @file   random/exponential.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random exponential classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class exponential random/exponential.hh <gslmm/random/exponential.hh> 
      @brief Exponential distribution interface class. 
      @ingroup dists 

      The Exponential PDF is 
      @f[
        p(x) dx = {\frac 1 \mu} \exp(-x/\mu) dx
      @f]
      for @f$ x \geq 0@f$. 
   */
  class exponential : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** The mean (@f$\mu@f$) of the distribution */
    double _mu;
  public:
    /** Constructor
	@param mu The inverse slope of the distribution
	@param r The random number generator */
    exponential(double mu, generator& r) 
      : base_type(r), _mu(mu)
    {}
    /** Destructor  */
    virtual ~exponential() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_exponential(rng(), _mu);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_exponential_pdf(x, _mu);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      return gsl_cdf_exponential_P(x, _mu);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      return gsl_cdf_exponential_Q(x, _mu);
    }

    /** @see gslmm::distribution<Return,Arg>::lower_invcdf */
    virtual return_type lower_invcdf(double p)
    {
      return gsl_cdf_exponential_Pinv(p, _mu);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_invcdf */
    virtual return_type upper_invcdf(double q)
    {
      return gsl_cdf_exponential_Qinv(q, _mu);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
