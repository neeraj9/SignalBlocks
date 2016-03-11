//
// $Id: multinomial.hh,v 1.4 2008-02-13 15:50:59 cholm Exp $ 
//  
//  gslmm::multinomial
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
#ifndef GSLMM_random_multinomial
#define GSLMM_random_multinomial

/** @file   random/multinomial.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random multinomial classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class multinomial random/multinomial.hh <gslmm/random/multinomial.hh> 
      @brief Multinomial distribution interface class. 
      @ingroup dists 

      This gives @f$ K@f$ random variates from a multinomial
      distribution. The Multinomial PDF is 
      @f[
         P(n_1, n_2, ..., n_K) =
            (N!/(n_1! n_2! ... n_K!)) p_1^{n_1} p_2^{n_2} ... p_K^{n_K}
      @f]
      where @f$ (n_1, n_2, ..., n_K)@f$ are nonnegative integers with  
      @f$ \sum_{k=1}^K n_k = N@f$, and @f$(p_1, p_2, ..., p_K)@f$ is a
      probability distribution with @f$\sum p_i = 1@f$.  If the array
      @a p is not normalized then its entries will be treated as 
      weights and normalized appropriately.

      Random variates are generated using the conditional binomial
      method.
  */
  class multinomial : public distribution<std::vector<unsigned int> >
  {
  protected:
    /** The base type */
    typedef distribution<std::vector<unsigned int> > base_type;
    std::vector<double> _p;
    /** Parameter n of the distribution */
    unsigned int _n;
  public:
    /** Constructor
	@param p Probabilities. @f$ k@f$ is the size of @a p.
	@param n Sum
	@param r The random number generator */
    multinomial(const std::vector<double>& p, unsigned int n, 
		generator& r) 
      : base_type(r), _p(p), _n(n)
    {}
    /** Destructor  */
    virtual ~multinomial() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type& sample(return_type& n) 
    {
      gsl_ran_multinomial(rng(), _p.size(), _n, &(_p[0]), &(n[0]));
      return n;
    }
    /** Sample the distribution. 
	@return the sample */
    virtual return_type sample() 
    {
      return_type n(_p.size());
      return sample(n);
    }
    
	
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& n) 
    {
      return gsl_ran_multinomial_pdf(_p.size(), &(_p[0]), &(n[0]));
    }
    /** Computes @f$ \log P(\mathbf{n})@f$ */
    virtual double log_pdf(const return_type& n) 
    {
      return gsl_ran_multinomial_pdf(_p.size(), &(_p[0]), &(n[0]));
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
