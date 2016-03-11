//
// $Id: poisson.hh,v 1.3 2007-04-21 09:03:13 cholm Exp $ 
//  
//  gslmm::poisson
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
#ifndef GSLMM_random_poisson
#define GSLMM_random_poisson

/** @file   random/poisson.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random poisson classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class poisson random/poisson.hh <gslmm/random/poisson.hh> 
      @brief Poisson distribution interface class. 
      @ingroup dists 

      The Poisson PDF is 
      @f[
        p(k) = \frac{\mu^k}{k!} \exp(-\mu)
      @f]
      for @f$ k \geq 0@f$

   */
  class poisson : public distribution<size_t>
  {
  protected:
    /** The base type */
    typedef distribution<size_t> base_type;
    /** The mean (@f$\mu@f$) of the distribution */
    double _mu;
  public:
    /** Constructor
	@param mu The mean of the distribution
	@param r The random number generator */
    poisson(double mu, generator& r) 
      : base_type(r), _mu(mu)
    {}
    /** Destructor  */
    virtual ~poisson() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_poisson(rng(), _mu);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_poisson_pdf(x, _mu);
    }    
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
