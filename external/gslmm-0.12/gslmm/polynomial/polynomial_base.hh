//
// $Id: polynomial_base.hh,v 1.5 2004-11-06 00:49:46 cholm Exp $ 
//  
//  gslmm::polynomial
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
#ifndef GSLMM_polynomial_base
#define GSLMM_polynomial_base
// #ifndef GSLMM_function_1d
// #include <gslmm/function/function_1d.hh>
// #endif
#ifndef GSLMM_utility
#include <gslmm/math/utility.hh>
#endif
#ifndef __VECTOR__
#include <vector>
#endif
#ifndef __GSL_MATH_H__
#include <gsl/gsl_math.h>
#endif

/** @file   polynomial_base.hh
    @author Christian Holm
    @date   Sat Mar 01 00:33:46 2003
    @brief  A polynomial */
/** @defgroup polynomial Polynomials */

namespace gslmm
{
  /** @class polynomial polynomial_base.hh <gslmm/polynomial/polynomial_base.hh>
      @brief A polynomial in 1 dimension. 
      @ingroup polynomial
  */
  template <typename Value>
  class polynomial // : public function_1d<Value>
  {
  public:
    /// The type of the return values. 
    typedef Value value_type;
    /// The type of the argument values. 
    typedef Value argument_type;
  protected:
    /// The coefficents of the polynomial 
    std::vector<value_type> _p;
  public:
    /** Constructor. */
    polynomial() : _p(0) {}
    /** Constructor. */
    polynomial(const value_type* p, size_t n) : _p(n) { coefficents(p,n); }
    /** Constructor. */
    polynomial(const std::vector<value_type>& p) : _p(p) { }
    /** Constructor. */
    polynomial(const value_type p0,   const value_type p1,   
	       const value_type p2=0, const value_type p3=0, 
	       const value_type p4=0, const value_type p5=0, 
	       const value_type p6=0, const value_type p7=0, 
	       const value_type p8=0, const value_type p9=0);
    /** Destructor. */
    virtual ~polynomial() {}
    /** Evaluate the polynomial */
    virtual value_type evaluate(const argument_type& x);
    /** Evaluate the derivative of the polynomial */
    virtual value_type derivative(const argument_type& x);
    /** Get the size of the polynomial.  That is, the size
	of @f$ p(x) = p_0 + p_1x + p_2x^2 @f$ is 2. */
    virtual size_t size() const { return _p.size() - 1; }
    /** Set the coefficents of the polynomial. */
    virtual const std::vector<value_type>& coefficents(const value_type* p, 
						       size_t n);
    /** Set the coefficents of the polynomial. */
    virtual const std::vector<value_type>& coefficents(const 
						       std::vector<value_type>&
						       p);
    /** Set the coefficents of the polynomial. */
    virtual const std::vector<value_type>& coefficents(const value_type p0,   
						       const value_type p1,   
						       const value_type p2=0, 
						       const value_type p3=0, 
						       const value_type p4=0, 
						       const value_type p5=0, 
						       const value_type p6=0, 
						       const value_type p7=0, 
						       const value_type p8=0, 
						       const value_type p9=0);
    /** Get the coefficents */
    virtual const std::vector<value_type> coefficents() const { return _p; }
    /** Set a coefficent */ 
    virtual value_type coefficent(size_t i, const value_type& c) { 
      return _p[i] = c; }
    /** Get a coefficent */
    virtual value_type coefficent(size_t i) const { return _p[i]; }
  };   

  //__________________________________________________________________
  template <typename V> 
  inline
  polynomial<V>::polynomial(const value_type p0, const value_type p1,   
			    const value_type p2, const value_type p3, 
			    const value_type p4, const value_type p5, 
			    const value_type p6, const value_type p7, 
			    const value_type p8, const value_type p9)
    : _p(1)
  {
    coefficents(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
  }
  //__________________________________________________________________
  template <typename V> 
  inline typename polynomial<V>::value_type
  polynomial<V>::evaluate(const argument_type& x) 
  {
    value_type v = 0;
    for (size_t i = 0; i < _p.size(); i++) v += _p[i] * pow_int(x, i);
    return v;
  }
  //__________________________________________________________________
  template <typename V> 
  inline typename polynomial<V>::value_type 
  polynomial<V>::derivative(const argument_type& x) 
  {
    value_type v = 0;
    for (size_t i = 1; i < _p.size(); i++) v += _p[i] * pow_int(x, i-1);
    return v;
  }
  //__________________________________________________________________
  template <typename V> 
  inline const std::vector<typename polynomial<V>::value_type>& 
  polynomial<V>::coefficents(const value_type* p, size_t n) 
  { 
    _p.resize(n,0);
    for (size_t i = 0; i < n; i++)  _p[i] = p[i];
    return _p;
  }
  //__________________________________________________________________
  template <typename V> 
  inline const std::vector<typename polynomial<V>::value_type>& 
  polynomial<V>::coefficents(const std::vector<value_type>&  p) 
  { 
    _p = p;
    return _p;
  }
  //__________________________________________________________________
  template <typename V> 
  inline const std::vector<typename polynomial<V>::value_type>& 
  polynomial<V>::coefficents(const value_type p0, const value_type p1,   
			     const value_type p2, const value_type p3, 
			     const value_type p4, const value_type p5, 
			     const value_type p6, const value_type p7, 
			     const value_type p8, const value_type p9)
  {
    std::vector<value_type> p(10);
    p[0] = p0;  p[1] = p1; p[2] = p2; p[3] = p3; p[4] = p4;
    p[5] = p5;  p[6] = p6; p[7] = p7; p[8] = p8; p[9] = p9;
    size_t i = 9, d = 0;
    for (i = 9; i >= 0; i--, d++) if (p[i] != 0) break;
    return coefficents(&(p[0]), d+1);
  }
}


#endif
//____________________________________________________________________
//
// EOF
//
