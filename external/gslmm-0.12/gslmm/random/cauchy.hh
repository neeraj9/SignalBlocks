//
// $Id: cauchy.hh,v 1.3 2007-04-21 09:03:12 cholm Exp $ 
//  
//  gslmm::cauchy
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
#ifndef GSLMM_random_cauchy
#define GSLMM_random_cauchy

/** @file   random/cauchy.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random cauchy classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class cauchy random/cauchy.hh <gslmm/random/cauchy.hh> 
      @brief Cauchy distribution interface class. 
      @ingroup dists 

      The Cauchy PDF is 
      @f[
        p(x) dx = \frac{1}{a\pi (1 + (x/a)^2)} dx
      @f]
      for @f$ x@f$ in the range @f$-\infty@f$ to @f$+\infty@f$.  The
      Cauchy distribution is also known as the Lorentz distribution
   */
  class cauchy : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Parameter a of the distribution */
    double _a;
  public:
    /** Constructor
	@param a The scale parameter 
	@param r The random number generator */
    cauchy(double a, generator& r) 
      : base_type(r), _a(a)
    {}
    /** Destructor  */
    virtual ~cauchy() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_cauchy(rng(), _a);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_cauchy_pdf(x, _a);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      return gsl_cdf_cauchy_P(x, _a);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      return gsl_cdf_cauchy_Q(x, _a);
    }

    /** @see gslmm::distribution<Return,Arg>::lower_invcdf */
    virtual return_type lower_invcdf(double p)
    {
      return gsl_cdf_cauchy_Pinv(p, _a);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_invcdf */
    virtual return_type upper_invcdf(double q)
    {
      return gsl_cdf_cauchy_Qinv(q, _a);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
