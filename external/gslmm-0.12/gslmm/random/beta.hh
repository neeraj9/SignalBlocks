//
// $Id: beta.hh,v 1.4 2007-04-21 09:03:12 cholm Exp $ 
//  
//  gslmm::beta
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
#ifndef GSLMM_random_beta
#define GSLMM_random_beta

/** @file   random/beta.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random beta classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class beta random/beta.hh <gslmm/random/beta.hh> 
      @brief @f$\beta@f$ distribution interface class. 
      @ingroup dists 

      The @f$\beta@f$ PDF is 
      @f[
      p(x) dx = \frac{\Gamma(a+b)}{\Gamma(a) \Gamma(b)} x^{a-1} (1-x)^{b-1} dx
      @f]
      for @f$ 0 \leq x \leq 1@f$.
   */
  class beta : public distribution<double>
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
	@param a The parameter @f$ a@f$ of the PDF
	@param b The parameter @f$ b@f$ of the PDF
	@param r The random number generator */
    beta(double a, double b, generator& r) 
      : base_type(r), _a(a), _b(b)
    {}
    /** Destructor  */
    virtual ~beta() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_beta(rng(), _a, _b);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_beta_pdf(x, _a, _b);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      return gsl_cdf_beta_P(x, _a, _b);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      return gsl_cdf_beta_Q(x, _a, _b);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
