//
// $Id: gumbel.hh,v 1.3 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::gumbel
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
#ifndef GSLMM_random_gumbel
#define GSLMM_random_gumbel

/** @file   random/gumbel.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random gumbel classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif

namespace gslmm 
{
  /** @class gumbel random/gumbel.hh <gslmm/random/gumbel.hh> 
      @brief Gumbel distribution (type 1 and 2) interface class. 
      @ingroup dists 

      The Gumbel type 1 PDF is 
      @f[
        p(x) dx = a b \exp(-(b \exp(-ax) + ax)) dx
      @f]
      for @f$ -\infty < x < \infty@f$.

      The Gumbel type 2 PDF is 
      @f[
        p(x) dx = a b x^{-a-1} \exp(-b x^{-a}) dx
      @f]
      for @f$ 0 < x < \infty@f$.
   */
  class gumbel : public distribution<double>
  {
  protected:
    /** The base type */
    typedef distribution<double> base_type;
    /** Parameter type of the distribution */
    size_t _type;
    /** Parameter a of the distribution */
    double _a;
    /** Parameter b of the distribution */
    double _b;
  public:
    /** Constructor
	@param t The type of the PDF (1 or 2) 
	@param a The parameter @f$ a@f$ of the PDF
	@param b The parameter @f$ b@f$ of the PDF
	@param r The random number generator */
    gumbel(size_t t, double a, double b, generator& r) 
      : base_type(r), _type(t), _a(a), _b(b)
    {}
    /** Destructor  */
    virtual ~gumbel() {}
    
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      switch (_type) {
      case 1: return gsl_ran_gumbel1(rng(), _a, _b);
      case 2: return gsl_ran_gumbel2(rng(), _a, _b);
      }
      GSL_ERROR_VAL("sample not defined for t != 1, 2", GSL_EUNIMPL, 0);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      switch (_type) {
      case 1: return gsl_ran_gumbel1_pdf(x, _a, _b);
      case 2: return gsl_ran_gumbel2_pdf(x, _a, _b);
      }
      GSL_ERROR_VAL("pdf not defined for t != 1, 2", GSL_EUNIMPL, 0);
    }
    
    /** @see gslmm::distribution<Return,Arg>::lower_cdf */
    virtual double lower_cdf(const return_type& x) 
    {
      switch (_type) {
      case 1: return gsl_cdf_gumbel1_P(x, _a, _b);
      case 2: return gsl_cdf_gumbel2_P(x, _a, _b);
      }
      GSL_ERROR_VAL("cdf not defined for t != 1, 2", GSL_EUNIMPL, 0);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_cdf */
    virtual double upper_cdf(const return_type& x)
    {
      switch (_type) {
      case 1: return gsl_cdf_gumbel1_Q(x, _a, _b);
      case 2: return gsl_cdf_gumbel2_Q(x, _a, _b);
      }
      GSL_ERROR_VAL("cdf not defined for t != 1, 2", GSL_EUNIMPL, 0);
    }

    /** @see gslmm::distribution<Return,Arg>::lower_invcdf */
    virtual return_type lower_invcdf(double p)
    {
      switch (_type) {
      case 1: return gsl_cdf_gumbel1_Pinv(p, _a, _b);
      case 2: return gsl_cdf_gumbel2_Pinv(p, _a, _b);
      }
      GSL_ERROR_VAL("cdf not defined for t != 1, 2", GSL_EUNIMPL, 0);
    }
    /** @see gslmm::distribution<Return,Arg>::upper_invcdf */
    virtual return_type upper_invcdf(double q)
    {
      switch (_type) {
      case 1: return gsl_cdf_gumbel1_Qinv(q, _a, _b);
      case 2: return gsl_cdf_gumbel2_Qinv(q, _a, _b);
      }
      GSL_ERROR_VAL("cdf not defined for t != 1, 2", GSL_EUNIMPL, 0);
    }
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
