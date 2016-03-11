//
// $Id: exponential_power.hh,v 1.3 2007-04-21 09:03:12 cholm Exp $ 
//  
//  gslmm::exponential_power
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
#ifndef GSLMM_random_exponential_power
#define GSLMM_random_exponential_power

/** @file   random/exponential_power.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random exponential_power classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class exponential_power random/exponential_power.hh <gslmm/random/exponential_power.hh> 
      @brief Exponential power distribution interface class. 
      @ingroup dists 

      The Exponential power PDF is 
      @f[
         p(x) dx = \frac{1}{2 a \Gamma(1+1/b)} \exp(-|x/a|^b) dx
      @f]
      for @f$ x \geq 0@f$.  For @f$ b = 1@f$ this reduces to the
      gslmm::laplace distribution.  For @f$ b = 2@f$ it has the same
      form as a gaussian distribution, but with @f$ a = \sqrt{2}
      \sigma@f$.       
   */
  class exponential_power : public distribution<double>
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
	@param a The scale parameter 
	@param b the exponent
	@param r The random number generator */
    exponential_power(double a, double b, generator& r) 
      : base_type(r), _a(a), _b(b)
    {}
    /** Destructor  */
    virtual ~exponential_power() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_exppow(rng(), _a, _b);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_exppow_pdf(x, _a, _b);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
