//
// $Id: function_1d.hh,v 1.8 2006-02-08 00:07:15 cholm Exp $ 
//  
//  gslmm::function_1d
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
#ifndef GSLMM_function_1d
#define GSLMM_function_1d
#ifndef GSLMM_function
#include <gslmm/function/function_base.hh>
#endif
#ifndef __GSL_MATH_H__
#include <gsl/gsl_math.h>
#endif

/** @file   function/function_1d.hh
    @author Christian Holm
    @date   Fri Feb 28 23:24:46 2003
    @brief  A 1D generalised function */

namespace gslmm
{
  /** @class function_1d function/function_1d.hh <gslmm/function/function_1d.hh>
      @brief A 1D generalised function.
      @ingroup function
      @param Type The argument and return type 
   */
  template <typename Type>
  class function_1d : public function_base<Type,Type>
  {};

  // double f_helper(double x, void* p);
  // double df_helper(double x, void* p);
  // void   fdf_helper(double x, void* p, double* f, double* df);

  /** @brief A 1D generalised function - double specialisation. */
  template <>
  class function_1d<double> : public function_base<double,double>
  {
  protected:
    /// A union of types
    union {
      /// A simple function 
      gsl_function     _f;
      /// A function that also has the derivative 
      gsl_function_fdf _fdf;
    };

    /**@{*/
    /** @name Helper functions for function_1d<double> */
    /** Helper function.  Forwards calls to object passed as the
        second parameter (cast to type @c void).  
        @param x The @f$ x@f$ value to evaluate the function at. 
	@param p Pointer to object to evaluate (cast to type @c void)
	@return The result of 
	@c static_cast<function_1d<double>*>(p)->evaluate(x) */
    friend double f_helper(double x, void* p) {
      function_1d<double>* self = static_cast<function_1d<double>*>(p);
      return self->evaluate(x);
    }
    /** Helper function.  Forwards calls to object passed as the
        second parameter (cast to type @c void).  
        @param x The @f$ x@f$ value to evaluate the derivative of the
	function at.  
	@param p Pointer to object to evaluate (cast to type @c void)
	@return The result of 
	@c static_cast<function_1d<double>*>(p)->derivative(x) */
    friend double df_helper(double x, void* p) {
      function_1d<double>* self = static_cast<function_1d<double>*>(p);
      return self->derivative(x);
    }
    /** Helper function.  Forwards calls to object passed as the
        second parameter (cast to type @c void).  
        @param x The @f$ x@f$ value to function and the derivative of
	the function at.  
	@param p Pointer to object to evaluate (cast to type @c void)
	@param f On output, the result of 
	@c static_cast<function_1d<double>*>(p)->evaluate(x) 
	@param df On  output, the result of 
	@c static_cast<function_1d<double>*>(p)->derivative(x) 
    */
    friend void fdf_helper(double x, void* p, double* f, double* df) {
      function_1d<double>* self = static_cast<function_1d<double>*>(p);
      *f  = self->evaluate(x);
      *df = self->derivative(x);
    }
    /**@}*/
  public:
    /** Constructor. */
    function_1d() : _fdf()
    {
      _f.function = f_helper;
      _f.params   = static_cast<void*>(this);
      _fdf.f      = f_helper;
      _fdf.df     = df_helper;
      _fdf.fdf    = fdf_helper;
      _fdf.params = static_cast<void*>(this);
    }
    
    /** Destructor. */
    virtual ~function_1d() {}
  };
}

#endif
//____________________________________________________________________
//
// EOF
//
