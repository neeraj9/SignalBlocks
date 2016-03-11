//
// $Id: permutation.hh,v 1.13 2006-05-01 14:25:33 cholm Exp $ 
//  
//  gslmm::permutation
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
#ifndef GSLMM_permutation
#define GSLMM_permutation

/** @file   permutation.hh
    @author Christian Holm
    @date   Wed Mar 05 01:07:43 2003
    @brief  A permutation @f$ p @f$ representation.*/ 

#ifndef __GSL_PERMUTATION_H__
#include <gsl/gsl_permutation.h>
#endif
#ifndef __GSL_PERMUTE_H__
#include <gsl/gsl_permute.h>
#endif
#ifndef __GSL_PERMUTE_VECTOR_H__
#include <gsl/gsl_permute_vector.h>
#endif
#ifndef __CINT__
# ifndef __GSL_ERRNO_H__
#  include <gsl/gsl_errno.h>
# endif
#endif
#ifndef GSLMM_util
#include <gslmm/gslmm-util.hh>
#endif
#ifndef GSLMM_vectormatrix_vector
#include <gslmm/vectormatrix/vector.hh>
#endif
#ifndef GSLMM_compleks_complex
#include <gslmm/compleks/complex.hh>
#endif
#ifndef __IOSTREAM__
#include <iostream>
#endif
#ifndef __IOMANIP__
#include <iomanip>
#endif
#if defined(G__DICTIONARY) || defined(__CINT__)
typedef long double    longdouble;
typedef unsigned long  unsignedlong;
typedef unsigned int   unsignedint;
typedef unsigned char  unsignedchar;
typedef unsigned short unsignedshort;
#endif

/** @defgroup permutation Permutations . */
namespace gslmm
{
  /** @class permutation permutation.hh <gslmm/permutation/permutation.hh>
      @brief Permutation representation. 
      A permutation @f$ p @f$ is represented by an array of 
      @f$ n @f$ integers in the range @f$ 0 \dots n-1 @f$, where each
      value @f$ p_i @f$ occurs once and only once. 

      Note that this is @e not a template, as it only makes sense for
      integer values. 

      @ingroup permutation
      @todo Should I leave in the put-to operator?
   */
  class permutation
  {
  protected:
    /// Wether this is in canonical form or not. 
    bool _canonical;
    /// 
    friend permutation operator*(const permutation& lhs, 
				 const permutation& rhs);
  public:
    /// The underlying GSL data
    gsl_permutation* _permutation;
    /** Constructor.
	@param n The size of the permutation
	@param id if true, initialise this to identity, otherwise,
	it's not initialised at all. */
    permutation(size_t n, bool id=true);
    /** Copy constructor */
    permutation(const permutation& o);
    /** Destructor. */
    virtual ~permutation();

    /** Get the size of this permutation. */
    size_t size() const;
    /** Check if this is a valid permutation. */
    bool is_valid() const;
    /** Check if this is in canonical form or not. */
    bool is_canonical() const { return _canonical; }

    /** Access an element in the permutation. */
    size_t& operator[](const size_t i);
    /** Access an element in the permutation. */
    const size_t& operator[](const size_t i) const;
    /** Swap element @a i and @a j */
    void swap(const size_t i, const size_t j);

    /** Make this permutation the identity permutation. */
    void identity();
    /** Turn this permutation into it's reverse. */
    void reverse();
    /** Get the inverse permutation of this. */
    permutation inverse();

    /** Advances this permutation to the next permutation in
	lexicographic order. Starting with the identity permutation
	and repeatedly applying this operation will iterate through
	all possible permutations of a given order.
	@return true if the permutation was advanced.  If no further  
	permutations are available, returns false and leaves the
	permutation unmodified.  */
    bool next();
    /** This function steps backwards from this permutation to the 
	previous permutation in lexicographic order. 
	@return see also gslmm::permutation::next */
    bool previous();

    /** Count number of inversions */
    size_t inversions() const; 
    /** Count the number of cycles. */
    size_t cycles() const;
    /** Return canonical form of this permutation.  If the permutation
	is already on canonical form, this is just a copy. */
    permutation canonical();
    /** Return linear form of this permutation.  If the permutation is 
	already on linear form, this is just a copy. */
    permutation linear();

    /** Permute the array @a data with stride @a stride. 
	@param data The array to permute.
	@param n Size of @a data array
	@param stride The stride in the array 
	@param inv if true, apply the inverse permutation. */
    template <typename Type>
    void permute(Type data, size_t n, size_t stride=1, bool inv=false) const 
    { 
      (void)data;
      (void)n;
      (void)stride;
      (void)inv;
    }
    /** Permute the vector @a v. */
    template <typename Type> 
    void permute(vector<Type>& v, bool inv=false) const {}
    /** Permute the complex number @a c. */
    template <typename Type> 
    void permute(complex<Type>& c, bool inv=false) const {}
    
    /** Assignment operator */
    permutation& operator=(const permutation& o);
    /** * assignment operator */
    permutation& operator*=(const permutation& o);
  };
  //__________________________________________________________________
  inline permutation::permutation(size_t n, bool id) 
    : _canonical(false),
      _permutation(0)
  {
    if (id) _permutation = gsl_permutation_calloc(n);
    else    _permutation = gsl_permutation_alloc(n);
  }
  //__________________________________________________________________
  inline permutation::permutation(const permutation& o) 
    : _canonical(o._canonical),
      _permutation(0)
  {
    _permutation = gsl_permutation_alloc(o.size());
    gsl_permutation_memcpy(_permutation,o._permutation);
  }
  //__________________________________________________________________
  inline permutation::~permutation() 
  {
    gsl_permutation_free(_permutation);
  }
  //__________________________________________________________________
  inline permutation& permutation::operator=(const permutation& o) 
  {
    _canonical = o._canonical;
    gsl_permutation_memcpy(_permutation,o._permutation);
    return *this;
  }
  //__________________________________________________________________
  inline permutation& permutation::operator*=(const permutation& o) 
  {
    _canonical = o._canonical;
    gsl_permutation* tmp = gsl_permutation_alloc(o.size());
    gsl_permutation_mul(tmp,_permutation,o._permutation);
    gsl_permutation_free(_permutation);
    _permutation = tmp;
    return *this;
  }
  //__________________________________________________________________
  inline permutation operator*(const permutation& lhs, 
			       const permutation& rhs) 
  {
    permutation tmp(lhs.size());
    gsl_permutation_mul(tmp._permutation,lhs._permutation,
			rhs._permutation);
    return tmp;
  }
  //__________________________________________________________________
  inline size_t permutation::size() const
  {
    return gsl_permutation_size(_permutation);
  }
  //__________________________________________________________________
  inline bool permutation::is_valid() const
  {
    return gsl_permutation_valid(_permutation) == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline size_t& permutation::operator[](const size_t i) 
  {
    return gsl_permutation_data(_permutation)[i];
  }
  //__________________________________________________________________
  inline const size_t& permutation::operator[](const size_t i) const
  {
    return gsl_permutation_data(_permutation)[i];
  }
  //__________________________________________________________________
  inline void permutation::swap(const size_t i, const size_t j)
  {
   gsl_permutation_swap(_permutation,i,j);
  }
  //__________________________________________________________________
  inline void permutation::identity() 
  {
    gsl_permutation_init(_permutation);
  }
  //__________________________________________________________________
  inline void permutation::reverse() 
  {
    gsl_permutation_reverse(_permutation);
  }
  //__________________________________________________________________
  inline permutation permutation::inverse() 
  {
    permutation p(size(),false);
    gsl_permutation_inverse(p._permutation,_permutation);
    return p;
  }
  //__________________________________________________________________
  inline bool permutation::next() 
  {
    return gsl_permutation_next(_permutation) == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline bool permutation::previous() 
  {
    return gsl_permutation_next(_permutation) == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline size_t permutation::inversions() const
  {
    return gsl_permutation_inversions(_permutation);
  }
  //__________________________________________________________________
  inline size_t permutation::cycles() const
  {
    if (_canonical) 
      return gsl_permutation_canonical_cycles(_permutation);
    return gsl_permutation_linear_cycles(_permutation);
  }
  //__________________________________________________________________
  inline permutation permutation::canonical()
  {
    if (_canonical) return *this;
    permutation p(this->size(),false);
    p._canonical = true;
    gsl_permutation_linear_to_canonical(p._permutation,_permutation);
    return p;
  }
  //__________________________________________________________________
  inline permutation permutation::linear()
  {
    if (!_canonical) return *this;
    permutation p(this->size(),true);
    p._canonical = false;
    gsl_permutation_canonical_to_linear(p._permutation,_permutation);
    return p;
  }
#define DATA gsl_permutation_data(_permutation) 
#define PERMUTE(t,i) gsl_permute ## t ## i(DATA,data,stride,n)
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<double*>(double* data, size_t n, 
				size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(,);
    else      PERMUTE(,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<long double*>(long double* data, size_t n, 
				     size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(_long_double,);
    else      PERMUTE(_long_double,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<float*>(float* data, size_t n, 
			       size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(_float,);
    else      PERMUTE(_float,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<long*>(long* data, size_t n, 
			      size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(_long,);
    else      PERMUTE(_long,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<unsigned long*>(unsigned long* data, size_t n, 
				       size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(_ulong,);
    else      PERMUTE(_ulong,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<int*>(int* data, size_t n, 
			     size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(_int,);
    else      PERMUTE(_int,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<unsigned int*>(unsigned int* data, size_t n, 
				      size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(_uint,);
    else      PERMUTE(_uint,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<short*>(short* data, size_t n, 
			       size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(_short,);
    else      PERMUTE(_short,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<unsigned short*>(unsigned short* data, size_t n, 
					size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(_ushort,);
    else      PERMUTE(_ushort,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<char*>(char* data, size_t n, 
			      size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(_char,);
    else      PERMUTE(_char,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<unsigned char*>(unsigned char* data, size_t n, 
				       size_t stride, bool inv) const
  {
    if (!inv) PERMUTE(_uchar,);
    else      PERMUTE(_uchar,_inverse);
  }
#undef PERMUTE
#define PERMUTE(t,i) gsl_permute_vector ## t ## i(_permutation,v._vector) 
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<double>(gslmm::vector<double>& v, 
			       bool inv) const
  {
    if (!inv) PERMUTE(,);
    else      PERMUTE(,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<long double>(gslmm::vector<long double>& v, 
				    bool inv) const
  {
    if (!inv) PERMUTE(_long_double,);
    else      PERMUTE(_long_double,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<float>(gslmm::vector<float>& v, 
				    bool inv) const
  {
    if (!inv) PERMUTE(_float,);
    else      PERMUTE(_float,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<long>(gslmm::vector<long>& v, bool inv) const
  {
    if (!inv) PERMUTE(_long,);
    else      PERMUTE(_long,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<unsigned long>(gslmm::vector<unsigned long>& v,
						    bool inv) const
  {
    if (!inv) PERMUTE(_ulong,);
    else      PERMUTE(_ulong,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<int>(gslmm::vector<int>& v, bool inv) const
  {
    if (!inv) PERMUTE(_int,);
    else      PERMUTE(_int,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<unsigned int>(gslmm::vector<unsigned int>&v,
				     bool inv) const
  {
    if (!inv) PERMUTE(_uint,);
    else      PERMUTE(_uint,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<short>(gslmm::vector<short>& v, bool inv) const
  {
    if (!inv) PERMUTE(_short,);
    else      PERMUTE(_short,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<unsigned short>(gslmm::vector<unsigned short>& v, 
				       bool inv) const
  {
    if (!inv) PERMUTE(_ushort,);
    else      PERMUTE(_ushort,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<char>(gslmm::vector<char>& v, bool inv) const
  {
    if (!inv) PERMUTE(_char,);
    else      PERMUTE(_char,_inverse);
  }
  //__________________________________________________________________
  template <>
  inline void 
  permutation::permute<unsigned char>(gslmm::vector<unsigned char>& v, 
				      bool inv) const
  {
    if (!inv) PERMUTE(_uchar,);
    else      PERMUTE(_uchar,_inverse);
  }
  //@{
  /// @name Permutation multiplication on vectors
  //__________________________________________________________________
  /** Apply a permutation to a vector, using a multiplicative
      notation. 
      @param p The permutation to apply (on the left) to the vector. 
      @param v the vector to apply the permutation to. 
      @ingroup permutation 
  */
  template <typename Type> 
  inline gslmm::vector<Type> 
  operator*(const permutation& p, const gslmm::vector<Type>& v) 
  {
    gslmm::vector<Type> vv(v);
    p.permute(vv);
    return vv;
  }
  //__________________________________________________________________
  /** Apply a permutation to a vector, using a multiplicative
      notation. 
      @param p The permutation to apply (on the right) to the vector. 
      @param v the vector to apply the permutation to. 
      @ingroup permutation 
  */
  template <typename Type> 
  inline gslmm::vector<Type> 
  operator*(const gslmm::vector<Type>& v, const permutation& p) 
  {
    gslmm::vector<Type> vv(v);
    p.permute(vv, true);
    return vv;
  }
  //@}
  //@{
  /// @name Output streamers for permutations
  /** Write a premutation to output stream. 
      @ingroup permutation
      @param o The output stream 
      @param p The permutation. 
      @return the output stream. */
  inline std::ostream& operator<<(std::ostream& o, const permutation& p) 
  {
    o << "[";
    for (size_t i = 0; i < p.size(); i++) o << std::setw(3) << p[i];
    return o << "]";
  }
  //@}
}
#undef DATA  
#undef PERMUTE

#endif
//____________________________________________________________________
//
// EOF
//
