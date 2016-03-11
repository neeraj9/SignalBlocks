//
// $Id: gaussian_tail.hh,v 1.3 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::gaussian_tail
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
#ifndef GSLMM_random_gaussian_tail
#define GSLMM_random_gaussian_tail

/** @file   random/gaussian_tail.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random gaussian_tail classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class gaussian_tail random/gaussian_tail.hh <gslmm/random/gaussian_tail.hh> 
      @brief Gaussian tail distribution interface class. 
      @ingroup dists 

      The Gaussian tail PDF is 
      @f[
        p(x) dx = \frac 1 {N(a\sigma)} \exp (- x^2/(2 \sigma^2)) dx
      @f]
      for @f$ x > a@f$ where @f$ N(a\sigma)@f$ is the normalization
      constant, 
      @f[
        N(a\sigma) = (1/2) erfc(a / sqrt(2 sigma^2)).
      @f]
   */
  class gaussian_tail : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Width (@f$\sigma@f$) of the distribution */
    double _sigma;
    /** Parameter a of the distribution */
    double _a;
  public:
    /** Constructor
	@param a The height of the distribution 
	@param sigma the width of the distribution
	@param r The random number generator */
    gaussian_tail(double a, double sigma, generator& r) 
      : base_type(r), _sigma(sigma), _a(a)
    {}
    /** Destructor  */
    virtual ~gaussian_tail() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      if (_sigma == 1) return gsl_ran_ugaussian_tail(rng(),_a);
      else             return gsl_ran_gaussian_tail(rng(), _a, _sigma);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      if (_sigma == 1) return gsl_ran_ugaussian_tail_pdf(x, _a);
      else             return gsl_ran_gaussian_tail_pdf(x, _a, _sigma);
    }    
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
