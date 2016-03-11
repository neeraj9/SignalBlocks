//
// $Id: pareto.hh,v 1.2 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::pareto
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
#ifndef GSLMM_random_pareto
#define GSLMM_random_pareto

/** @file   random/pareto.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random pareto classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class pareto random/pareto.hh <gslmm/random/pareto.hh> 
      @brief Pareto distribution interface class. 
      @ingroup dists 

      The Pareto PDF is 
      @f[
         p(x) dx = (a/b) / (x/b)^{a+1} dx
      @f]
      for @f$ x \geq b@f$

   */
  class pareto : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Parameter a of the distribution */
    double _a;
    /** Parameter b of the distribution */
    double _b;
  public:
    /** Constructor
	@param a
	@param b
	@param r The random number generator */
    pareto(double a, double b, generator& r) 
      : base_type(r), _a(a), _b(b)
    {}
    /** Destructor  */
    virtual ~pareto() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_pareto(rng(), _a, _b);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_pareto_pdf(x, _a, _b);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      return gsl_cdf_pareto_P(x, _a, _b);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      return gsl_cdf_pareto_Q(x, _a, _b);
    }

    /** @see gslmm::distribution<Return,Arg>::lower_invcdf */
    virtual return_type lower_invcdf(double p)
    {
      return gsl_cdf_pareto_Pinv(p, _a, _b);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_invcdf */
    virtual return_type upper_invcdf(double q)
    {
      return gsl_cdf_pareto_Qinv(q, _a, _b);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
