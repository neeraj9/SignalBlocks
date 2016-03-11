//
// $Id: rayleigh_tail.hh,v 1.3 2007-04-21 09:03:14 cholm Exp $ 
//  
//  gslmm::rayleigh_tail
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
#ifndef GSLMM_random_rayleigh_tail
#define GSLMM_random_rayleigh_tail

/** @file   random/rayleigh_tail.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random rayleigh_tail classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class rayleigh_tail random/rayleigh_tail.hh <gslmm/random/rayleigh_tail.hh> 
      @brief Rayleigh Tail distribution interface class. 
      @ingroup dists 

      The Rayleigh Tail PDF is 
      @f[
        p(x) dx = \frac{x}{\sigma^2} \exp ((a^2 - x^2) /(2 \sigma^2)) dx
      @f]
      for @f$ x > a@f$.
   */
  class rayleigh_tail : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Parameter a of the distribution */
    double _a;
    /** Width (@f$\sigma@f$) of the distribution */
    double _sigma;
  public:
    /** Constructor
	@param a The lower limit 
	@param sigma the scale parameter 
	@param r The random number generator */
    rayleigh_tail(double a, double sigma, generator& r) 
      : base_type(r), _a(a), _sigma(sigma)
    {}
    /** Destructor  */
    virtual ~rayleigh_tail() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_rayleigh_tail(rng(), _a, _sigma);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_rayleigh_tail_pdf(x, _a, _sigma);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
