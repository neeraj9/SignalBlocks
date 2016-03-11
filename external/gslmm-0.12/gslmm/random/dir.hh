//
// $Id: dir.hh,v 1.5 2006-05-09 07:40:34 cholm Exp $ 
//  
//  gslmm::dir
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
#ifndef GSLMM_random_dir
#define GSLMM_random_dir

/** @file   random/dir.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random dir classes */

#ifndef GSLMM_random_distribution
# include <gslmm/random/distribution.hh>
#endif
#ifndef GSLMM_vectormatrix_vector_double
# include <gslmm/vectormatrix/vector_double.hh>
#endif

namespace gslmm 
{
  /** @class dir random/dir.hh <gslmm/random/dir.hh> 
      @brief Spherical distribution interface class. 
      @ingroup dists
      
      Returns a unit random vector in a @f$ N@f$ dimensional space. 

      For @f$ n = 1@f$ this gives the vector @f$\mathbf{v} = (1)@f$

      For @f$ n = 2@f$ this gives a random direction vector 
      @f$ \mathbf{v} = (x,y)@f$ in two dimensions.  The vector is
      normalized such that @f$ |\mathbf{v}|^2 = x^2 + y^2 = 1@f$.  The
      obvious way to do this is to take a uniform random number
      between @f$ 0@f$ and @f$ 2\pi@f$ and let @f$ x@f$ and @f$ y@f$
      be the sine and cosine respectively (this is done if the
      parameter @a trig of the constructor is @c true).  Two trig
      functions would have been expensive in the  old days, but with
      modern hardware implementations, this is sometimes the fastest
      way to go.  This is the case for the Pentium (but not the case
      for the Sun Sparcstation).  One can avoid the trig evaluations
      by choosing @f$ x@f$ and @f$ y@f$ in the interior of a unit
      circle (choose them at random from the interior of the enclosing
      square, and then reject those that are outside the unit circle),
      and then dividing by @f$ \sqrt{x^2 + y^2}@f$.  A much cleverer
      approach, attributed to von Neumann requires neither trig nor a
      square root.  In this approach, @f$ u@f$ and @f$ v@f$ are chosen
      at random from the interior of a unit circle, and then 
      @f$ x=(u^2-v^2)/(u^2+v^2)@f$ and @f$ y=uv/(u^2+v^2)@f$

      For @f$ n = 3@f$ this gives a random direction vector 
      @f$ \mathbf{v} = (x,y,z)@f$ in three dimensions.  The vector is
      normalized such that @f$ |\mathbf{v}|^2 = x^2 + y^2 + z^2 = 1@f$.
      The method employed is due to Robert E. Knop, and explained in
      Knuth.  It uses the surprising fact that the distribution
      projected along any axis is actually uniform (this is only true
      for 3d). 

      For @f$ n > 3@f$ this gives a random direction vector 
      @f$ \mathbf{v} = (x_1,x_2,...,x_n)@f$ in @f$ n@f$ dimensions.
      The vector is normalized such that 
      @f$ |\mathbf{v}|^2 = x_1^2 + x_2^2 + ... + x_n^2 = 1@f$.  The
      method uses the fact that a multivariate gaussian distribution
      is spherically symmetric.  Each component is generated to have a
      gaussian distribution, and then the components are normalized.
      The method is described by Knuth, and attributed to G. W. Brown.
  */
  class dir : public distribution<gslmm::vector<double> >
  {
  private:
    /** The base type */
    typedef distribution<gslmm::vector<double> > base_type;
    /** Dimension of the distribution */
    size_t _n;
    /** Parameter trig of the distribution */
    bool _trig;
  public:
    /** Constructor 
	@param n The dimension of the vector returned. 
	@param trig Use trigonomic implementation for 2D 
	@param r Random number generator  */
    dir(size_t n, generator& r, bool trig=false) 
      : base_type(r), _n(n), _trig(trig)
    {}
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type& sample(return_type& v) 
    {
      switch (_n) {
      case 1: v[0] = 1;
      case 2:
	if (_trig) gsl_ran_dir_2d(rng(), &(v[0]), &(v[1]));
	else       gsl_ran_dir_2d_trig_method(rng(), &(v[0]), &(v[1]));
	break;
      case 3:
	gsl_ran_dir_3d(rng(), &(v[0]), &(v[1]), &(v[2]));
	break;
      default:
	gsl_ran_dir_nd(rng(), _n, &(v[0]));
      }
      return v;
    }
    /** Sample the distribution. 
	@return The sample. */
    virtual return_type sample() 
    {
      return_type v(_n);
      return sample(v);
    }
    
    /** @see gslmm::distribution<Return,Arg>::pdf */
    virtual double pdf(const return_type& x) 
    {
      switch (_n) {
      case 1: return (x[0] == 1 ? 1 : 0);
      case 2: return 1. / (2. * M_PI);
      case 3: return 1. / (4. * M_PI);
      }
      GSL_ERROR_VAL("pdf not defined for n > 2", GSL_EUNIMPL, 0);
    }    

    /** Evaluate the inverse of @e lower @e cumulative @e distribution
	@e function at @f$ P@f$ 
	@warning If the derived class does not overload this member
	function, the error handler will be invoked.  
	@return @f$ P^{-1}(P)@f$ */
    virtual return_type lower_invcdf(double)
    {
      GSL_ERROR_VAL("lower_cdf not defined", GSL_EUNIMPL, return_type(_n));
    }
    /** Evaluate the inverse of @e upper @e cumulative @e distribution
	@e function at @f$ P@f$ 
	@warning If the derived class does not overload this member
	function, the error handler will be invoked.  
	@return @f$ Q^{-1}(Q)@f$ */
    virtual return_type upper_invcdf(double)
    {
      GSL_ERROR_VAL("lower_cdf not defined", GSL_EUNIMPL, return_type(_n));
    }
    /** Get the dimensionality of the generator. 
	@return  the dimensionality of the generator.  */
    size_t size() const 
    {
      return _n;
    }
    
  };
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
