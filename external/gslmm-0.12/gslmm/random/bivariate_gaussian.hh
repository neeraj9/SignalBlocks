//
// $Id: bivariate_gaussian.hh,v 1.3 2007-04-21 09:03:12 cholm Exp $ 
//  
//  gslmm::bivariate_gaussian
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
#ifndef GSLMM_random_bivariate_gaussian
#define GSLMM_random_bivariate_gaussian

/** @file   random/bivariate_gaussian.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random bivariate_gaussian classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class bivariate_gaussian random/bivariate_gaussian.hh <gslmm/random/bivariate_gaussian.hh> 
      @brief Gaussian Bivrate distribution interface class. 
      @ingroup dists 

      The Gaussian Bivrate PDF is 
      @f[
         p(x,y) dx dy = \frac{1}{2 \pi \sigma_x \sigma_y \sqrt{1-\rho^2}} 
	   \exp \left(-\frac{x^2/\sigma_x^2 
	                + y^2/\sigma_y^2 
		        - 2 \rho x y/(\sigma_x\sigma_y)}
		       {2(1-\rho^2)}\right) dx dy
      @f]
      for @f$ x,y@f$ in the range @f$-\infty@f$ to @f$+\infty@f$.  The
      correlation coefficient @f$\rho@f$ should lie between 1 and -1.
   */
  class bivariate_gaussian : public distribution<std::pair<double,double> >
  {
  protected:
    /** The base type */
    typedef distribution<std::pair<double,double> > base_type;
    /** Width (@f$\sigma@f$) of the distribution */
    return_type _sigma;
    /** Parameter rho of the distribution */
    double _rho;
  public:
    /** Constructor
	@param sigma the @f$\sigma@f$ of the distribution 
	@param rho the correlation coefficient
	@param r The random number generator */
    bivariate_gaussian(const return_type& sigma, double rho, 
		       generator& r) 
      : base_type(r), _sigma(sigma), _rho(rho)
    {}
    /** Constructor
	@param sigma_x the @f$\sigma_x@f$ of the distribution 
	@param sigma_y the @f$\sigma_y@f$ of the distribution 
	@param rho the correlation coefficient
	@param r The random number generator */
    bivariate_gaussian(double sigma_x, double sigma_y, double rho, 
		       generator& r) 
      : base_type(r), _sigma(sigma_x,sigma_y), _rho(rho)
    {}
    /** Destructor  */
    virtual ~bivariate_gaussian() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type& sample(return_type& v) 
    {
      gsl_ran_bivariate_gaussian(rng(), _sigma.first, _sigma.second, 
				 _rho, &v.first, &v.second);
      return v;
    }
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return_type v;
      return sample(v);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      return gsl_ran_bivariate_gaussian_pdf(x.first, x.second, 
					    _sigma.first, _sigma.second, 
					    _rho);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
