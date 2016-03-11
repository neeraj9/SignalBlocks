//
// $Id: chisq.hh,v 1.3 2007-04-21 09:03:12 cholm Exp $ 
//  
//  gslmm::chisq
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
#ifndef GSLMM_random_chisq
#define GSLMM_random_chisq

/** @file   random/chisq.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random chisq classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class chisq random/chisq.hh <gslmm/random/chisq.hh> 
      @brief @f$\chi^2@f$ distribution interface class. 
      @ingroup dists 


      The @f$ \chi^2@f$ distribution arises in statistics. If @f$
      Y_i@f$ are @f$ \nu@f$ independent gslmm::gaussian random
      variates with unit variance then the sum-of-squares,
      @f[
        X_i = \sum_i Y_i^2
      @f]
      has a chi-squared distribution with @f$ \nu@f$ degrees of
      freedom. 

      The @f$\chi^2@f$ PDF is
      @f[
        p(x) dx = \frac{1}{2 \Gamma(\nu/2)} (x/2)^{\nu/2 - 1} \exp(-x/2) dx
      @f]
      for @f$ x >= 0@f$.
      @see gslmm::gamma

   */
  class chisq : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Degrees of freedom (@f$\nu@f$) of the distribution */
    double _nu;
  public:
    /** Constructor
	@param nu Number of degrees of freedom
	@param r The random number generator */
    chisq(double nu, generator& r) 
      : base_type(r), _nu(nu)
    {}
    /** Destructor  */
    virtual ~chisq() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_chisq(rng(), _nu);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_chisq_pdf(x, _nu);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      return gsl_cdf_chisq_P(x, _nu);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      return gsl_cdf_chisq_Q(x, _nu);
    }

    /** @see gslmm::distribution<Return,Arg>::lower_invcdf */
    virtual return_type lower_invcdf(double p)
    {
      return gsl_cdf_chisq_Pinv(p, _nu);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_invcdf */
    virtual return_type upper_invcdf(double q)
    {
      return gsl_cdf_chisq_Qinv(q, _nu);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
