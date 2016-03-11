//
// $Id: quasi_generator.hh,v 1.3 2006-04-26 23:08:16 cholm Exp $ 
//  
//  gslmm::quasi_generator
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
#ifndef GSLMM_random_quasi_generator
#define GSLMM_random_quasi_generator

/** @file   random/quasi_generator.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  quasi_random class */

#ifndef __GSL_QRNG_H__
# include <gsl/gsl_qrng.h>
#endif
#ifndef __IOSTREAM__
# include <iostream>
#endif
#ifndef __IOMANIP__
# include <iomanip>
#endif
#ifndef __CASSERT__
# include <cassert>
#endif
#ifndef __VECTOR__
# include <vector>
#endif
#ifndef __STRING__
# include <string>
#endif

namespace gslmm 
{
  /** @class quasi_generator quasi_generator.hh <gslmm/random/quasi_generator.hh> 
      @brief Random number generator interface class. 
      @ingroup gener
   */
  class quasi_generator
  {
  private:
    /** The low-level object */
    gsl_qrng* _qrng;
  public:
    /** Backend types of random number generators.  Pass one of these to
	the constructor of random_generator to use that random number
	generator backend.

	Please refer to the GSL documentation for information on the
	various backends (called @c gsl_qrng_type in GSL)
    */
    enum backend {
      niederreiter_2,  /** Alogrithm by  Bratley, Fox, and Niederreiter */
      sobol            /** Algorithm using a Sobol sequence */
    };

    /** Type of types */
    typedef const gsl_qrng_type* backend_type;
    /** Type of list of types.  */
    typedef std::vector<backend_type> backend_list;
    /** Constructor  
	@param d Dimension of the generator 
	@param t the backend type */
    quasi_generator(size_t d, backend_type t=gsl_qrng_niederreiter_2) 
      : _qrng(0)
    {
      _qrng = gsl_qrng_alloc(t, d);
    }
    /** Constructor
	@param d Dimension of the generator 
	@param s Name of the generator to use. */
    quasi_generator(size_t d, const std::string& s) 
      : _qrng(0)
    {
      backend_type type = 0;
      if      (s == "niederreiter_2") type = gsl_qrng_niederreiter_2; 
      else if (s == "sobol")          type = gsl_qrng_sobol;
      else                            type = gsl_qrng_niederreiter_2; 
      _qrng = gsl_qrng_alloc(type,d);
    }
    /** Constructor. 
	@param d Dimension of the generator 
	@param t A number that says which generator type to use. 
    */
    quasi_generator(size_t d, backend t) 
      : _qrng(0)
    {
      backend_type type = 0;
      switch (t) {
      case niederreiter_2:   type = gsl_qrng_niederreiter_2; break;
      case sobol:            type = gsl_qrng_sobol;          break;
      default:               type = gsl_qrng_niederreiter_2; break;
      }
      _qrng = gsl_qrng_alloc(type,d);
    }
    
      
    /** Copy constructor. 
	@param other Object to copy from */
    quasi_generator(const quasi_generator& other) 
    {
      _qrng = gsl_qrng_clone(other._qrng);
    }
    /** Destructor   */
    virtual ~quasi_generator() 
    {
      gsl_qrng_free(_qrng);
    }

    /** Assignment operator. 
	@param other Object to assign from.  Must be of the same type
	as this object. 
	@return  this object. */
    quasi_generator& operator=(const quasi_generator& other) 
    {
      gsl_qrng_memcpy(_qrng, other._qrng);
      return *this;
    }
    /** function operator.  The same as get. 
	@return The same as get.  */
    bool operator()(std::vector<double>& x)
    {
      return get(x);
    }
    
    /** Reinitialize the generator (argument is for compatibility with
	gslmm::random) 
    */
    void set(unsigned long int=0) 
    {
      gsl_qrng_init(_qrng);
    }
    /** Get the next point in the sequence and store it in the output
	variable @a x.  @f$ 0 < x_i < 1 @f$ for each @f$ x_i \in
	x@f$.  @a x must have the same, or higher dimension as the
	generator. 
	@return Uniform random number @f$ \in [min,max] @f$ */
    bool get(std::vector<double>& x) 
    {
      assert(x.size() >= _qrng->dimension);
      return gsl_qrng_get(_qrng,&(x[0])) == GSL_SUCCESS;
    }
    /** Get the name of the random number generator. 
	@return name of the random number generator. */
    const char* name() const 
    {
      return gsl_qrng_name(_qrng);
    }
    
    /** Get the state of the random number generator.  
	@return state of the random number generator. */
    void* state() const 
    {
      return gsl_qrng_state(_qrng);
    }
    
    /** Get the size of the state 
	@return size of the state */
    size_t size() const 
    {
      return gsl_qrng_size(_qrng);
    }
    
    /** Get the dimension of the generator */
    size_t dimension() const 
    {
      return _qrng->dimension;
    }
  };

  /**@{*/
  /** @name Output streams for quasi random number generators */
  /** Put to operator for random number generators. 
      @param o Output stream 
      @param r random number generator. 
      @return  @a o after streaming @a r to it. */
  std::ostream& operator<<(std::ostream& o, quasi_generator& r) 
  {
    unsigned char* p = static_cast<unsigned char*>(r.state());
    const size_t   n = r.size();
    for (size_t i = 0; i < n; ++i) 
      o << std::hex << std::setprecision(2) << int(p[i]);
    
    return o;
  }
  /**@}*/
}

    
    
#endif
//____________________________________________________________________
//
// EOF
//
