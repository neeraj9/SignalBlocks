//
// $Id: divide_difference_base.hh,v 1.6 2005-05-31 21:02:22 cholm Exp $ 
//  
//  gslmm::divide_difference
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
#ifndef GSLMM_divide_difference_base
#define GSLMM_divide_difference_base

/** @file   divide_difference_base.hh
    @author Christian Holm
    @date   Sat Mar 01 12:56:22 2003
    @brief  Newtons divide difference representation of a polynomial */

#ifndef GSLMM_polynomial
#include <gslmm/polynomial/polynomial.hh>
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

namespace gslmm
{
  /** @class divide_difference divide_difference_base.hh <gslmm/polynomial/divide_difference_base.hh>
      @brief Newtons divide difference representation of a polynomial.
      @ingroup polynomial
   */
  template <typename Value>
  class divide_difference : public polynomial<Value> 
  {
  public:
    /// The type of the return values. 
    typedef typename polynomial<Value>::value_type    value_type;
    /// The type of the argument values. 
    typedef typename polynomial<Value>::argument_type argument_type;
  protected:
    /// The coefficents of the polynomial 
    std::vector<value_type> _xa;
  public: 
    /** Constructor. */
    divide_difference(const value_type* xa, const value_type* ya, size_t n);
    /** Constructor. */
    divide_difference(const std::vector<value_type>& xa, 
		      const std::vector<value_type>& ya);
    /** Constructor. */
    divide_difference(const value_type xa0,   const value_type ya0,   
		      const value_type xa1=0, const value_type ya1=0, 
		      const value_type xa2=0, const value_type ya2=0, 
		      const value_type xa3=0, const value_type ya3=0, 
		      const value_type xa4=0, const value_type ya4=0,
		      const value_type xa5=0, const value_type ya5=0,
		      const value_type xa6=0, const value_type ya6=0,
		      const value_type xa7=0, const value_type ya7=0,
		      const value_type xa8=0, const value_type ya8=0,
		      const value_type xa9=0, const value_type ya9=0);
    /** Destructor. */
    virtual ~divide_difference() {}

    /** Evaluate the polynomial */
    virtual value_type evaluate(const argument_type& x);

    /** Initialise The Divide Difference Object. */
    void initialize(const value_type* xa, const value_type* ya, size_t n);
    /** Initialise The Divide Difference Object. */
    void initialize(const std::vector<value_type>& xa, 
		    const std::vector<value_type>& ya);
    /** Initialise The Divide Difference Object. */
    void initialize(const value_type xa0,   const value_type ya0,   
		    const value_type xa1=0, const value_type ya1=0, 
		    const value_type xa2=0, const value_type ya2=0, 
		    const value_type xa3=0, const value_type ya3=0, 
		    const value_type xa4=0, const value_type ya4=0,
		    const value_type xa5=0, const value_type ya5=0,
		    const value_type xa6=0, const value_type ya6=0,
		    const value_type xa7=0, const value_type ya7=0,
		    const value_type xa8=0, const value_type ya8=0,
		    const value_type xa9=0, const value_type ya9=0);

    /** Set the coefficents of the polynomial. */
    virtual const std::vector<value_type>& coefficents(const value_type* p,
						       size_t n)
    { 
      return polynomial<value_type>::_p; 
    }
    
    /** Set the coefficents of the polynomial. */
    virtual const std::vector<value_type>& coefficents(const 
						       std::vector<value_type>&
						       p) 
    { 
      return polynomial<value_type>::_p; 
    }
    
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
						       const value_type p9=0) 
    { 
      return polynomial<value_type>::_p; 
    }

    /** Get the coefficents */
    virtual const std::vector<value_type> coefficents() const 
    { 
      return polynomial<value_type>::_p; 
    }
    /** Set a coefficent */ 
    virtual value_type coefficent(size_t i, const value_type& c) 
    { 
      return polynomial<value_type>::_p[i]; 
    }
    /** Get a coefficent */
    virtual value_type coefficent(size_t i) const 
    { 
      return polynomial<value_type>::_p[i]; 
    }
    
    /** Get the dd array. */
    // const std::vector<double> dd() const 
    const value_type* dd() const 
    { 
      return &(polynomial<value_type>::_p[0]); 
    } 
    /** Get the xa array. */
    // const std::vector<double> xa() const { return _xa; } 
    const value_type* xa() const { return &(_xa[0]); } 

    /** Get the size of the polynomial */
    size_t size() const 
    { 
      return polynomial<value_type>::_p.size(); 
    }
  };
  //__________________________________________________________________
  template <typename Value>
  inline
  divide_difference<Value>::divide_difference(const value_type* xa, 
					      const value_type* ya, 
					      size_t n) 
    : _xa()
  {
    initialize(xa,ya,n);
  }
  //__________________________________________________________________
  template <typename Value>
  inline 
  divide_difference<Value>::divide_difference(const value_type xa0,   
					      const value_type ya0,   
					      const value_type xa1, 
					      const value_type ya1, 
					      const value_type xa2, 
					      const value_type ya2, 
					      const value_type xa3, 
					      const value_type ya3, 
					      const value_type xa4, 
					      const value_type ya4,
					      const value_type xa5, 
					      const value_type ya5,
					      const value_type xa6, 
					      const value_type ya6,
					      const value_type xa7, 
					      const value_type ya7,
					      const value_type xa8, 
					      const value_type ya8,
					      const value_type xa9, 
					      const value_type ya9)  
    : _xa()
  {
    initialize(xa0, ya0, xa1, ya1, xa2, ya2, xa3, ya3, xa4, ya4,
	       xa5, ya5, xa6, ya6, xa7, ya7, xa8, ya8, xa9, ya9);
  }
  //__________________________________________________________________
  template <typename Value>
  inline
  divide_difference<Value>::divide_difference(const std::vector<value_type>& 
					      xa,  
					      const std::vector<value_type>& 
					      ya) 
    : _xa()
  {
    initialize(xa, ya);
  }
  //__________________________________________________________________
  template <typename Value>
  inline typename divide_difference<Value>::value_type
  divide_difference<Value>::evaluate(const argument_type& x) 
  {
    value_type y = polynomial<value_type>::_p[size() - 1];
    for (size_t i = size(); i--;) 
      y = polynomial<value_type>::_p[i] + (x - _xa[i]) * y;
    return y;
  }
  //__________________________________________________________________
  template <typename Value>
  inline void
  divide_difference<Value>::initialize(const value_type* xa, 
				       const value_type* ya, 
				       size_t n) 
  {
    polynomial<value_type>::_p.resize(n);
    _xa.resize(n);
    polynomial<value_type>::_p[0] = ya[0];
    _xa[0] = xa[0];
    for (size_t i = n - 1; i >= 1; i--) {
      polynomial<value_type>::_p[i] = (ya[i] - ya[i-1]) / (xa[i] - xa[i-1]);
      _xa[i] = xa[i];
    }
    for (size_t i = 2; i < n; i++) {
      for (size_t j = n - 1; j >= i; j--) 
	polynomial<value_type>::_p[j] = 
	  (polynomial<value_type>::_p[j] - polynomial<value_type>::_p[j-1]) 
	  / (_xa[j] - _xa[j-1]);
    }
  }
  //__________________________________________________________________
  template <typename Value>
  inline void
  divide_difference<Value>::initialize(const value_type xa0,   
				       const value_type ya0,   
				       const value_type xa1, 
				       const value_type ya1, 
				       const value_type xa2, 
				       const value_type ya2, 
				       const value_type xa3, 
				       const value_type ya3, 
				       const value_type xa4, 
				       const value_type ya4,
				       const value_type xa5, 
				       const value_type ya5,
				       const value_type xa6, 
				       const value_type ya6,
				       const value_type xa7, 
				       const value_type ya7,
				       const value_type xa8, 
				       const value_type ya8,
				       const value_type xa9, 
				       const value_type ya9)  
  {
    _xa.resize(10);   std::vector<value_type> ya[10];
    _xa[0] = xa0;     ya[0] = ya0;
    _xa[1] = xa1;     ya[1] = ya1;
    _xa[2] = xa2;     ya[2] = ya2;
    _xa[3] = xa3;     ya[3] = ya3;
    _xa[4] = xa4;     ya[4] = ya4;
    _xa[5] = xa5;     ya[5] = ya5;
    _xa[6] = xa6;     ya[6] = ya6;
    _xa[7] = xa7;     ya[7] = ya7;
    _xa[8] = xa8;     ya[8] = ya8;
    _xa[9] = xa9;     ya[9] = ya9;
    size_t d = 9;
    for (size_t i = 9; i >= 0; i++, d--) 
      if (_xa[i] != 0 && ya[i] != 0) break;
    initialize(_xa, ya, d);
  }
  //__________________________________________________________________
  template <typename Value>
  inline void
  divide_difference<Value>::initialize(const std::vector<value_type>& xa,  
				       const std::vector<value_type>& ya) 
  {
    initialize(&(xa[0]), &(ya[0]), min(xa.size(), ya.size()));
  }
}

#endif
//____________________________________________________________________
//
// EOF
//
