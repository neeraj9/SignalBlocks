//
// $Id: pascal.hh,v 1.4 2007-04-21 09:03:13 cholm Exp $ 
//  
//  gslmm::pascal
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
#ifndef GSLMM_random_pascal
#define GSLMM_random_pascal

/** @file   random/pascal.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random pascal classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class pascal random/pascal.hh <gslmm/random/pascal.hh> 
      @brief Pascal distribution interface class. 
      @ingroup dists 

      This is the same as gslmm::negative_binomal, except @f$ n@f$ is
      an integer.

      The Pascal PDF is 
      @f[
        p(k) = \frac{(n + k - 1)!}{k! (n - 1)!} p^n (1-p)^k
      @f]
      for @f$ k \geq 0@f$

   */
  class pascal : public distribution<size_t>
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
	@param p The probability parameter 
	@param n The number parameter 
	@param r The random number generator */
    pascal(double p, size_t n, generator& r) 
      : base_type(r), _p(p), _n(n)
    {}
    /** Destructor  */
    virtual ~pascal() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_pascal(rng(), _p, _n);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& k) 
    {
      return gsl_ran_pascal_pdf(k, _p, _n);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
