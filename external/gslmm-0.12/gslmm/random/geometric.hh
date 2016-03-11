//
// $Id: geometric.hh,v 1.3 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::geometric
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
#ifndef GSLMM_random_geometric
#define GSLMM_random_geometric

/** @file   random/geometric.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random geometric classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class geometric random/geometric.hh <gslmm/random/geometric.hh> 
      @brief Geometric distribution interface class. 
      @ingroup dists 

      This gives a random integer from the geometric distribution, the
      number of independent trials with probability @f$ p@f$ until the
      first success.

      The Geometric PDF is 
      @f[
        p(k) =  p (1-p)^{k-1}
      @f]
      for @f$ k \geq@f$

   */
  class geometric : public distribution<size_t>
  {
  protected:
    /** The base type */
    typedef distribution<size_t> base_type;
    /** Probability of the distribution */
    double _p;
  public:
    /** Constructor
	@param p The probability parameter of the PDF
	@param r The random number generator */
    geometric(double p, generator& r) 
      : base_type(r), _p(p)
    {}
    /** Destructor  */
    virtual ~geometric() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_geometric(rng(), _p);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_geometric_pdf(x, _p);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
