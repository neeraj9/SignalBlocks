//
// $Id: binomial.hh,v 1.4 2007-04-21 09:03:12 cholm Exp $ 
//  
//  gslmm::binomial
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
#ifndef GSLMM_random_binomial
#define GSLMM_random_binomial

/** @file   random/binomial.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random binomial classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class binomial random/binomial.hh <gslmm/random/binomial.hh> 
      @brief Binomial distribution interface class. 
      @ingroup dists 

      This gives a random integer from the binomial distribution, the
      number of successes in @f$ n@f$ independent trials with
      probability @f$ p@f$.

      The Binomial PDF is 
      @f[
        p(k) = \frac{n!}{k! (n-k)!} p^k (1-p)^{n-k}
      @f]
      for @f$ 0 \leq k \leq n@f$

   */
  class binomial : public distribution<size_t>
  {
  protected:
    /** The base type */
    typedef distribution<size_t> base_type;
    /** Probability of the distribution */
    double _p;
    /** Parameter n of the distribution */
    size_t _n;
  public:
    /** Constructor
	@param p The probability parameter of the PDF
	@param n The number parameter of the PDF
	@param r The random number generator */
    binomial(double p, size_t n, generator& r) 
      : base_type(r), _p(p), _n(n)
    {}
    /** Destructor  */
    virtual ~binomial() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_binomial(rng(), _p, _n);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_binomial_pdf(x, _p, _n);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
