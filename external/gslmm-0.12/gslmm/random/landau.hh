//
// $Id: landau.hh,v 1.2 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::landau
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
#ifndef GSLMM_random_landau
#define GSLMM_random_landau

/** @file   random/landau.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random landau classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class landau random/landau.hh <gslmm/random/landau.hh> 
      @brief Landau distribution interface class. 
      @ingroup dists 

      The Landau PDF is defined analytically by the complex integral,
      @f[
        p(x) = (1/(2 \pi i)) \int_{c-i\infty}^{c+i\infty} ds 
	   \exp(s log(s) + x s)
      @f]
      For numerical purposes it is more convenient to use the
      following equivalent form of the integral,
      @f[
          p(x) = (1/\pi) \int_0^\infty dt \exp(-t \log(t) - x t) \sin(\pi t).
      @f]

   */
  class landau : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
  public:
    /** Constructor
	@param r The random number generator */
    landau(generator& r) 
      : base_type(r)
    {}
    /** Destructor  */
    virtual ~landau() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_landau(rng());
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_landau_pdf(x);
    }    
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
