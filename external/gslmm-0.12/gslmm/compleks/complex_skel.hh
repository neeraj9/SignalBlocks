//
// $Id: complex_skel.hh,v 1.17 2006-05-01 14:24:38 cholm Exp $ 
//  
// WARNING: automatically generated by @progname@
//          options: @skel@, @type@.  
//          Do not edit this file.
//
//  gslmm::complex@name@
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
/** @file   complex@name@.hh
    @author Christian Holm
    @date   Fri Sep 20 13:16:42 2002
    @brief  Stubs for declarations of gslmm::complex<T>
    specialisations. 
*/
#line 33 "complex_skel.hh"
#ifndef GSLMM_compleks_complex@name@
#define GSLMM_compleks_complex@name@
#ifndef __GSL_COMPLEX_H__
# include <gsl/gsl_complex.h>
#endif
#ifndef __GSL_COMPLEX_MATH_H__
# include <gsl/gsl_complex_math.h>
#endif
#ifndef GSLMM_compleks_complex_base
# include <gslmm/compleks/complex_base.hh>
#endif 
#ifndef __IOMANIP__
# include <iomanip>
#endif
 
namespace gslmm 
{ 
  /** @brief @c @type@ Specialisation of gslmm::complex<T>. 
      @ingroup complex
  */
  template <> 
  class complex<@type@>  
  { 
  public:
    /** The value type  */
    typedef @type@ value_type;
    /** Type of the low_level data  */
    typedef gsl_complex@low@ low_type;
    /** Trait type */
    typedef type_trait<complex<@type@> > trait_type;
    /** The imaginary unit */
    static const complex<@type@>& i();
    
    /** The data */
    gsl_complex@low@* _complex; 
    // gsl_complex@low@  _complex; 
    bool              _owner;
    /** Constructor.  */
    complex(const value_type& real=0, const value_type& imag=0,
	    bool polar=false)  
    {  
      _complex = (low_type*)malloc(sizeof(gsl_complex@low@));
      _owner = true;
      assign(real, imag, polar); 
    } 
    /** Conversion constructor from std::complex.  */
    complex(const std::complex<value_type>& z) 
    { 
      _complex = (low_type*)malloc(sizeof(gsl_complex@low@));
      _owner   = true;
      GSL_SET_COMPLEX(_complex, z.real(), z.imag()); 
    }  
    /** Copy constructor.  */
    complex(const complex<value_type>& z, bool owner=false) 
      : _complex(z._complex), _owner(z._owner)
    {
      // assign(GSL_REAL(z._complex), GSL_IMAG(z._complex));
      _owner = owner;
      if (_owner) {
	_complex = (low_type*)malloc(sizeof(gsl_complex@low@));
	GSL_SET_COMPLEX(_complex, z.real(), z.imag()); 
      }
    }
    /** Conversion Constructor.  */
    complex(const gsl_complex@low@* z, bool owner=false); 
    /** Conversion constructor.  */
    complex(const gsl_complex@low@& z, bool owner=false); 
    /** Destructor */
    virtual ~complex() {  if (_owner) free(_complex); }
    /** Check if this is purely real  */
    bool is_real() const { return imag() == 0; } 
    /** Check if this is purely complex  */
    bool is_complex() const { return real() == 0; } 
    /** Assign this number  */
    void assign(const value_type& real=0, const value_type& imag=0,
		bool polar=false); 
    /** Set the real part  */
    const value_type& real() const
    {
      return _complex->dat[0];
    } 
    /** Set imaginary partt  */
    const value_type& imag() const
    {
      return _complex->dat[1];
    } 
    /** Get the real part  */
    value_type& real()
    { 
      return _complex->dat[0];
    }
    /** Get the imaginary part */
    value_type& imag() 
    { 
      return _complex->dat[1];
    }
    /** Get angle */
    value_type arg() const 
    { 
      value_type ret = gsl_complex@low@_arg(*_complex); 	   
      return ret;
    }
    /** Get length */
    value_type abs() const 
    { 
      return gsl_complex@low@_abs(*_complex); 
    }
    /** Get the square of the length */
    value_type square_abs() const 
    { 
      return gsl_complex@low@_abs2(*_complex); 
    }
    /** Get the log of the length */
    value_type log_of_abs() const 
    { 
      return gsl_complex@low@_logabs(*_complex); 
    }
    /** Assign */
    complex<value_type>& operator=(const gslmm::complex<value_type>& z) 
    { 
      GSL_SET_COMPLEX(_complex, z.real(), z.imag());
      return *this; 
    }
    /** Assign */
    complex<value_type>& operator=(const value_type& x) 
    { 
      GSL_SET_COMPLEX(_complex, x, 0); 
      return *this; 
    }
    /** Assign */
    complex<value_type>& operator=(const std::complex<value_type>& z) 
    { 
      GSL_SET_COMPLEX(_complex, z.real(), z.imag()); 
      return *this; 
    }
    /** compare */
    bool operator==(const complex<value_type>& z1) const 
    { 
      return GSL_COMPLEX_EQ(*_complex, *(z1._complex)); 
    }
    /** compare */
    bool operator!=(const complex<value_type>& z1) const 
    { 
      return !(GSL_COMPLEX_EQ(*_complex, *(z1._complex))); 
    }
    /** Unary minus */
    complex<value_type>    operator-() const; 
    /** Addition operator */
    complex<value_type>    operator+(const complex<value_type>& z1); 
    /** Substraction operator */
    complex<value_type>    operator-(const complex<value_type>& z1); 
    /** Multiplication operator */
    complex<value_type>    operator*(const complex<value_type>& z1); 
    /** Division operator */
    complex<value_type>    operator/(const complex<value_type>& z1); 
    /** Addition assignment  */
    complex<value_type>&   operator+=(const complex<value_type>& z1); 
    /** Substraction asssignment */
    complex<value_type>&   operator-=(const complex<value_type>& z1); 
    /** Multiplication assignment */
    complex<value_type>&   operator*=(const complex<value_type>& z1); 
    /** Division assignment */
    complex<value_type>&   operator/=(const complex<value_type>& z1); 
    /** Addition operator  */
    complex<value_type>    operator+(const value_type& a); 
    /** Substraction asssignment */
    complex<value_type>    operator-(const value_type& a); 
    /** Multiplication operator */
    complex<value_type>    operator*(const value_type& a); 
    /** Division operator */
    complex<value_type>    operator/(const value_type& a); 
    /** Addition assignment  */
    complex<value_type>&   operator+=(const value_type& a); 
    /** Substraction asssignment */
    complex<value_type>&   operator-=(const value_type& a); 
    /** Multiplication assignment */
    complex<value_type>&   operator*=(const value_type& a); 
    /** Division assignment */
    complex<value_type>&   operator/=(const value_type& a); 
    /** Addition assignment  */
    void  add_to_real(const value_type& a);
    /** Substraction asssignment */
    void  subtract_from_real(const value_type& a); 
    /** Multiplication assignment */
    void  multiply_real(const value_type& a); 
    /** Division assignment */
    void  divide_real(const value_type& a); 
    /** Addition assignment  */
    void  add_to_imag(const value_type& a); 
    /** Substraction asssignment */
    void  subtract_from_imag(const value_type& a); 
    /** Multiplication assignment */
    void  multiply_imag(const value_type& a); 
    /** Division assignment */
    void  divide_imag(const value_type& a); 
    /** Return complex conjugate  */
    complex<value_type> conjugate() const; 
    /** Return 1 / z */
    complex<value_type> inverse() const; 
    /** Conversion  */
    std::complex<value_type> as_std() const 
    { 
      return std::complex<value_type>(real(), imag()); 
    } 
    /** Conversion  */
    gsl_complex@low@* as_c_type_ptr() const 
    { 
      return const_cast<gsl_complex@low@*>(_complex); 
    } 
    /** Conversion  */
    gsl_complex@low@& as_c_type() 
    { 
      return *_complex; 
    } 
    /** Conversion  */
    const gsl_complex@low@& as_c_type() const 
    { 
      return *_complex; 
    } 
    /** Conversion  */
    operator gsl_complex@low@& () 
    { 
      return *_complex; 
    } 
    /** Conversion  */
    operator const gsl_complex@low@& () const 
    { 
      return *_complex; 
    } 
  }; 

  //__________________________________________________________________
  // const complex<@type@>* complex<@type@>::_i = 0;

  //__________________________________________________________________
  inline const complex<@type@>&
  complex<@type@>::i()
  { 
    static complex<@type@> _i(0.,1.);
    return _i;
  }
  
  //__________________________________________________________________
  inline complex<@type@>::complex(const gsl_complex@low@* z, bool owner)
    : _owner(owner)
  { 
    if (_owner) {
      _complex = (low_type*)malloc(sizeof(gsl_complex@low@));      
      GSL_SET_COMPLEX(_complex, GSL_REAL(*z), GSL_IMAG(*z)); 
    }
    else
      _complex = const_cast<gsl_complex@low@*>(z);
  }

  //__________________________________________________________________
  inline complex<@type@>::complex(const gsl_complex@low@& z, bool owner)
    : _owner(owner)
  { 
    if (_owner) {
      _complex = (low_type*)malloc(sizeof(gsl_complex@low@));      
      GSL_SET_COMPLEX(_complex, GSL_REAL(z), GSL_IMAG(z)); 
    }
    else
      _complex = const_cast<gsl_complex@low@*>(&z);
  }

  //__________________________________________________________________
  inline void 
  complex<@type@>::assign(const value_type& real, const value_type& imag, 
			  bool polar) 
  { 
    if (polar) 
      *_complex = gsl_complex@low@_polar(real,imag); 
    else {
      GSL_SET_COMPLEX(_complex, real, imag);
      // *_complex = gsl_complex@low@_rect(real,imag); 
    }
  } 

  //__________________________________________________________________
  inline complex<@type@> 
  complex<@type@>::operator-() const 
  {
    gsl_complex@low@ t = gsl_complex@low@_negative(*_complex); 
    return complex<@type@>(&t); 
  }  

  //__________________________________________________________________
  inline complex<@type@>
  complex<@type@>::operator+(const complex<value_type>& z1) 
  { 
    low_type rl = gsl_complex@low@_add(*_complex,*(z1._complex)); 
    return complex<@type@>(rl,true);
  } 

  //__________________________________________________________________
  inline complex<@type@> 
  complex<@type@>::operator-(const complex<value_type>& z1) 
  { 
    low_type rl = gsl_complex@low@_sub(*_complex,*(z1._complex)); 
    return complex<@type@>(rl,true); 
  } 

  //__________________________________________________________________
  inline complex<@type@> 
  complex<@type@>::operator*(const complex<value_type>& z1) 
  { 
    low_type rl = gsl_complex@low@_mul(*_complex,(*z1._complex)); 
    complex<@type@> r(rl,true);
    return r; 
  } 

  //__________________________________________________________________
  inline complex<@type@> 
  complex<@type@>::operator/(const complex<value_type>& z1) 
  { 
    low_type rl = gsl_complex@low@_div(*_complex,*(z1._complex)); 
    return complex<@type@>(rl,true); 
  } 

  //__________________________________________________________________
  inline complex<@type@>& 
  complex<@type@>::operator+=(const complex<value_type>& z1) 
  { 
    *_complex = gsl_complex@low@_add(*_complex,*(z1._complex)); 
    return *this; 
  } 

  //__________________________________________________________________
  inline complex<@type@>& 
  complex<@type@>::operator-=(const complex<value_type>& z1) 
  { 
    *_complex = gsl_complex@low@_sub(*_complex,*(z1._complex)); 
    return *this; 
  } 

  //__________________________________________________________________
  inline complex<@type@>& 
  complex<@type@>::operator*=(const complex<value_type>& z1) 
  { 
    *_complex = gsl_complex@low@_mul(*_complex,(*z1._complex)); 
    return *this; 
  } 

  //__________________________________________________________________
  inline complex<@type@>& 
  complex<@type@>::operator/=(const complex<value_type>& z1) 
  { 
    *_complex = gsl_complex@low@_div(*_complex,*(z1._complex)); 
    return *this; 
  } 

  //__________________________________________________________________
  inline complex<@type@> 
  complex<@type@>::operator+(const value_type& a) 
  { 
    low_type rl = gsl_complex@low@_add_real(*_complex,a); 
    return complex<@type@>(rl,true);
  }

  //__________________________________________________________________
  inline complex<@type@>
  complex<@type@>::operator-(const value_type& a) 
  { 
    low_type rl = gsl_complex@low@_sub_real(*_complex,a); 
    return complex<@type@>(rl,true); 
  }

  //__________________________________________________________________
  inline complex<@type@> 
  complex<@type@>::operator*(const value_type& a) 
  { 
    low_type rl = gsl_complex@low@_mul_real(*_complex,a); 
    return complex<@type@>(rl,true); 
  }

  //__________________________________________________________________
  inline complex<@type@> 
  complex<@type@>::operator/(const value_type& a) 
  { 
    low_type rl = gsl_complex@low@_div_real(*_complex,a); 
    return complex<@type@>(rl,true); 
  }

  //__________________________________________________________________
  inline complex<@type@>& 
  complex<@type@>::operator+=(const value_type& a) 
  { 
    *_complex = gsl_complex@low@_add_real(*_complex,a); 
    return *this; 
  }

  //__________________________________________________________________
  inline complex<@type@>& 
  complex<@type@>::operator-=(const value_type& a) 
  { 
    *_complex = gsl_complex@low@_sub_real(*_complex,a); 
    return *this; 
  }

  //__________________________________________________________________
  inline complex<@type@>& 
  complex<@type@>::operator*=(const value_type& a) 
  { 
    *_complex = gsl_complex@low@_mul_real(*_complex,a); 
    return *this; 
  }

  //__________________________________________________________________
  inline complex<@type@>& 
  complex<@type@>::operator/=(const value_type& a) 
  { 
    *_complex = gsl_complex@low@_div_real(*_complex,a); 
    return *this; 
  }

  //__________________________________________________________________
  inline void 
  complex<@type@>::add_to_real(const value_type& a) 
  {
    GSL_SET_REAL(_complex, GSL_REAL(*_complex) + a);    
  } 
  
  //__________________________________________________________________
  inline void 
  complex<@type@>::subtract_from_real(const value_type& a) 
  {
    GSL_SET_REAL(_complex, GSL_REAL(*_complex) - a); 
  } 
  
  //__________________________________________________________________
  inline void 
  complex<@type@>::multiply_real(const value_type& a) 
  {
    GSL_SET_REAL(_complex, GSL_REAL(*_complex) * a); 
  } 
  
  //__________________________________________________________________
  inline void 
  complex<@type@>::divide_real(const value_type& a) 
  {
    GSL_SET_REAL(_complex, GSL_REAL(*_complex) / a); 
  } 

  //__________________________________________________________________
  inline void 
  complex<@type@>::add_to_imag(const value_type& a) 
  {
    GSL_SET_IMAG(_complex, GSL_IMAG(*_complex) + a); 
  } 

  //__________________________________________________________________
  inline void 
  complex<@type@>::subtract_from_imag(const value_type& a) 
  {
    GSL_SET_IMAG(_complex, GSL_IMAG(*_complex) - a); 
  } 

  //__________________________________________________________________
  inline void 
  complex<@type@>::multiply_imag(const value_type& a) 
  {
    GSL_SET_IMAG(_complex, GSL_IMAG(*_complex) * a); 
  } 

  //__________________________________________________________________
  inline void 
  complex<@type@>::divide_imag(const value_type& a) 
  {
    GSL_SET_IMAG(_complex, GSL_IMAG(*_complex) / a); 
  } 

  //__________________________________________________________________
  inline complex<@type@> 
  complex<@type@>::conjugate() const 
  {
    gsl_complex@low@ t = gsl_complex@low@_conjugate(*_complex); 
    gslmm::complex<@type@> ret(t, true);
    // std::cout << "z=" << *this << " zbar=" << ret << std::endl;
    return ret; 
  } 

  //__________________________________________________________________
  inline complex<@type@> 
  complex<@type@>::inverse() const 
  {
    gsl_complex@low@ t = gsl_complex@low@_inverse(*_complex); 
    gslmm::complex<@type@> ret(t, true);
    return ret; 
  } 

  //==================================================================
  /** @{
      @name Operations on complex<@type@> numbers  */
  //__________________________________________________________________
  /** Add a real and complex numbers 
      @ingroup complex
      @param x1 Real number
      @param z2 Complex number
      @return @f$ x_1 + z_2 @f$ */ 
  inline complex<@type@> operator+(const @type@&          x1,
				   const complex<@type@>& z2) 
  {
    complex<@type@> z1(x1, 0);
    complex<@type@> r(z1 + z2);
    return r;
  }
  //__________________________________________________________________
  /** Subtract a real and complex numbers 
      @ingroup complex
      @param x1 Real number
      @param z2 Complex number
      @return @f$ x_1 - z_2 @f$ */ 
  inline complex<@type@> operator-(const @type@&          x1,
				   const complex<@type@>& z2) 
  {
    complex<@type@> z1(x1, 0);
    complex<@type@> r(z1 - z2);
    return r;
  }
  //__________________________________________________________________
  /** Multiply a real and complex numbers 
      @ingroup complex
      @param x1 Real number
      @param z2 Complex number
      @return @f$ x_1 z_2 @f$ */ 
  inline complex<@type@> operator*(const @type@&          x1,
				   const complex<@type@>& z2) 
  {
    complex<@type@> z1(x1, 0);
    complex<@type@> r(z1 * z2);
    return r;
  }
  //__________________________________________________________________
  /** Divide a real and complex numbers 
      @ingroup complex
      @param x1 Real number
      @param z2 Complex number
      @return @f$ x_1 / z_2 @f$ */ 
  inline complex<@type@> operator/(const @type@&          x1,
				   const complex<@type@>& z2) 
  {
    complex<@type@> z1(x1, 0);
    complex<@type@> r(z1 / z2);
    return r;
  }

  /** @} */

  //==================================================================
  /** @{
      @name Exponential and logarithms of complex<@type@> numbers */
  /** exponentioal of a complex number
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> exp(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_exp(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Logarithm of a complex number (base e)
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> log(const complex<@type@>& z)  
  {
    return complex<@type@>(gsl_complex@low@_log(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Logarithm of a complex number (base 10)
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> log10(const complex<@type@>& z)   
  {
    return complex<@type@>(gsl_complex@low@_log10(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Logarithm of a complex number (base b) 
      @ingroup complex
      @param z
      @param b
      @return */ 
  inline complex<@type@> log(const complex<@type@>& z, 
			     const complex<@type@>& b) 
  {
    return complex<@type@>(gsl_complex@low@_log_b(z.as_c_type(), 
						  b.as_c_type())); 
  }
  /** @} */
  //==================================================================
  /** @{
      @name Powers of complex<@type@> numbers */
  /** Square root of a complex number
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> sqrt(const complex<@type@>& z) 
  {
    complex<@type@>::low_type rl = gsl_complex@low@_sqrt(z.as_c_type());
    complex<@type@> r = complex<@type@>(rl);
    // std::cout << "sqrt(" << z.real() << " + i * " << z.imag() 
    // << ") = " << GSL_REAL(rl) << " + i * " << GSL_IMAG(rl) << std::endl;
    return r; 
  }
  //__________________________________________________________________
  /** Complex square root of a real number
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> sqrt(const @type@ x) 
  {
    return complex<@type@>(gsl_complex@low@_sqrt_real(x)); 
  }
  //__________________________________________________________________
  /** Complex number to the z2 complex order
      @ingroup complex
      @param z1 Left-hand complex number
      @param z2 Right-hand complex number
      @return */ 
  inline complex<@type@> pow(const complex<@type@>& z1, 
			const complex<@type@>& z2) 
  {
    return complex<@type@>(gsl_complex@low@_pow(z1.as_c_type(), 
						z2.as_c_type())); 
  }
  //__________________________________________________________________
  /** Complex number to the x real order
      @ingroup complex
      @param z Complex number 
      @param x Real order
      @return @f$ z^x @f$ */ 
  inline complex<@type@> pow(const complex<@type@>& z, const @type@ x) 
  {
    return complex<@type@>(gsl_complex@low@_pow_real(z.as_c_type(), x)); 
  }
  /** @} */
  //==================================================================
  /** @{
      @name trigonometric functions on complex<@type@> numbers */
  /** Sine
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> sin(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_sin(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Cosine
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> cos(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_cos(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Tangent 
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> tan(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_tan(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Sectant
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> sec(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_sec(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Cosectant
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> csc(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_csc(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Cotangent
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> cot(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_cot(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse sine 
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arcsin(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arcsin(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse sine 
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arcsin(const @type@ x) 
  {
    return complex<@type@>(gsl_complex@low@_arcsin_real(x)); 
  }
  //__________________________________________________________________
  /** Inverse cosine 
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arccos(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arccos(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse cosine 
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arccos(const @type@ x) 
  {
    return complex<@type@>(gsl_complex@low@_arccos_real(x)); 
  }
  //__________________________________________________________________
  /** Inverse tangent 
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arctan(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arctan(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse tangent 
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arctan(const @type@ x) 
  {
    return complex<@type@>(::atan(x)); 
  }
  //__________________________________________________________________
  /** Inverse secant
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arcsec(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arcsec(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse secant
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arcsec(const @type@ x) 
  {
    return complex<@type@>(gsl_complex@low@_arcsec_real(x)); 
  }
  //__________________________________________________________________
  /** Inverse cosecant
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arccsc(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arccsc(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse cosecant
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arccsc(const @type@ x) 
  {
    return complex<@type@>(gsl_complex@low@_arccsc_real(x)); 
  }
  //__________________________________________________________________
  /** Inverse cotangent
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arccot(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arccot(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse cotangent
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arccot(const @type@ x) 
  {
    (void)x;
    return complex<@type@>(0., 0.); 
  }
  /** @} */
  //==================================================================
  /** @{
      @name Hyperbolic functions on complex<@type@> numbers */
  /** Hyperbolic sine
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> sinh(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_sinh(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Hyperbolic cosine
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> cosh(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_cosh(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Hyperbolic tangent
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> tanh(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_tanh(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Hyperbolic secant
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> sech(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_sech(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Hyperbolic cosecant
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> csch(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_csch(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Hyperbolic cotangent
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> coth(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_coth(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic sine
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arcsinh(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arcsinh(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic sine
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arcsinh(const @type@ x) 
  {
    return complex<@type@>(::asinh(x),0.); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic cosine
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arccosh(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arccosh(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic cosine
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arccosh(const @type@ x) 
  {
    return complex<@type@>(gsl_complex@low@_arccosh_real(x)); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic tangent
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arctanh(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arctanh(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic tangent
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arctanh(const @type@ x) 
  {
    return complex<@type@>(gsl_complex@low@_arctanh_real(x)); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic secant
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arcsech(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arcsech(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic secant
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arcsech(const @type@ x) 
  {
    (void)x;
    return complex<@type@>(0., 0.); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic cosecant
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arccsch(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arccsch(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic cosecant
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arccsch(const @type@ x) 
  {
    (void)x;
    return complex<@type@>(0., 0.); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic cotangent
      @ingroup complex
      @param z
      @return */ 
  inline complex<@type@> arccoth(const complex<@type@>& z) 
  {
    return complex<@type@>(gsl_complex@low@_arccoth(z.as_c_type())); 
  }
  //__________________________________________________________________
  /** Inverse hyperbolic cotangent
      @ingroup complex
      @param x
      @return */ 
  inline complex<@type@> arccoth(const @type@ x) 
  {
    (void)x;
    return complex<@type@>(0., 0.); 
  }
  //@}
}
#endif
//____________________________________________________________________
//
// EOF
//
