//
// $Id: erlang.hh,v 1.3 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::erlang
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
#ifndef GSLMM_random_erlang
#define GSLMM_random_erlang

/** @file   random/erlang.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random erlang classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class erlang random/erlang.hh <gslmm/random/erlang.hh> 
      @brief Erlang distribution interface class. 
      @ingroup dists 

      The Erlang PDF is 
      @f[
        p(x) = \exp((n - 1) \log(x/a) - x/a - log\Gamma(n)/a)
      @f]

   */
  class erlang : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** The parameter a of the distribution */
    double _a;
    /** The parameter n of the distribution */
    double _n;
  public:
    /** Constructor
	@param a The parameter @f$ a@f$ of the PDF  
	@param n Thenu number parameter of the PDF
	@param r The random number generator */
    erlang(double a, double n, generator& r) 
      : base_type(r), _a(a), _n(n)
    {}
    /** Destructor  */
    virtual ~erlang() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_erlang(rng(), _a, _n);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_erlang_pdf(x, _a, _n);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
