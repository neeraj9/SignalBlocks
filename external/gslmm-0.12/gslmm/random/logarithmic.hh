//
// $Id: logarithmic.hh,v 1.3 2007-04-21 09:03:13 cholm Exp $ 
//  
//  gslmm::logarithmic
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
#ifndef GSLMM_random_logarithmic
#define GSLMM_random_logarithmic

/** @file   random/logarithmic.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random logarithmic classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class logarithmic random/logarithmic.hh <gslmm/random/logarithmic.hh> 
      @brief Logarithmic distribution interface class. 
      @ingroup dists 

      The Logarithmic PDF is 
      @f[
        p(k) = \frac{-1}{\log(1-p)} {(\frac{p^k}{k})}
      @f]
      for @f$ k \geq 1@f$

   */
  class logarithmic : public distribution<size_t>
  {
  protected:
    /** The base type */
    typedef distribution<size_t> base_type;
    /** Probability of the distribution */
    double _p;
  public:
    /** Constructor
	@param p The probability parameter of the distribution
	@param r The random number generator */
    logarithmic(double p, generator& r) 
      : base_type(r), _p(p)
    {}
    /** Destructor  */
    virtual ~logarithmic() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_logarithmic(rng(), _p);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& k) 
    {
      return gsl_ran_logarithmic_pdf(k, _p);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
