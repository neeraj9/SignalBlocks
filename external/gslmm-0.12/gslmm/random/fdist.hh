//
// $Id: fdist.hh,v 1.3 2007-04-21 09:03:13 cholm Exp $ 
//  
//  gslmm::fdist
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
#ifndef GSLMM_random_fdist
#define GSLMM_random_fdist

/** @file   random/fdist.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random fdist classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class fdist random/fdist.hh <gslmm/random/fdist.hh> 
      @brief F distribution interface class. 
      @ingroup dists 

      The F-distribution (Also called the Fisher-Snedecor
      Distribution) arises in statistics.  If @f$ Y_1@f$ and 
      @f$ Y_2@f$ are @f$\chi^2@f$ deviates with @f$\nu_1@f$ and
      @f$\nu_2@f$ degrees of freedom then the ratio,
      @f[
        X = \frac{(Y_1 / \nu_1)}{(Y_2 / \nu_2)}
      @f]
      has an F-distribution @f$ F(x;\nu_1,\nu_2)@f$.

      The F PDF is 
      @f[
           p(x) dx =
             \frac{\Gamma((\nu_1 + \nu_2)/2)}
	     {\Gamma(\nu_1/2) \Gamma(\nu_2/2)}
             \nu_1^{\nu_1/2} \nu_2^{\nu_2/2}
             x^{\nu_1/2 - 1} (\nu_2 + \nu_1 x)^{-\nu_1/2 -\nu_2/2}
      @f]
      for @f$ x \geq 0@f$.

      @see gslmm::gamma
      @see gslmm::chisq
   */
  class fdist : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Degrees of freedom (@f$\nu_1@f$) of the distribution */
    double _nu1;
    /** Degrees of freedom (@f$\nu_2@f$) of the distribution */
    double _nu2;
  public:
    /** Constructor
	@param nu1 The number degrees of freedom @f$\nu_1@f$
	@param nu2 The number degrees of freedom @f$\nu_2@f$
	@param r The random number generator */
    fdist(double nu1, double nu2, generator& r) 
      : base_type(r), _nu1(nu1), _nu2(nu2)
    {}
    /** Destructor  */
    virtual ~fdist() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_fdist(rng(), _nu1, _nu2);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_fdist_pdf(x, _nu1, _nu2);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      return gsl_cdf_fdist_P(x, _nu1, _nu2);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      return gsl_cdf_fdist_Q(x, _nu1, _nu2);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
