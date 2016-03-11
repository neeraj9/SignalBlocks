//
// $Id: ieee_representation.hh,v 1.6 2006-02-08 00:07:28 cholm Exp $ 
//  
//  gslmm::ieee_representation
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
#ifndef GSLMM_ieee_representation
#define GSLMM_ieee_representation

/** @file   ieee_representation.hh
    @author Christian Holm
    @date   Wed Mar 05 12:00:13 2003
    @brief  Represetation of real values numbers */

#ifndef __GSL_IEEE_UTILS_H__
#include <gsl/gsl_ieee_utils.h>
#endif
#ifndef __ALGORITHM__
#include <algorithm>
#endif
#ifndef __BITSET__
#include <bitset>
#endif

namespace gslmm
{
  /** @class ieee_representation ieee_representation.hh <gslmm/ieee/ieee_representation.hh>
      Represetation of real values numbers. Base template. 
      @ingroup ieee 
  */
  template <typename Real>
  class ieee_representation {};
  

  //==================================================================
  /** Represetation of real values numbers. @c double specialisation 
      @ingroup ieee 
  */
  template <>
  class ieee_representation<double>
  {
  public:
    /// 
    typedef double real_type;
    /// 
    enum type {
      not_a_number = GSL_IEEE_TYPE_NAN, 
      infinity     = GSL_IEEE_TYPE_INF,
      normal       = GSL_IEEE_TYPE_NORMAL,
      denormal     = GSL_IEEE_TYPE_DENORMAL,
      zero         = GSL_IEEE_TYPE_ZERO
    };
  protected:
    /// 
    gsl_ieee_double_rep _rep;
    /// 
    real_type& _number;
  public:
    /** Constructor. */
    ieee_representation(real_type& x);
    /** Destructor. */
    virtual ~ieee_representation() {}
    /** Get the sign  bit
	@return 0 for positive, and 1 for negative. */
    int sign() const { return _rep.sign; }
    /** Get the bits
	@return an bitset of teh bits. */
    std::bitset<52> mantissa() const;
    /** Get the exponent
	@return the exponent of the number. */
    int exponent() const { return _rep.exponent; }
    /** Get the type.  
	@return one of gslmm::ieee_representation::not_a_number, 
	gslmm::ieee_representation::infinity, 
	gslmm::ieee_representation::normal,
	gslmm::ieee_representation::zero,
	gslmm::ieee_representation::denormal */
    int type() const;
    /** Get the number. */
    real_type& number() { return _number; }
  };
  //__________________________________________________________________
  ieee_representation<double>::ieee_representation(real_type& x) 
    : _rep(), _number(x)
  {
    gsl_ieee_double_to_rep(&x, &_rep);
  }
  //__________________________________________________________________
  std::bitset<52> ieee_representation<double>::mantissa() const 
  {
    return std::bitset<52>(std::string(_rep.mantissa)); 
  }
  //__________________________________________________________________
  int ieee_representation<double>::type() const
  {
    switch (_rep.type) {
    case GSL_IEEE_TYPE_NAN:          return not_a_number; 
    case GSL_IEEE_TYPE_INF:          return infinity;
    case GSL_IEEE_TYPE_NORMAL:       return normal;
    case GSL_IEEE_TYPE_DENORMAL:     return denormal;
    case GSL_IEEE_TYPE_ZERO:         return zero;
    }
    return normal;
  }

  //==================================================================
  /** Represetation of real values numbers. @c float specialisation 
      @ingroup ieee 
  */
  template <>
  class ieee_representation<float>
  {
  public:
    /// 
    typedef float real_type;
    /// 
    enum {
      not_a_number = GSL_IEEE_TYPE_NAN, 
      infinity     = GSL_IEEE_TYPE_INF,
      normal       = GSL_IEEE_TYPE_NORMAL,
      denormal     = GSL_IEEE_TYPE_DENORMAL,
      zero         = GSL_IEEE_TYPE_ZERO
    };
  protected:
    /// 
    gsl_ieee_float_rep _rep;
    /// The number 
    real_type& _number;
  public:
    /** Constructor. */
    ieee_representation(real_type& x);
    /** Destructor. */
    virtual ~ieee_representation() {}
    /** Get the sign  bit
	@return 0 for positive, and 1 for negative. */
    int sign() const { return _rep.sign; }
    /** Get the bits
	@return a bit set of the bits. */
    std::bitset<23> mantissa() const;
    /** Get the exponent
	@return the exponent of the number. */
    int exponent() const { return _rep.exponent; }
    /** Get the type.  
	@return one of gslmm::ieee_representation::not_a_number, 
	gslmm::ieee_representation::infinity, 
	gslmm::ieee_representation::normal,
	gslmm::ieee_representation::zero,
	gslmm::ieee_representation::denormal */
    int type() const;
    /** Get the number. */
    real_type& number() { return _number; }
  };
  //__________________________________________________________________
  ieee_representation<float>::ieee_representation(real_type& x) 
    : _rep(), _number(x)
  {
    gsl_ieee_float_to_rep(&x, &_rep);
  }
  //__________________________________________________________________
  std::bitset<23> ieee_representation<float>::mantissa() const 
  {
    return std::bitset<23>(std::string(_rep.mantissa)); 
  }
  //__________________________________________________________________
  int ieee_representation<float>::type() const
  {
    switch (_rep.type) {
    case GSL_IEEE_TYPE_NAN:          return not_a_number; 
    case GSL_IEEE_TYPE_INF:          return infinity;
    case GSL_IEEE_TYPE_NORMAL:       return normal;
    case GSL_IEEE_TYPE_DENORMAL:     return denormal;
    case GSL_IEEE_TYPE_ZERO:         return zero;
    }
    return normal;
  }
  

}

#endif
//____________________________________________________________________
//
// EOF
//
