//
// $Id: gamma.hh,v 1.3 2007-04-21 09:03:13 cholm Exp $ 
//  
//  gslmm::gamma
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
#ifndef GSLMM_random_gamma
#define GSLMM_random_gamma

/** @file   random/gamma.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random gamma classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class gamma random/gamma.hh <gslmm/random/gamma.hh> 
      @brief @f$ \Gamma@f$ distribution interface class. 
      @ingroup dists 

      The @f$ \Gamma@f$ PDF is 
      @f[
        p(x) dx = \frac{1}{\Gamma(a) b^a} x^{a-1} e^{-x/b} dx
      @f]
      for @f$ x > 0@f$.

   */
  class gamma : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Parameter a of the distribution */
    double _a;
    /** Parameter b of the distribution */
    double _b;
  public:
    /** Constructor
	@param a 
	@param b
	@param r The random number generator */
    gamma(double a, double b, generator& r) 
      : base_type(r), _a(a), _b(b)
    {}
    /** Destructor  */
    virtual ~gamma() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_gamma(rng(), _a, _b);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_gamma_pdf(x, _a, _b);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      return gsl_cdf_gamma_P(x, _a, _b);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      return gsl_cdf_gamma_Q(x, _a, _b);
    }

    /** @see gslmm::distribution<Return,Arg>::lower_invcdf */
    virtual return_type lower_invcdf(double p)
    {
      return gsl_cdf_gamma_Pinv(p, _a, _b);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_invcdf */
    virtual return_type upper_invcdf(double q)
    {
      return gsl_cdf_gamma_Qinv(q, _a, _b);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
