//
// $Id: negative_binomial.hh,v 1.4 2007-04-21 09:03:13 cholm Exp $ 
//  
//  gslmm::nagtive_binomial
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
#ifndef GSLMM_random_negative_binomial
#define GSLMM_random_negative_binomial

/** @file   random/negative_binomial.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random negative_binomial classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class negative_binomial random/negative_binomial.hh <gslmm/random/negative_binomial.hh> 
      @brief Negative binomial distribution interface class. 
      @ingroup dists 

      This gives a random integer from the negative binomial
      distribution, the number of failures occurring before @f$ n@f$
      successes in independent trials with probability @f$ p@f$ of
      success. 

      The Negative binomial PDF is 
      @f[
        p(k) = \frac{\Gamma(n + k)}{\Gamma(k+1) \Gamma(n)} p^n (1-p)^k
      @f]

   */
  class negative_binomial : public distribution<size_t>
  {
  protected:
    /** The base type */
    typedef distribution<size_t> base_type;
    /** Probability of the distribution */
    double _p;
    /** Parameter n of the distribution */
    double _n;
  public:
    /** Constructor
	@param p The probability parameter 
	@param n The number parameter 
	@param r The random number generator */
    negative_binomial(double p, double n, generator& r) 
      : base_type(r), _p(p), _n(n)
    {}
    /** Destructor  */
    virtual ~negative_binomial() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_negative_binomial(rng(), _p, _n);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& k) 
    {
      return gsl_ran_negative_binomial_pdf(k, _p, _n);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
