//
// $Id: lognormal.hh,v 1.3 2007-04-21 09:03:13 cholm Exp $ 
//  
//  gslmm::lognormal
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
#ifndef GSLMM_random_lognormal
#define GSLMM_random_lognormal

/** @file   random/lognormal.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random lognormal classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class lognormal random/lognormal.hh <gslmm/random/lognormal.hh> 
      @brief Log normal distribution interface class. 
      @ingroup dists 

      The Log normal PDF is 
      @f[
        p(x) dx = \frac{1}{x \sqrt{2 \pi \sigma^2}} 
	  \exp(-(\ln(x) - \zeta)^2/2 \sigma^2) dx
      @f]
      for @f$ x > 0@f$.

   */
  class lognormal : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Parameter zeta of the distribution */
    double _zeta;
    /** Width (@f$\sigma@f$) of the distribution */
    double _sigma;
  public:
    /** Constructor
	@param zeta
	@param sigma
	@param r The random number generator */
    lognormal(double zeta, double sigma, generator& r) 
      : base_type(r), _zeta(zeta), _sigma(sigma)
    {}
    /** Destructor  */
    virtual ~lognormal() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_lognormal(rng(), _zeta, _sigma);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_lognormal_pdf(x, _zeta, _sigma);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      return gsl_cdf_lognormal_P(x, _zeta, _sigma);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      return gsl_cdf_lognormal_Q(x, _zeta, _sigma);
    }

    /** @see gslmm::distribution<Return,Arg>::lower_invcdf */
    virtual return_type lower_invcdf(double p)
    {
      return gsl_cdf_lognormal_Pinv(p, _zeta, _sigma);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_invcdf */
    virtual return_type upper_invcdf(double q)
    {
      return gsl_cdf_lognormal_Qinv(q, _zeta, _sigma);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
