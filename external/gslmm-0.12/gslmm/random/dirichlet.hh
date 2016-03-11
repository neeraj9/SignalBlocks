//
// $Id: dirichlet.hh,v 1.2 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::dirichlet
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
#ifndef GSLMM_random_dirichlet
#define GSLMM_random_dirichlet

/** @file   random/dirichlet.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random dirichlet classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class dirichlet random/dirichlet.hh <gslmm/random/dirichlet.hh> 
      @brief Dirichlet distribution interface class. 
      @ingroup dists 

      The Dirichlet PDF is 
      @f[
         p(\theta_1, \ldots, \theta_K) d\theta_1 \ldots \theta_K =
            (1/Z) \prod_{i=1}^K \theta_i^{\alpha_i - 1} 
	       \delta(1 -\sum_{i=1}^K \theta_i) d\theta_1 \ldots d\theta_K
      @f]
      for @f$\theta_i \geq 0@f$ and @f$\alpha_i \geq 0@f$.  
      The delta function ensures that @f$\sum \theta_i = 1@f$.  The
      normalization factor @f$ Z@f$ is 
      @f[
        Z = {\prod_{i=1}^K \Gamma(\alpha_i)} / 
	      {\Gamma( \sum_{i=1}^K \alpha_i)}
      @f]
      The random variates are generated by sampling @f$ K@f$ values
      from gslmm::gamma distributions with parameters 
      @f$ a=\alpha_i, b=1@f$, and renormalizing. 

   */
  class dirichlet : public distribution<std::vector<double> >
  {
  protected:
    /** The base type */
    typedef distribution<std::vector<double> > base_type;
    /** Parameter @f$\alpha@f$ of the distribution */
    return_type _alpha;
  public:
    /** Constructor
	@param alpha A vector of weights
	@param r The random number generator */
    dirichlet(const return_type& alpha, generator& r) 
      : base_type(r), _alpha(alpha)
    {}
    /** Destructor  */
    virtual ~dirichlet() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type& sample(return_type& theta) 
    {
      gsl_ran_dirichlet(rng(), _alpha.size(), &(_alpha[0]), &(theta[0]));
      return theta;
    }
    virtual return_type sample() 
    {
      return_type theta(_alpha.size());
      return sample(theta);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_dirichlet_pdf(_alpha.size(), &(_alpha[0]), &(x[0]));
    }

    /** Computes @f$\log P(x)@f$ */
    virtual double log_pdf(const return_type& x) 
    {
      return gsl_ran_dirichlet_lnpdf(_alpha.size(), &(_alpha[0]), &(x[0]));
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
