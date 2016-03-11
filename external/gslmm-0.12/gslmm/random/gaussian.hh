//
// $Id: gaussian.hh,v 1.3 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::gaussian
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
#ifndef GSLMM_random_gaussian
#define GSLMM_random_gaussian

/** @file   random/gaussian.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random gaussian classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class gaussian random/gaussian.hh <gslmm/random/gaussian.hh> 
      @brief gaussian distribution interface class. 
      @ingroup dists 

      The gaussian PDF is 
      @f[
      p(x)dx = \frac{1}{\sqrt{2 \pi \sigma^2}} \exp (-x^2/2\sigma^2) dx
      @f]

   */
  class gaussian : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Width (@f$\sigma@f$) of the distribution */
    double _sigma;
    /** Parameter ratio of the distribution */
    bool _ratio;
  public:
    /** Constructor
	@param sigma The @f$ \sigma@f$ of the PDF.
	@param ratio if true use the Kinderman-Monahan ratio method.
	@param r The random number generator */
    gaussian(double sigma, generator& r, bool ratio=false) 
      : base_type(r), _sigma(sigma), _ratio(ratio)
    {}
    /** Destructor  */
    virtual ~gaussian() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      if (_ratio) 
	if (_sigma == 1)
	  return gsl_ran_ugaussian_ratio_method(rng());
	else 
	  return gsl_ran_gaussian_ratio_method(rng(), _sigma);
      else 
	if (_sigma == 1) 
	  return gsl_ran_ugaussian(rng());
      return gsl_ran_gaussian(rng(), _sigma);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      if (_sigma == 1) gsl_ran_ugaussian_pdf(x);
      return gsl_ran_gaussian_pdf(x, _sigma);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      if (_sigma == 1) return gsl_cdf_ugaussian_P(x);
      return gsl_cdf_gaussian_P(x, _sigma);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      if (_sigma == 1) return gsl_cdf_ugaussian_Q(x);
      return gsl_cdf_gaussian_Q(x, _sigma);
    }

    /** @see gslmm::distribution<Return,Arg>::lower_invcdf */
    virtual return_type lower_invcdf(double p)
    {
      if (_sigma == 1) return gsl_cdf_ugaussian_Pinv(p);
      return gsl_cdf_gaussian_Pinv(p, _sigma);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_invcdf */
    virtual return_type upper_invcdf(double q)
    {
      if (_sigma == 1) return gsl_cdf_ugaussian_Qinv(q);
      return gsl_cdf_gaussian_Qinv(q, _sigma);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
