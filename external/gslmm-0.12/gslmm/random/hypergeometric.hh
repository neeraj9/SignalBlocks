//
// $Id: hypergeometric.hh,v 1.2 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::hypergeometric
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
#ifndef GSLMM_random_hypergeometric
#define GSLMM_random_hypergeometric

/** @file   random/hypergeometric.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random hypergeometric classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class hypergeometric random/hypergeometric.hh <gslmm/random/hypergeometric.hh> 
      @brief Hypergeometric distribution interface class. 
      @ingroup dists 

      This gives a random integer from the hypergeometric distribution.
      
      The Hypergeometric PDF is 
      @f[
        p(k) =  C(n_1,k) C(n_2, t-k) / C(n_1 + n_2,k)
      @f]
      where @f$ C(a,b) = a!/(b!(a-b)!)@f$.  The domain of k is
      @f$ max(0,t-n_2), \ldots, max(t,n_1)@f$.
   */
  class hypergeometric : public distribution<size_t>
  {
  protected:
    /** The base type */
    typedef distribution<size_t> base_type;
    size_t _n1;
    size_t _n2;
    /** Parameter t of the distribution */
    size_t _t;
  public:
    /** Constructor
	@param n1
	@param n2
	@param t
	@param r The random number generator */
    hypergeometric(size_t n1, size_t n2, size_t t, generator& r) 
      : base_type(r), _n1(n1), _n2(n2), _t(t)
    {}
    /** Destructor  */
    virtual ~hypergeometric() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_hypergeometric(rng(), _n1, _n2, _t);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& k) 
    {
      return gsl_ran_hypergeometric_pdf(k, _n1, _n2, _t);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
