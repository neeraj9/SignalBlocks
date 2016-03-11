//
// $Id: generator.hh,v 1.11 2006-04-28 23:23:49 cholm Exp $ 
//  
//  gslmm::generator
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
#ifndef GSLMM_random_generator
#define GSLMM_random_generator

/** @file   random/generator.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  random number generator class */


/** @defgroup random Random Number Generation and Distributions. 

    These classes cover 3 GSL sections, the @e Random @e Number @e
    Generation, @e Quasi-Random @e Sequences, and @e Random @e Number
    @e Distributions.  I've lumped them all together into one module,
    as that seems natural to me.

    This module consists of three parts: 
    <dl>
      <dt>gslmm::generator</dt>
      <dd> Random number generator.  A varity of algorithms can be
           chosen.  
      </dd>
      <dt>gslmm::quasi_generator</dt>
      <dd> A quasi-random sequence progressively covers a
         @f$ n@f$-dimensional space with a set of points that are
	 uniformly distributed. Quasi-random sequences are also known
	 as low-discrepancy sequences. 
      </dd>
      <dt>gslmm::distribution</dt>
      <dd> Samples from a distribution randomly. 
        Continuous random number distributions are defined by a
	@e probability @e density @e function, @f$ p(x)@f$, such that
        the probability of @f$ x@f$ occurring in the infinitesimal range
	@f$ x @f$ to @f$ x+dx@f$ is @f$ p dx@f$. 

        The @e cumulative @e distribution @e function for the @e lower 
        tail is defined by,
	@f[ 
          P(x) = \int_{-\infty}^{x} dx' p(x')
	@f]
	and gives the probability of a variate taking a value less
        than @f$ x@f$. 

	The cumulative distribution function for the @e upper tail is 
	defined by,
	@f[
          Q(x) = \int_{x}^{\infty} dx' p(x')
	@f]
	and gives the probability of a variate taking a greater than
	@f$ x @f$. 

	The upper and lower cumulative distribution functions are
	related by @f$ P(x) + Q(x) = 1@f$ and satisfy 
	@f$ 0 <= P(x) <= 1, 0 <= Q(x) <= 1@f$.

        The inverse cumulative distributions, @f$ x=P^{-1}(P)@f$ and 
        @f$ x=Q^{-1}(Q) @f$ give the values of @f$ x@f$ which
        correspond to a specific value of @f$ P@f$ or @f$ Q@f$. They
        can be used to find confidence limits from probability
        values. 
      </dd>
   </dl>
*/

#ifndef __GSL_RNG_H__
# include <gsl/gsl_rng.h>
#endif
#ifndef __IOSTREAM__
# include <iostream>
#endif
#ifndef __IOMANIP__
# include <iomanip>
#endif
#ifndef __VECTOR__
# include <vector>
#endif
#ifndef __STRING__
# include <string>
#endif

namespace gslmm 
{
  /** @defgroup gener Generators and Quasi-generators
      @ingroup random 
  */
  /** @class generator random/generator.hh <gslmm/random/generator.hh> 
      @brief Random number generator interface class. 
      @ingroup gener
   */
  class generator
  {
  public:
    /** Type of low-level */
    typedef gsl_rng* low_type;
    /** Type of types */
    typedef const gsl_rng_type* backend_type;
    /** Type of list of types.  */
    typedef std::vector<backend_type> backend_type_list;

    /** Backend types of random number generators.  Pass one of these to
	the constructor of generator to use that random number
	generator backend.

	Please refer to the GSL documentation for information on the
	various backends (called @c gsl_rng_type in GSL)
    */
    enum backend {
      borosh13=1,	/** backend @e borosh13 by Borosh, Niederreiter */
      coveyou,		/** backend @e coveyou by Coveyou*/
      cmrg,		/** backend @e cmrg by by L'Ecuyer */
      fishman18,	/** backend @e fishman18 Fishman, Moore III */
      fishman20,	/** backend @e fishman20 Fishman, Moore III */
      fishman2x,	/** backend @e fishman2x Fishman, Moore III */
      gfsr4,		/** backend @e gfsr4 lagged-fibonacci-like generator */
      knuthran,		/** backend @e knuthran 2. order multiple recursive */
      knuthran2,	/** backend @e knuthran2 2. order multiple recursive*/
      lecuyer21,	/** backend @e lecuyer21 by L'Ecuyer */
      minstd,		/** backend @e minstd "minimal standard" in MatLab */
      mrg,		/** backend @e mrg by L'Ecuyer,  Blouin and Coutre */
      mt19937,		/** backend @e mt19937 of Matsumoto and Nishimura */
      mt19937_1999,	/** backend @e mt19937_1999 variant of mt19937 */
      mt19937_1998,	/** backend @e mt19937_1998 variant of mt19937 */
      r250,		/** backend @e r250 by Kirkpatrick and Stoll */
      ran0,		/** backend @e ran0 */
      ran1,		/** backend @e ran1 */
      ran2,		/** backend @e ran2 */
      ran3,		/** backend @e ran3 */
      rand,		/** backend @e rand from BSD */
      rand48,		/** backend @e rand48 from UNIX */
      random128_bsd,	/** backend @e random128_bsd 128 bit BSD */
      random128_glibc2,	/** backend @e random128_glibc2 128 bit BSD variant */
      random128_libc5,	/** backend @e random128_libc5 128 bit BSD variant */
      random256_bsd,	/** backend @e random256_bsd 128 bit BSD variant */
      random256_glibc2,	/** backend @e random256_glibc2 256 bit BSD variant */
      random256_libc5,	/** backend @e random256_libc5 256 bit BSD variant */
      random32_bsd,	/** backend @e random32_bsd 32 bit BSD variant */
      random32_glibc2,	/** backend @e random32_glibc2 32 bit BSD variant */
      random32_libc5,	/** backend @e random32_libc5 32 bit BSD variant */
      random64_bsd,	/** backend @e random64_bsd 64 bit BSD variant */
      random64_glibc2,	/** backend @e random64_glibc2 64 bit BSD variant */
      random64_libc5,	/** backend @e random64_libc5 64 bit BSD variant */
      random8_bsd,	/** backend @e random8_bsd 8 bit BSD variant */
      random8_glibc2,	/** backend @e random8_glibc2 8 bit BSD variant */
      random8_libc5,	/** backend @e random8_libc5 8 bit BSD variant */
      random_bsd,	/** backend @e random_bsd from BSD */
      random_glibc2,	/** backend @e random_glibc2 from GNU libc */
      random_libc5,	/** backend @e random_libc5 from Linux libc */
      randu,		/** backend @e randu from IBM */
      ranf,		/** backend @e ranf from CRAY */
      ranlux,		/** backend @e ranlux original Luscher algorithm */
      ranlux389,	/** backend @e ranlux389 original Luscher algorithm */
      ranlxd1,		/** backend @e ranlxd1 */
      ranlxd2,		/** backend @e ranlxd2 Double prec. of ranlxs */
      ranlxs0,		/** backend @e ranlxs0 2. generation ranlux */
      ranlxs1,		/** backend @e ranlxs1 2. generation ranlux */
      ranlxs2,		/** backend @e ranlxs2 2. generation ranlux */
      ranmar,		/** backend @e ranmar from CERNLIB */
      slatec,		/** backend @e slatec from SLATEC */
      taus,		/** backend @e taus combined Tausworthe generator */
      taus2,		/** backend @e taus2 combined Tausworthe generator */
      taus113,		/** backend @e taus113 */
      transputer,	/** backend @e transputer from INMOS */
      tt800,		/** backend @e tt800 twisted generalized feedback */
      uni,		/** backend @e uni from SLATEC (16 byte) */
      uni32,		/** backend @e uni32 (32 byte) */
      vax,		/** backend @e vax from VAX */
      waterman14,	/** backend @e waterman14 by Waterman*/
      zuf		/** backend @e zuf ZUFALL lagged Fibonacci series */
    };
    /** Type of list of backends */ 
    typedef std::vector<backend> backend_list;

    /** Translate a backend value to a GSL low-level @c gsl_rng_type*
	@param t The backend value 
	@return The corresponding @c gsl_rng_type*  */
    static backend_type backend2type(backend t) 
    {
      backend_type type = 0;
      switch (t) {
      case borosh13:         type = gsl_rng_borosh13;         break;
      case coveyou:          type = gsl_rng_coveyou;          break;
      case cmrg:             type = gsl_rng_cmrg;             break;
      case fishman18:        type = gsl_rng_fishman18;        break;
      case fishman20:        type = gsl_rng_fishman20;        break;
      case fishman2x:        type = gsl_rng_fishman2x;        break;
      case gfsr4:            type = gsl_rng_gfsr4;            break;
      case knuthran:         type = gsl_rng_knuthran;         break;
      case knuthran2:        type = gsl_rng_knuthran2;        break;
      case lecuyer21:        type = gsl_rng_lecuyer21;        break;
      case minstd:           type = gsl_rng_minstd;           break;
      case mrg:              type = gsl_rng_mrg;              break;
      case mt19937:          type = gsl_rng_mt19937;          break;
      case mt19937_1999:     type = gsl_rng_mt19937_1999;     break;
      case mt19937_1998:     type = gsl_rng_mt19937_1998;     break;
      case r250:             type = gsl_rng_r250;             break;
      case ran0:             type = gsl_rng_ran0;             break;
      case ran1:             type = gsl_rng_ran1;             break;
      case ran2:             type = gsl_rng_ran2;             break;
      case ran3:             type = gsl_rng_ran3;             break;
      case rand:             type = gsl_rng_rand;             break;
      case rand48:           type = gsl_rng_rand48;           break;
      case random128_bsd:    type = gsl_rng_random128_bsd;    break;
      case random128_glibc2: type = gsl_rng_random128_glibc2; break;
      case random128_libc5:  type = gsl_rng_random128_libc5;  break;
      case random256_bsd:    type = gsl_rng_random256_bsd;    break;
      case random256_glibc2: type = gsl_rng_random256_glibc2; break;
      case random256_libc5:  type = gsl_rng_random256_libc5;  break;
      case random32_bsd:     type = gsl_rng_random32_bsd;     break;
      case random32_glibc2:  type = gsl_rng_random32_glibc2;  break;
      case random32_libc5:   type = gsl_rng_random32_libc5;   break;
      case random64_bsd:     type = gsl_rng_random64_bsd;     break;
      case random64_glibc2:  type = gsl_rng_random64_glibc2;  break;
      case random64_libc5:   type = gsl_rng_random64_libc5;   break;
      case random8_bsd:      type = gsl_rng_random8_bsd;      break;
      case random8_glibc2:   type = gsl_rng_random8_glibc2;   break;
      case random8_libc5:    type = gsl_rng_random8_libc5;    break;
      case random_bsd:       type = gsl_rng_random_bsd;       break;
      case random_glibc2:    type = gsl_rng_random_glibc2;    break;
      case random_libc5:     type = gsl_rng_random_libc5;     break;
      case randu:            type = gsl_rng_randu;            break;
      case ranf:             type = gsl_rng_ranf;             break;
      case ranlux:           type = gsl_rng_ranlux;           break;
      case ranlux389:        type = gsl_rng_ranlux389;        break;
      case ranlxd1:          type = gsl_rng_ranlxd1;          break;
      case ranlxd2:          type = gsl_rng_ranlxd2;          break;
      case ranlxs0:          type = gsl_rng_ranlxs0;          break;
      case ranlxs1:          type = gsl_rng_ranlxs1;          break;
      case ranlxs2:          type = gsl_rng_ranlxs2;          break;
      case ranmar:           type = gsl_rng_ranmar;           break;
      case slatec:           type = gsl_rng_slatec;           break;
      case taus:             type = gsl_rng_taus;             break;
      case taus2:            type = gsl_rng_taus2;            break;
      case taus113:          type = gsl_rng_taus113;          break;
      case transputer:       type = gsl_rng_transputer;       break;
      case tt800:            type = gsl_rng_tt800;            break;
      case uni:              type = gsl_rng_uni;              break;
      case uni32:            type = gsl_rng_uni32;            break;
      case vax:              type = gsl_rng_vax;              break;
      case waterman14:       type = gsl_rng_waterman14;       break;
      case zuf:              type = gsl_rng_zuf;              break;
      default:               type = gsl_rng_default;          break;
      }
      return type;
    }
    /** Translate a GSL low-level @c gsl_rng_type* to a backend value
	@param t The @c gsl_rng_type*
	@return The corresponding backend value */
    static backend type2backend(backend_type t=gsl_rng_default) 
    {
      if (t == gsl_rng_borosh13)         return borosh13;
      if (t == gsl_rng_coveyou)          return coveyou;
      if (t == gsl_rng_cmrg)             return cmrg;
      if (t == gsl_rng_fishman18)        return fishman18;
      if (t == gsl_rng_fishman20)        return fishman20;
      if (t == gsl_rng_fishman2x)        return fishman2x;
      if (t == gsl_rng_gfsr4)            return gfsr4;
      if (t == gsl_rng_knuthran)         return knuthran;
      if (t == gsl_rng_knuthran2)        return knuthran2;
      if (t == gsl_rng_lecuyer21)        return lecuyer21;
      if (t == gsl_rng_minstd)           return minstd;
      if (t == gsl_rng_mrg)              return mrg;
      if (t == gsl_rng_mt19937)          return mt19937;
      if (t == gsl_rng_mt19937_1999)     return mt19937_1999;
      if (t == gsl_rng_mt19937_1998)     return mt19937_1998;
      if (t == gsl_rng_r250)             return r250;
      if (t == gsl_rng_ran0)             return ran0;
      if (t == gsl_rng_ran1)             return ran1;
      if (t == gsl_rng_ran2)             return ran2;
      if (t == gsl_rng_ran3)             return ran3;
      if (t == gsl_rng_rand)             return rand;
      if (t == gsl_rng_rand48)           return rand48;
      if (t == gsl_rng_random128_bsd)    return random128_bsd;
      if (t == gsl_rng_random128_glibc2) return random128_glibc2;
      if (t == gsl_rng_random128_libc5)  return random128_libc5;
      if (t == gsl_rng_random256_bsd)    return random256_bsd;
      if (t == gsl_rng_random256_glibc2) return random256_glibc2;
      if (t == gsl_rng_random256_libc5)  return random256_libc5;
      if (t == gsl_rng_random32_bsd)     return random32_bsd;
      if (t == gsl_rng_random32_glibc2)  return random32_glibc2;
      if (t == gsl_rng_random32_libc5)   return random32_libc5;
      if (t == gsl_rng_random64_bsd)     return random64_bsd;
      if (t == gsl_rng_random64_glibc2)  return random64_glibc2;
      if (t == gsl_rng_random64_libc5)   return random64_libc5;
      if (t == gsl_rng_random8_bsd)      return random8_bsd;
      if (t == gsl_rng_random8_glibc2)   return random8_glibc2;
      if (t == gsl_rng_random8_libc5)    return random8_libc5;
      if (t == gsl_rng_random_bsd)       return random_bsd;
      if (t == gsl_rng_random_glibc2)    return random_glibc2;
      if (t == gsl_rng_random_libc5)     return random_libc5;
      if (t == gsl_rng_randu)            return randu;
      if (t == gsl_rng_ranf)             return ranf;
      if (t == gsl_rng_ranlux)           return ranlux;
      if (t == gsl_rng_ranlux389)        return ranlux389;
      if (t == gsl_rng_ranlxd1)          return ranlxd1;
      if (t == gsl_rng_ranlxd2)          return ranlxd2;
      if (t == gsl_rng_ranlxs0)          return ranlxs0;
      if (t == gsl_rng_ranlxs1)          return ranlxs1;
      if (t == gsl_rng_ranlxs2)          return ranlxs2;
      if (t == gsl_rng_ranmar)           return ranmar;
      if (t == gsl_rng_slatec)           return slatec;
      if (t == gsl_rng_taus)             return taus;
      if (t == gsl_rng_taus2)            return taus2;
      if (t == gsl_rng_taus113)          return taus113;
      if (t == gsl_rng_transputer)       return transputer;
      if (t == gsl_rng_tt800)            return tt800;
      if (t == gsl_rng_uni)              return uni;
      if (t == gsl_rng_uni32)            return uni32;
      if (t == gsl_rng_vax)              return vax;
      if (t == gsl_rng_waterman14)       return waterman14;
      if (t == gsl_rng_zuf)              return zuf;
      return mt19937;
    }
    
    /** Constructor  */
    generator(backend_type type=gsl_rng_default) 
      : _rng(0)
    {
      _rng = gsl_rng_alloc(type);
    }
    /** Constructor
	@param s Name of the generator to use. */
    generator(const std::string& s) 
      : _rng(0)
    {
      backend_type_list& l = types();
      backend_type type    = gsl_rng_default;
      if (s.size() != 0) {
	for (backend_type_list::const_iterator i = l.begin(); 
	     i != l.end(); ++i) 
	  if (s == (*i)->name) type = *i;
      }
      _rng = gsl_rng_alloc(type);
    }
    /** Constructor. 
	@param t A number that says which generator type to use. 
    */
    generator(backend t) 
      : _rng(0)
    {
      backend_type type = backend2type(t);
      _rng = gsl_rng_alloc(type);
    }
      
    /** Copy constructor. 
	@param other Object to copy from */
    generator(const generator& other) 
    {
      _rng = gsl_rng_clone(other._rng);
    }
    /** Destructor   */
    virtual ~generator() 
    {
      gsl_rng_free(_rng);
    }

    /** Assignment operator. 
	@param other Object to assign from.  Must be of the same type
	as this object. 
	@return  this object. */
    generator& operator=(const generator& other) 
    {
      gsl_rng_memcpy(_rng, other._rng);
      return *this;
    }
    /** function operator.  The same as get. 
	@return The same as get.  */
    unsigned long int operator()()
    {
      return get();
    }
    
    /** Set the seed of the random number generator. 
	@param seed The seed.  */
    void set(unsigned long int seed=gsl_rng_default_seed) 
    {
      gsl_rng_set(_rng, seed);
    }
    /** Get a uniformly distributed random number in the range from
	@f$ [min,max] @f$ where @f$ min@f$ and @f$ max@f$ i given by
	the member functions of the same name. 
	@return Uniform random number @f$ \in [min,max] @f$ */
    unsigned long int get() 
    {
      return gsl_rng_get(_rng);
    }
    /** The minimum of the random number generator 
	@return minimum of the random number generator  */
    unsigned long min() const 
    {
      return gsl_rng_min(_rng);
    }
    /** The maximum of the random number generator 
	@return maximum of the random number generator  */
    unsigned long max() const 
    {
      return gsl_rng_max(_rng);
    }
    /** Returns a double precision floating point number uniformly
	distributed in the range @f$ [0,1)@f$.  The range includes 0
	but excludes 1.   
	@return random number uniformly distributed in the range @f$
	[0,1)@f$ */ 
    double uniform() 
    {
      return gsl_rng_uniform(_rng);
    }
    /** Returns a double precision floating point number uniformly
	distributed in the range @f$ (0,1)@f$.  The range includes
	excluding both 0 and 1.   
	@return random number uniformly distributed in the range @f$
	(0,1)@f$ */ 
    double uniform_nonzero() 
    {
      return gsl_rng_uniform(_rng);
    }
    /** This function returns a random integer from 0 to @a n-1
	inclusive.  All integers in the range @f$ [0,N-1]@f$ are
	equally likely, regardless of the generator used.  
	@param n  The maximum
	@return random integer from 0 to @a n-1 inclusive */
    unsigned long int integer(unsigned long int n) 
    {
      return gsl_rng_uniform_int(_rng, n);
    }
    
    /** Get the name of the random number generator. 
	@return name of the random number generator. */
    const char* name() const 
    {
      return gsl_rng_name(_rng);
    }
    
    /** Get the state of the random number generator.  
	@return state of the random number generator. */
    void* state() const 
    {
      return gsl_rng_state(_rng);
    }
    
    /** Get the size of the state 
	@return size of the state */
    size_t size() const 
    {
      return gsl_rng_size(_rng);
    }

    /** Reads the enviroment variables @c GSL_RNG_TYPE and @c
	GSL_RNG_SEED and sets up the default random number generator
	type and seed. 
     */
    static void env_setup() 
    {
      gsl_rng_env_setup();
    }

    /** Get the default seed as read from the environment */
    static unsigned long int default_seed() 
    {
      return gsl_rng_default_seed;
    }
    /** Get the default type as read from the environment */
    static backend default_backend() 
    {
      backend_type t = gsl_rng_default;
      return type2backend(t);
    }
    /** Get the default type as read from the environment */
    static backend_type default_type() 
    {
      return gsl_rng_default;
    }
    
    /** Static function that returns a list of all known types. 
	@return List of known types. */
    static backend_type_list& types() 
    {
      static backend_type_list r;
      if (r.size() > 0) return r;
      for (backend_type* n = gsl_rng_types_setup(); *n != 0; n++)
	r.push_back(*n);
      return r;
    }
    /** Static function that returns a list of all known types. 
	@return List of known types. */
    static backend_list& backends() 
    {
      static backend_list r;
      if (r.size() > 0) return r;
      backend_type_list& l = types();
      for (backend_type_list::const_iterator i = l.begin(); i != l.end(); ++i)
	r.push_back(type2backend(*i));
      return r;
    }

    /** Shuffles the order of the ojects in @a v.  The generator is
	used to produce random numbers for the permutation. 

	@code 
	std::vector<int> a(52);
	for (size_t i = 0; i < a.size(); i++) a[i] = i;
	gslmm::generator r;
	r.shuffle(a);
	@endcode 

	@param v The array to shuffle */
    template <typename Type> 
    void shuffle(std::vector<Type>& v) 
    {
      gsl_ran_shuffle(_rng, &(v[0]), v.size(),
		      sizeof(typename std::vector<Type>::value_type));
    }
    /** Fills the array @a dest with @a dets@c .size() elements from
	@a src.  The output of the generator is used to make the
	selection.  The algorithm ensures that all possible samples
	are equally likely, assuming a perfect source of randomness. 
	
	The objects are sampled @e without replacement, thus each
	object in @a src can only appear once in @a dest.  The size of
	@a dest must be smaller than or equal to the size of @a src.
	The objects stored in @a dest will be in the same relative
	order as in @a src.  You can use shuffle to randomise the
	order.  

	@code 
	std::vector a(3);
	std::vector b(100);
	for (size_t i = 0; i < b.size(); i++) b[i] = i;
	gslmm::generator r;
	r.choose(a, b);
	@endcode 

	@param dest Destination array 
	@param src Source array 
    */
    template <typename Type>
    void choose(std::vector<Type>& dest, const std::vector<Type>& src) 
    {
      gsl_ran_choose(_rng, &(dest[0]), dest.size(), 
		     const_cast<Type*>(&(src[0])), 
		     src.size(), 
		     sizeof(typename std::vector<Type>::value_type));
    }
    /** Sample @a dest@c .size() items from @a src with replacement,
	so that the same object may appear more than once in @a dest
	after evaluation.  The size of @a dest needn't be smaller than
	or equal to the size of @a src
	@param dest Destination array 
	@param src Source array 
    */
    template <typename Type> 
    void sample(std::vector<Type>& dest, const std::vector<Type>& src) 
    {
      gsl_ran_choose(_rng, &(dest[0]), dest.size(), 
		     const_cast<Type*>(&(src[0])), 
		     src.size(), 
		     sizeof(typename std::vector<Type>::value_type));
    }
    operator low_type() { return _rng; }
    /** The low-level object */
    gsl_rng* _rng;
  private:

    template <typename Value>
    friend class distribution;
  };

  /**@{*/
  /** @name Output streams for random number generators */
  /** Put to operator for random number generators. 
      @param o Output stream 
      @param r random number generator. 
      @return  @a o after streaming @a r to it. */
  inline 
  std::ostream& operator<<(std::ostream& o, generator& r) 
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
