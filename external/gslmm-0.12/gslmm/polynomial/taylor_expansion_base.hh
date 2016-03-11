//
// $Id: taylor_expansion_base.hh,v 1.4 2003-03-22 04:35:40 cholm Exp $ 
//  
//  gslmm::taylor_expansion
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
#ifndef GSLMM_taylor_expansion_base
#define GSLMM_taylor_expansion_base
#ifndef GSLMM_polynomial
#include <gslmm/function/polynomial.hh>
#endif
#ifndef GSLMM_utility
#include <gslmm/math/utility.hh>
#endif
#ifndef __VECTOR__
#include <vector>
#endif
#ifndef __GSL_MATH_H__
#include <gsl/gsl_math.h>
#endif

/** @file   polynomial/taylor_expansion_base.hh
    @author Christian Holm
    @date   Sat Mar 08 18:31:32 2003
    @brief  Representation of a tayler expansion */


namespace gslmm 
{
  /** @class taylor_expansion  taylor_expansion_base.hh <gslmm/polynomial/taylor_expansion_base.hh>
      @brief Representation of a tayler expansion - base template. 
      @ingroup polynomial
   */
  template <typename Value>
  class taylor_expansion : public polynomial<Value>
  {
  public:
    /// The type of the return values. 
    typedef typename polynomial<Value>::value_type    value_type;
    /// The type of the argument values. 
    typedef typename polynomial<Value>::argument_type argument_type;
  protected:
    /// The value expanded about 
    argument_type _x0;
  public:
    /** Constructor. */
    taylor_expansion(argument_type x0, const value_type* p, size_t n);
    /** Constructor. */
    taylor_expansion(argument_type x0, const std::vector<value_type>& p);
    /** Constructor. */
    taylor_expansion(argument_type x0, 
		     const value_type p0,   
		     const value_type p1,   
		     const value_type p2=0, 
		     const value_type p3=0, 
		     const value_type p4=0, 
		     const value_type p5=0, 
		     const value_type p6=0, 
		     const value_type p7=0,
		     const value_type p8=0, 
		     const value_type p9=0);
    /** Destructor. */
    virtual ~taylor_expansion() {}
 
    /** Get the expansion point. */
    argument_type expansion_point() const { return _x0; }
    /** Set the expansion point. */
    argument_type expansion_point(argument_type x0) { return _x0 = x0; }

    /** Evaluate the polynomial */
    value_type evaluate(const argument_type& x);  
  };
  //__________________________________________________________________
  template <typename Value>
  inline
  taylor_expansion<Value>::taylor_expansion(const argument_type x0, 
					    const value_type p0, 
					    const value_type p1,   
					    const value_type p2, 
					    const value_type p3, 
					    const value_type p4, 
					    const value_type p5, 
					    const value_type p6, 
					    const value_type p7, 
					    const value_type p8, 
					    const value_type p9)
    : polynomial<Value>(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9), _x0(x0)
  {}
  //__________________________________________________________________
  template <typename Value>
  inline
  taylor_expansion<Value>::taylor_expansion(const argument_type x0, 
					    const value_type* p,
					    size_t n) 
    : polynomial<Value>(p, n), _x0(x0)
  {}
  //__________________________________________________________________
  template <typename Value>
  inline
  taylor_expansion<Value>::taylor_expansion(const argument_type x0, 
					    const std::vector<value_type>& p) 
    : polynomial<Value>(p), _x0(x0)
  {}
  //__________________________________________________________________
  template <typename Value>
  inline typename taylor_expansion<Value>::value_type
  taylor_expansion<Value>::evaluate(const argument_type& x) 
  {
    return polynomial<value_type>::evaluate(x - _x0);
  }
}

#endif
//____________________________________________________________________
//
// EOF
//
