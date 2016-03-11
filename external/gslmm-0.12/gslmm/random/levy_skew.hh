//
// $Id: levy_skew.hh,v 1.4 2007-04-21 09:03:13 cholm Exp $ 
//  
//  gslmm::levy_skew
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
#ifndef GSLMM_random_levy_skew
#define GSLMM_random_levy_skew

/** @file   random/levy_skew.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random levy_skew classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class levy_skew random/levy_skew.hh <gslmm/random/levy_skew.hh> 
      @brief Levy skew distribution interface class. 
      @ingroup dists 

      The Levy skew PDF is defined by a fourier transform
      @f[
         p(x) = \frac{1}{2 \pi} \int_{-\infty}^{+\infty} dt 
	   \exp(-it x - |c t|^\alpha (1-i \beta \mbox{sign}(t) 
	   \tan(\pi \alpha/2)))
      @f]

      When @f$\alpha = 1@f$ the term @f$\tan(\pi \alpha/2)@f$ is
      replaced by @f$-(2/\pi)\log|t|@f$. 

      There is no explicit solution for the form of @f$ p(x)@f$ and
      the library does only define a corresponding `pdf' function for
      @f$ \alpha = 1@f$ or @f$ 2@f$.  For @f$ \alpha = 1 @f$
      the distribution reduces to the gslmm::cauchy distribution.  For
      @f$ \alpha = 2@f$ it is a glsmm::gaussian distribution with
      @f$ \sigma = \sqrt{2} c@f$.  For  @f$\alpha < 1@f$ the tails of
      the distribution become extremely wide.  The symmetric
      distribution corresponds to @f$\beta = 0@f$.

      The algorithm only works for @f$ 0 < \alpha <= 2@f$.

      The Levy alpha-stable distributions have the property that if
      @f$ N @f$ alpha-stable variates are drawn from the distribution
      @f$ p(c, \alpha, \beta)@f$ then the sum @f$ Y = X_1 + X_2 +
      \dots + X_N@f$ will also be distributed as an alpha-stable
      variate, @f$ p(N^{1/\alpha} c, \alpha, \beta)@f$
   */
  class levy_skew : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Parameter c of the distribution */
    double _c;
    /** Parameter @f$\alpha@f$ of the distribution */
    double _alpha;
    /** Parameter @f$\beta@f$ (skewness) of the distribution */
    double _beta;
  public:
    /** Constructor
	@param c 
	@param alpha The parameter @f$\alpha@f$ of the distribution
	@param beta Skewness
	@param r The random number generator */
    levy_skew(double c, double alpha, double beta, 
	      generator& r) 
      : base_type(r), _c(c), _alpha(alpha), _beta(beta)
    {}
    /** Destructor  */
    virtual ~levy_skew() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return gsl_ran_levy_skew(rng(), _c, _alpha, _beta);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      if      (_alpha == 1) return gsl_ran_cauchy_pdf(x, _c);
      else if (_alpha == 2) return gsl_ran_gaussian_pdf(x, ::sqrt(2.) * _c);
      GSL_ERROR_VAL("pdf not defined for _alpha != 1 or 2", GSL_EUNIMPL, 0);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
