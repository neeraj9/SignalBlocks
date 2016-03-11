//
// $Id: function_base.hh,v 1.7 2006-05-01 14:26:02 cholm Exp $ 
//  
//  gslmm::function_base
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
#ifndef GSLMM_function_base
#define GSLMM_function_base

/** @file   function/function_base.hh
    @author Christian Holm
    @date   Fri Feb 28 20:23:35 2003
    @brief  Abstract base class for functions of all kinds  */

/** @defgroup function Functions */

namespace gslmm
{
  /** @class function_base function_base.hh <gslmm/function/function_base.hh>
      @brief Abstract base class for functions of all kinds.

      The sub-classes should at a minimum override the member
      functions function_base::evaluate to return @f$ f(x)@f$.  Per
      default, the member function function_base::derivative does a
      numerical differentation, based on an increase of
      gslmm::type_trait<value_type>::epsilon() in the argument, and then
      calculates the Newton cofficient @f$ (f(x+dx)-f(x))/dx @f$.
      Derived classes should overload this if the derivative can be
      better determined. 
      @ingroup function
      @param Value The value type.  That is, the type of the
      evaluation of the function @f$ f(x)@f$. 
      @param Argument The argument type. That is, the type of @f$
      x@f$. 
      @param Derive The type of the derivative of the function.  It
      can be straight forward value, or it can be a Jacobian matrix,
      or similar. 
      @param Error The type of error estimates. 
      @param Scale The type of scale factors in the result. 
      @todo I'm not sure I really want this class, as must of the
      interfaces will deal with template arguments, rather than
      sub-classing of a specific base class. 
   */
  template <typename Value, typename Argument, typename Derive=Value,
	    typename Error=Value, typename Scale=Value>
  class function_base
  {
  public:
    /// The type of the function value
    typedef Value value_type;
    /// The type of the function argument
    typedef Argument argument_type;
    /// The type of error estimates 
    typedef Error error_type;
    /// The type of scaling factors 
    typedef Scale scale_type;
    /// The type of the deriviative evaluation
    typedef Derive derive_type;
  private:
  protected:
  public:
    /** Constructor. */
    function_base() {}
    /** Destructor. */
    virtual ~function_base() {}

    //@{
    /// @name Evaluation
    /** Evaluate this function. 
	@param x The point to evaluate it at. 
	@return The function value at $p x .*/
    virtual value_type evaluate(const argument_type& x) = 0;
    /** Evaluate this function, with error estimate.  
	@param x The point to evaluate it at. 
	@param e The error estimate. 
	@return The function value at $p x .*/
    virtual value_type evaluate(const argument_type& x, error_type& e);
    /** Evaluate this function, with error estimate and scaling.
	@param x The point to evaluate it at. 
	@param s The  scaling factor.
	@param e The error estimate. 
	@return The function value at $p x .*/
    virtual value_type evaluate(const argument_type& x, error_type& e,
				scale_type& s);
    //@}

    //@{
    /// @name Evaluation of derivative
    /** Evaluate derivative of this function. 
	@param x The point to evaluate it at. 
	@return The function value at $p x .*/
    virtual derive_type derivative(const argument_type& x) = 0;
    /** Evaluate derivative of this function, with error estimate.  
	@param x The point to evaluate it at. 
	@param e The error estimate. 
	@return The function value at $p x .*/
    virtual derive_type derivative(const argument_type& x, error_type& e);
    /** Evaluate derivative of this function, with error estimate and scaling.
	@param x The point to evaluate it at. 
	@param s The  scaling factor.
	@param e The error estimate. 
	@return The function value at $p x .*/
    virtual derive_type derivative(const argument_type& x, error_type& e,
				  scale_type& s);
    //@}
  };
  //__________________________________________________________________
  template <typename V, typename A, typename D, typename E, typename S>
  inline V
  function_base<V,A,D,E,S>::evaluate(const argument_type& x, error_type& e) 
  {
    e = 0;
    return evaluate(x);
  }
  //__________________________________________________________________
  template <typename V, typename A, typename D, typename E, typename S>
  inline V
  function_base<V,A,D,E,S>::evaluate(const argument_type& x, error_type& e, 
				scale_type& s) 
  {
    e = 0;
    s = 0;
    return evaluate(x);
  }
  //__________________________________________________________________
  template <typename V, typename A, typename D, typename E, typename S>
  inline D
  function_base<V,A,D,E,S>::derivative(const argument_type& x, error_type& e) 
  {
    e = 0;
    return derivative(x);
  }
  //__________________________________________________________________
  template <typename V, typename A, typename D, typename E, typename S>
  inline D
  function_base<V,A,D,E,S>::derivative(const argument_type& x, error_type& e, 
				  scale_type& s) 
  {
    e = 0;
    s = 0;
    return derivative(x);
  }
}

#endif
//____________________________________________________________________
//
// EOF
//
