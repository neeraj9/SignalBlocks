//
// $Id: tdist.hh,v 1.4 2007-04-21 09:03:14 cholm Exp $ 
//  
//  gslmm::tdist
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
#ifndef GSLMM_random_tdist
#define GSLMM_random_tdist

/** @file   random/tdist.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random tdist classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class tdist random/tdist.hh <gslmm/random/tdist.hh> 
      @brief T distribution interface class. 
      @ingroup dists 

      The t-distribution (also called the Student's
      Distribution) arises in statistics.  If @f$ Y_1@f$ has a 
      normal distribution and @f$ Y_2@f$ has a chi-squared
      distribution with @f$ \nu@f$ degrees of freedom then the ratio, 
      @f[
        X = \frac{Y_1}{\sqrt{Y_2 / \nu}}
      @f]
      has a t-distribution @f$ t(x;\nu)@f$ with @f$\nu@f$ degrees of
      freedom. 

      The T PDF is 
      @f[
        p(x) dx = \frac{\Gamma((\nu + 1)/2)}{\sqrt{\pi \nu}
 	           \Gamma(\nu/2)} (1 + x^2/\nu)^{-(\nu + 1)/2} dx
      @f]
      for @f$-\infty < x < +\infty@f$.

   */
  class tdist : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Degrees of freedom (@f$\nu@f$) of the distribution */
    double _nu;
  public:
    /** Constructor
	@param nu The parameter @f$\nu@f$ of the PDF
	@param r The random number generator */
    tdist(double nu, generator& r) 
      : base_type(r), _nu(nu)
    {}
    /** Destructor  */
    virtual ~tdist() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_tdist(rng(), _nu);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_tdist_pdf(x, _nu);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      return gsl_cdf_tdist_P(x, _nu);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      return gsl_cdf_tdist_Q(x, _nu);
    }

    /** @see gslmm::distribution<Return,Arg>::lower_invcdf */
    virtual return_type lower_invcdf(double p)
    {
      return gsl_cdf_tdist_Pinv(p, _nu);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_invcdf */
    virtual return_type upper_invcdf(double q)
    {
      return gsl_cdf_tdist_Qinv(q, _nu);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
