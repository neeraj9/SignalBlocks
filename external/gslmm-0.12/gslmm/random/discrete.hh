//
// $Id: discrete.hh,v 1.5 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::random
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
#ifndef GSLMM_random
#define GSLMM_random

/** @file   random/discrete.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random distribution classes */

#ifndef GSLMM_random
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class discrete discrete.hh <gslmm/random/distribution.hh> 
      @brief Random number distribution interface class. 
      @ingroup dists 

      This contains a generic discrete random distribution.  The user
      should load in an array of probabilities for drawing an index
      number.  The probabilities needn't be normalised.  For example, 
      we could set up an object to simulate the sum of a two-dice
      roll: 
      @code 
      std::vector<double> p;
      p.push_back(1);  // How to get  2: 1+1
      p.push_back(2);  // How to get  3: 1+2, 2+1
      p.push_back(3);  // How to get  4: 1+3, 2+2, 3+1
      p.push_back(4);  // How to get  5: 1+4, 2+3, 3+2, 4+1
      p.push_back(5);  // How to get  6: 1+5, 2+4, 3+3, 4+2, 5+1
      p.push_back(6);  // How to get  7: 1+6, 2+5, 3+4, 4+3, 5+2, 6+1
      p.push_back(5);  // How to get  8: 2+6, 3+5, 4+4, 5+3, 6+2
      p.push_back(4);  // How to get  9: 3+6, 4+5, 5+4, 6+3
      p.push_back(3);  // How to get 10: 4+6, 5+5, 6+4
      p.push_back(2);  // How to get 11: 5+6, 6+5
      p.push_back(1);  // How to get 12: 6+6
      
      gslmm::discrete d(p);
      
      // Throw the dice. The `+ 2' is because the index starts at 0
      size_t result = d() + 2;
      @endcode 
   */
  class discrete : public distribution<size_t>
  {
  protected:
    /** The base type */
    typedef distribution<size_t> base_type;
    /** The low-level object */
    gsl_ran_discrete_t* _table;
  public:
    /** Constructor
	@param r The random number generator 
	@param k the number of elements in @a p
	@param p Array of @a k probabilities.  Element @c p[i] should
	give the (unnormalised) probability of getting @c i when
	sampling. */
    discrete(size_t k, const double* p, generator& r) 
      : base_type(r)
    {
      _table = gsl_ran_discrete_preproc(k, p);
    }
    /** Constructor
	@param r The random number generator 
	@param p Array of probabilities.  Element @c p[i] should
	give the (unnormalised) probability of getting @c i when 
	sampling. */ 
    discrete(const std::vector<double>& p, generator& r)
      : base_type(r)
    {
      _table = gsl_ran_discrete_preproc(p.size(), &(p[0]));
    }
    /** Destructor  */
    virtual ~discrete() { gsl_ran_discrete_free(_table); }
    /** Sample the distribution. 
	@return The sample. */
    return_type sample() 
    {
      return gsl_ran_discrete(rng(), _table);
    }
    /** Evaluate the Probability Density Function (PDF) at @a x 
	@param x The place to evaluate the PDF. 
	@return @f$ p(x)@f$ */
    double pdf(const return_type& x) 
    {
      return gsl_ran_discrete_pdf(x, _table);
    }    
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
