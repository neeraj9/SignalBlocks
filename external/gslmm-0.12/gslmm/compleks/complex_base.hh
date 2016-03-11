//
// $Id: complex_base.hh,v 1.10 2006-05-01 14:24:38 cholm Exp $ 
//  
//  gslmm::complex
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
#ifndef GSLMM_compleks_complex_base
#define GSLMM_compleks_complex_base

/** @file   complex_base.hh
    @author Christian Holm
    @date   Sat Mar 01 19:07:19 2003
    @brief  Base template for complex classes */

#ifndef __COMPLEX__
#include <complex>
#endif
#ifndef GSLMM_util
# include <gslmm/gslmm-util.hh>
#endif
#ifndef GSLMM_math_type_trait
# include <gslmm/math/type_trait.hh>
#endif
#ifndef __CMATH__
# include <cmath>
#endif

#include <cstdlib>  // abs

namespace gslmm
{
  /** @class complex complex_base.hh <gslmm/compleks/complex_base.hh>
      Complex numbers (base template).
      @ingroup complex
   */
  template <typename T>
  class complex
  {
  private:
    /// The real part. 
    T _real;
    /// The imaginary part. 
    T _imag;
  public:
    /** The imaginary unit */
    static const complex<T>& i();
    /** Constructor.
	@param real The real part or size of the complex number. 
	@param imag The imaginary part or argument of the complex number.
	@param polar if true, interpret x, y as size and argument,
	otherwise as carisian coordinates. */
    complex(const T real=0, const T imag=0, bool polar=false) { 
      assign(real, imag, polar); }
    /** Copy constructor from std::complex object. 
	@param z std::complex object to assign from. */
    complex(const std::complex<T>& z) { 
      assign(z.real(), z.imag()); }
    /** Copy constructor. 
	@param z Complex number to copy. */
    complex(const complex<T>& z) { 
      assign(z.real(), z.imag()); }
    /** Destructor. */
    virtual ~complex() {}

    /** Check if this object is real. 
	@return true if imaginary part is zero. */ 
    bool is_real() const { return _imag == 0.; }

    /** Check if this object is purely complex. 
	@return true if real part is zero. */ 
    bool is_complex() const { return _real == 0.; }

    /** Set the complex number. 
	@param real The real part or size of the complex number. 
	@param imag The imaginary part or argument of the complex number.
	@param polar if true, interpret x, y as size and argument,
	otherwise as carisian coordinates. */
    void assign(const T real=0, const T imag=0, bool polar=false);
    /** Set the real part of the complex number. 
	@return The real part of the complex number. */
    const T& real() const { return _real; }
    /** Set the imaginary part of the complex number. 
	@return The real part of the complex number. */
    const T& imag() const { return _imag; }

    /** Get the real part of the complex number. 
	@return real part @f$ x@f$ of @f$ z=x+iy@f$. */
    T& real() { return _real; }
    /** Get the imaginary part of the complex number. 
	@return imaginary part @f$ y@f$ of @f$ z=x+iy@f$. */
    T& imag() { return _imag; }
    /** Get the argument of the complex number. 
	@return @f$\arg{z}@f$ where @f$-\pi<\arg{z}\leq\pi@f$. */
    T arg() const { return atan2(_imag, _real); }
    /** Get the absolute size of the complex number. 
	@return @f$|z|@f$ */
    T abs() const { return sqrt(abs()); }
    /** Get the squared absolute size of the complex number. 
	@return @f$|z|^2@f$ */
    T square_abs() const;
    /** Get the logaritm of absolute size of the complex number. 
	@return @f$\log|z|@f$ */
    T log_of_abs() const;

    /** Assignment 
	@param z number to assign from. 
	@return this object. */ 
    complex<T>& operator=(const complex<T>& z) {
      assign(z.real(),z.imag()); return *this; }

    /** Assignment 
	@param x number to assign from. 
	@return this object. */ 
    complex<T>& operator=(const T& x) {
      assign(x,0); return *this; }

    /** Assignment 
	@param z number to assign from. 
	@return this object. */ 
    complex<T>& operator=(const std::complex<T>& z) {
      assign(z.real(),z.imag()); return *this; }

    /** Comparison for equality. 
	@param z1 complex number to compare to. 
	@return true if @f$ x = x_1 \wedge y = y_1 @f$, false
	otherwise. */
    bool operator==(const complex<T>& z1) const; 
    /** Comparison for in-equality. 
	@param z1 complex number to compare to. 
	@return true if @f$ \not (x = x_1 \wedge y = y_1) @f$, false
	otherwise. */
    bool operator!=(const complex<T>& z1) const; 

    /** Unary minus. 
	@return @f$ -x - iy@f$. */ 
    complex<T> operator-() const{ return complex<T>(-_real, -_imag); }

    /** Add a complex number. 
	@param z1 First number. 
	@return @f$ z = x + x_1 + i(y + y_1)  @f$ */
    complex<T>& operator+=(const complex<T>& z1);
    /** Subract a complex number. 
	@param z1 First number. 
	@return @f$ z = x - x_1 + i(y - y_1)  @f$ */
    complex<T>& operator-=(const complex<T>& z1);
    /** Multiply a complex number. 
	@param z1 First number. 
	@return @f$ z = x x_1 + i(y y_1)  @f$ */
    complex<T>& operator*=(const complex<T>& z1);
    /** Divide a complex number. 
	@param z1 First number. 
	@return @f$ z = x/ x_1 - i(y / y_1)  @f$ */
    complex<T>& operator/=(const complex<T>& z1);

    /** Add a real number to real and imaginary part. 
	@param a First number. 
	@return @f$ z = x + a + i(y + a)  @f$ */
    complex<T>& operator+=(const T a);
    /** Subract a real number to real and imaginary part. 
	@param a First number. 
	@return @f$ z = x - a + i(y - a)  @f$ */
    complex<T>& operator-=(const T a);
    /** Multiply a real number to real and imaginary part. 
	@param a First number. 
	@return @f$ z = x a + i(y a)  @f$ */
    complex<T>& operator*=(const T a);
    /** Divide a real number to real and imaginary part. 
	@param a First number. 
	@return @f$ z = x/ a - i(y / a)  @f$ */
    complex<T>& operator/=(const T a);

    /** Add a real number to real part. 
	@f$ z = x + a + iy  @f$
	@param a First number.  */
    void add_to_real(const T a) { _real += a; }
    /** Subract a real number to real part. 
	@f$ z = x - a + iy  @f$
	@param a First number.  */
    void subtract_from_real(const T a) { _real -= a; }
    /** Multiply a real number to real part. 
	@f$ z = x a + iy  @f$
	@param a First number.  */
    void multiply_real(const T a) { _real *= a; }
    /** Divide a real number to real part. 
	@f$ z = x / a + iy  @f$
	@param a First number.  */
    void divide_real(const T a) { _real /= a; }

    /** Add a real number to imaginary part. 
	@f$ z = x + i(y + a) @f$ 
	@param a First number. */
    void add_to_imag(const T a) { _imag += a; }
    /** Subract a real number to imaginary part. 
	@f$ z = x + i(y - a) @f$
	@param a First number.  */
    void subtract_from_imag(const T a) { _imag -= a; }
    /** Multiply a real number to imaginary part. 
	@f$ z = x + iy a @f$
	@param a First number.  */
    void multiply_imag(const T a) { _imag *= a; }
    /** Divide a real number to imaginary part. 
	@f$ z = x + iy / a @f$
	@param a First number.  */
    void divide_imag(const T a) { _imag /= a; }

    /** Make the complex conjugate of this complex number.
	@return @f$ x - iy@f$. */
    complex<T> conjugate() const { return complex<T>(_real, -_imag); }
    /** Make the complex conjugate of this complex number.
	@return @f$ (x - iy) / (x^2 + y^2) @f$. */
    complex<T> inverse() const { return conjugate() /= square_abs(); }
  
    /** Turn the object into a std::complex object. 
	@return a std::complex<T> object. */ 
    std::complex<T> as_std() const { return std::complex<T>(_real, _imag); }
  };

  //==================================================================
  /** @brief Type trait of complex<T> numbers 
      @ingroup math
      @ingroup complex 
  */
  template <typename T>
  struct type_trait<complex<T> > 
  {
    /** Type of the trait */
    typedef complex<T> value_type;
    /** Type of elements */
    typedef typename complex<T>::value_type elem_type;
    /** Type of trait of elements */
    typedef type_trait<elem_type> elem_trait;
    /** Type of reference */
    typedef complex<T> reference_type;
    /** Type of pointer */
    typedef complex<T>* pointer_type;
    /** Type of pointer */
    typedef T elementary_type;
    /** multicative neutral element */  
    static const value_type& unit()
    { 
      static value_type u(elem_trait::unit(), elem_trait::null());
      return u; 
    }
    /** additive neutral element */  
    static const value_type& null()
    { 
      static value_type z(elem_trait::null(), elem_trait::null());
      return z; 
    }
    /** Smallest difference representable by a complex<T> variable,
	@return @f$\epsilon_{\mbox{complex<T>}}@f$.  */
    static const value_type& epsilon()
    { 
      static value_type e(elem_trait::epsilon(), elem_trait::epsilon());
      return e; 
    }
    /** Least number representable by a complex<T>,
	@return @f$\mbox{min}_{\mbox{complex<T>}}@f$.  */
    static const value_type& min()              
    { 
      static value_type m(elem_trait::min(), elem_trait::min());
      return m; 
    }
    /** Largest number representable by a complex<T>,
	@return @f$\mbox{max}_{\mbox{complex<T>}}@f$.  */
    static const value_type& max()
    { 
      static value_type m(elem_trait::max(), elem_trait::max());
      return m; 
    }

    /** @param z Number to "conjugate"
	@return @f$ \bar{z} = x - iy@f$ */
    static const value_type conjugate(const complex<T>& z) 
    { 
      return z.conjugate(); 
    }
  };

  //__________________________________________________________________
  template <typename T> 
  inline const complex<T>& 
  complex<T>::i()
  {
    static complex<T> _i(type_trait<T>::null(), type_trait<T>::unit());
    return _i;
  }
  //__________________________________________________________________
  // Square argument of complex number. 
  template <typename T> 
  inline void complex<T>::assign(const T r, const T i, bool polar) 
  {
    if (polar) {
      _real = r * ::cos(i);
      _imag = r * ::sin(i);
    }
    else {
      _real = r; 
      _imag = i; 
    }
  }
  

  //__________________________________________________________________
  // Square argument of complex number. 
  template <typename T> 
  inline T complex<T>::square_abs() const
  {
    T x = _real, y = _imag;
    T s = max(::abs(x), ::abs(y));
    x /= s;
    y /= s;
    return s * s (x * x + y * y);
  }
  
  //__________________________________________________________________
  // Logarithm of argument of complex number. 
  template <typename T> 
  inline T complex<T>::log_of_abs() const
  {
    T x = ::abs(_real), y = ::abs(_imag);
    T s = (x >= y ? x : y);
    T u = (x >= y ? y / x : x / y);  
    return log(s) + .5 * log1p(u * u);
  }
  
  //__________________________________________________________________
  // Equality 
  template <typename T> 
  inline bool complex<T>::operator==(const complex<T>& z1) const 
  {
    return (_real == z1._real && _imag == z1._imag);
  }
  //__________________________________________________________________
  // Equality 
  template <typename T> 
  inline bool complex<T>::operator!=(const complex<T>& z1) const 
  {
    return !(*this == z1);
  }
  

  //__________________________________________________________________
  // Add a complex number. 
  template <typename T>
  inline complex<T>& complex<T>::operator+=(const complex<T>& z1)
  {
    _real += z1._real; _imag += z1._imag; return *this; 
  }

  //__________________________________________________________________
  // Subract a complex number. 
  template <typename T>
  inline complex<T>& complex<T>::operator-=(const complex<T>& z1)
  {
    _real -= z1._real; _imag -= z1._imag; return *this; 
  }

  //__________________________________________________________________
  // Multiply a complex number. 
  template <typename T>
  inline complex<T>& complex<T>::operator*=(const complex<T>& z1)
  {
    _real *= z1._real; _imag *= z1._imag; return *this; 
  }

  //__________________________________________________________________
  // Divide a complex number. 
  template <typename T>
  inline complex<T>& complex<T>::operator/=(const complex<T>& z1)
  {
    _real /= z1._real; _imag /= z1._imag; return *this; 
  }

  //__________________________________________________________________
  // Add a real number to real and imaginary part. 
  template <typename T>
  inline complex<T>& complex<T>::operator+=(const T a)
  {
    _real += a; _imag += a; return *this; 
  }

  //__________________________________________________________________
  // Subract a real number to real and imaginary part. 
  template <typename T>
  inline complex<T>& complex<T>::operator-=(const T a)
  {
    _real -= a; _imag -= a; return *this; 
  }

  //__________________________________________________________________
  // Multiply a real number to real and imaginary part. 
  template <typename T>
  inline complex<T>& complex<T>::operator*=(const T a)
  {
    _real *= a; _imag *= a; return *this; 
  }

  //__________________________________________________________________
  // Divide a real number to real and imaginary part. 
  template <typename T>
  inline complex<T>& complex<T>::operator/=(const T a)
  {
    _real /= a; _imag /= a; return *this; 
  }

  //==================================================================
  /// @name Operations on complex numbers 
  //@{
  /** Add two complex numbers. 
      @ingroup complex
      @param z1 First number. 
      @param z2 Second number. 
      @return @f$ z = x_1 + x_2 + i(y_1 + y_2)  @f$ */
  template <typename T>
  inline complex<T> operator+(const complex<T>& z1, const complex<T>& z2) 
  {
    complex<T> z(z1); return z += z2;
  }

  //__________________________________________________________________
  /** Subract two complex numbers. 
      @ingroup complex
      @param z1 First number. 
      @param z2 Second number. 
      @return @f$ z = x_1 - x_2 + i(y_1 - y_2)  @f$ */
  template <typename T>
  inline complex<T> operator-(const complex<T>& z1, const complex<T>& z2)
  {
    complex<T> z(z1,true); return z -= z2;
  }

  //__________________________________________________________________
  /** Multiply two complex numbers. 
      @ingroup complex
      @param z1 First number. 
      @param z2 Second number. 
      @return @f$ z = x_1 x_2 - i(y_1 y_2)  @f$ */
  template <typename T>
  inline complex<T> operator*(const complex<T>& z1, const complex<T>& z2)
  {
    complex<T> z(z1,true); z *= z2;
    return z;
  }

  //__________________________________________________________________
  /** Divide two complex numbers. 
      @ingroup complex
      @param z1 First number. 
      @param z2 Second number. 
      @return @f$ z = x_1/ x_2 - i(y_1 / y_2)  @f$ */
  template <typename T>
  inline complex<T> operator/(const complex<T>& z1, const complex<T>& z2)
  {
    complex<T> z(z1,true); return z /= z2;
  }

  //__________________________________________________________________
  /** Add real to a complex number. 
      @ingroup complex
      @param z complex number. 
      @param a real number. 
      @return @f$ z = x + a + i(y_1 + a)  @f$ */
  template <typename T>
  inline complex<T> operator+(const complex<T>& z,const T a) 
  {
    complex<T> z1(z,true); return z1 += a;
  }

  //__________________________________________________________________
  /** Subract real from a complex number. 
      @ingroup complex
      @param z complex number. 
      @param a real number. 
      @return @f$ x - a + i(y - a)  @f$ */
  template <typename T>
  inline complex<T> operator-(const complex<T>& z,const T a)
  {
    complex<T> z1(z,true); return z1 -= a;
  }

  //__________________________________________________________________ 
  /** Multiply a complex number by a real number. 
      @ingroup complex
      @param z complex number. 
      @param a real number. 
      @return @f$ x a + i(y a)  @f$ */
  template <typename T>
  inline complex<T> operator*(const complex<T>& z,const T a)
  {
    complex<T> z1(z,true);
    return z1 *= a;
  }

  //__________________________________________________________________
  /** Divide complex number by a real number. 
      @ingroup complex
      @param z complex number. 
      @param a real number. 
      @return @f$ x / a + i(y / a)  @f$ */
  template <typename T>
  inline complex<T> operator/(const complex<T>& z,const T a)
  {
    complex<T> z1(z,true);
    return z1 /= a;
  }
  //__________________________________________________________________
  /** Add real to a complex number. 
      @ingroup complex
      @param z complex number. 
      @param a real number. 
      @return @f$ z = x + a + i(y_1 + a)  @f$ */
  template <typename T>
  inline complex<T> operator+(const T a, const complex<T>& z) 
  {
    complex<T> z1(a,0); 
    return z1 - z;
  }

  //__________________________________________________________________
  /** Subract real from a complex number. 
      @ingroup complex
      @param z complex number. 
      @param a real number. 
      @return @f$ x - a + i(y - a)  @f$ */
  template <typename T>
  inline complex<T> operator-(const T a, const complex<T>& z)
  {
    complex<T> z1(a,0); 
    return z1 - z;
  }

  //__________________________________________________________________ 
  /** Multiply a complex number by a real number. 
      @ingroup complex
      @param z complex number. 
      @param a real number. 
      @return @f$ x a + i(y a)  @f$ */
  template <typename T>
  inline complex<T> operator*(const T a, const complex<T>& z)
  {
    complex<T> z1(a, 0);
    return z1 * z;
  }

  //__________________________________________________________________
  /** Divide complex number by a real number. 
      @ingroup complex
      @param z complex number. 
      @param a real number. 
      @return @f$ x / a + i(y / a)  @f$ */
  template <typename T>
  inline complex<T> operator/(const T a, const complex<T>& z)
  {
    complex<T> z1(a, 0);
    return z1 / z;
  }
  //@}
  
  //==================================================================
  /// @name Exponential and Logarithms of complex numbers
  //@{
  /** Exponential of a complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ \rho\cos(\theta) + i\rho\sin(\theta)@f$ where
      @f$\rho = e^x@f$ and  @f$\theta = y@f$ */ 
  template <typename T> 
  inline complex<T> exp(const complex<T>& z) 
  {
    T rho = ::exp(z.real()); 
    return complex<T>(rho * ::sin(z.imag()), rho * ::cos(z.imag())); 
  }
  
  //__________________________________________________________________
  /** Logarithm of complex number (base @f$ e@f$). 
      @ingroup complex
      @param z the complex number. 
      @return @f$\log|z| + i\arg{z}@f$. */
  template <typename T> 
  inline complex<T> log(const complex<T>& z)   
  {
    return complex<T>(z.log_of_abs(), z.arg());
  }
  
  //__________________________________________________________________
  /** Logarithm of complex number (base @f$ 10@f$). 
      @ingroup complex
      @param z the complex number. 
      @return @f$(\log|z| + i\arg{z}) / log(10) @f$. */
  template <typename T> 
  inline complex<T> log10(const complex<T>& z)   
  {
    complex<T> z1 = log(z1); z1 /= ::log(10.); return z1;
  }
  
  //__________________________________________________________________
  /** Logarithm of complex number (base @f$ b@f$). 
      @ingroup complex
      @param z the complex number. 
      @param b the complex number. 
      @return @f$\log(z) / log(b) @f$. */
  template <typename T> 
  inline complex<T> log(const complex<T>& z, const complex<T>& b)   
  {
    complex<T> z1 = log(z1); z1 /= log(b); return z1;
  }
  //@}
  
  //==================================================================
  /// @name Powers of complex numbers 
  //@{
  /** The square root of a complex number. 
      @ingroup complex
      @param z the complex number to take the square root of. 
      @return the square root of argument. */ 
  template <typename T> 
  inline complex<T> sqrt(const complex<T>& z) 
  {
    if (z.real() == 0. && z.imag() == 0.)
      return complex<T>(0,0);
    T x = ::abs(z.real()), y = ::abs(z.imag());
    T t = (x >= y ? y / x : x / y);
    T w = (x >= y ? 
	   ::sqrt(x) * ::sqrt(.5 * (1. + sqrt(1. + t * t))) :
	   ::sqrt(y) * ::sqrt(.5 * (t + sqrt(1. + t * t))));
    if (z.real() >= 0) 
      return complex<T>(w, z.imag() / 2. / w);
    T vi = (z.imag() >= 0 ? w : -w);
    return complex<T>(z.imag() / 2. / vi, vi);
  }

  //__________________________________________________________________
  /** The complex root of real number.
      @ingroup complex
      @param x real number.
      @return square root of argument. */ 
  template <typename T> 
  inline complex<T> sqrt(const T x) 
  {
    return (x >= 0 ? complex<T>(::sqrt(x), 0) : 
	    complex<T>(0,::sqrt(x)));
  }

  //__________________________________________________________________
  /** Complex power of a complex number. 
      @ingroup complex
      @param z1 The base. 
      @param z2 The power. 
      @return @f$ \rho\cos\beta,rho\sin\beta@f$, where
      @f$\rho=\exp(\log(|z|)x-y\arg{z})@f$ and 
      @f$\beta=\arg{z}x+y\log(|z|)@f$. */
  template <typename T> 
  inline complex<T> pow(const complex<T>& z1, 
				      const complex<T>& z2) 
  {
    complex<T> z(0,0);
    if (z1.real() == 0. && z1.imag() == 0.) 
      return z;
    T logr  = z.log_of_abs();
    T theta = z.arg();
    T x     = z.real(), y = z.imag(); 
    T rho   = ::exp(logr * x - y * theta);
    T beta  = theta * x + y * logr;
    z.assign(rho * ::cos(beta), rho * ::sin(beta));
    return z;
  }

  //__________________________________________________________________
  /** Real power of a complex number. 
      @ingroup complex
      @param z The base. 
      @param x The power. 
      @return @f$ \rho\cos\beta,rho\sin\beta@f$, where
      @f$\rho=\exp(\log(|z|)x)@f$ and @f$\beta=\arg{z}x@f$. */
  template <typename T> 
  inline complex<T> pow(const complex<T>& z, const T x) 
  {
    complex<T> z1(0,0);
    if (z1.real() != 0. || z1.imag() != 0.) {
      T logr  = z.log_of_abs();
      T theta = z.arg();
      T rho   = ::exp(logr * x);
      T beta  = theta * x;
      z1.assign(rho * ::cos(beta), rho * ::sin(beta));
    }
    return z1;
  }
  //@}

  //==================================================================
  /// @name Trigonometric functions of complex numbers 
  //@{
  /** Sinus of complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ \sin(x)\cosh(y) + i\cos(x)\sinh(y)@f$. */
  template <typename T> 
  inline complex<T> sin(const complex<T>& z)  
  {
    complex<T> z1(0,0);
    if (z.is_real()) 
      z1.real(::sin(z.real()));
    else 
      z1.assign(::sin(z.real()) * ::cosh(z.imag()), 
		::cos(z.real()) * ::sinh(z.imag()));
    return z1;
  }

  //__________________________________________________________________
  /** Cosine of complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ \cos(x)\cosh(y) + i\sin(x)\sinh(-y)@f$. */
  template <typename T> 
  inline complex<T> cos(const complex<T>& z)  
  {
    complex<T> z1(0,0);
    if (z.is_real()) 
      z1.real(::cos(z.real()));
    else 
      z1.assign(::cos(z.real()) * ::cosh(z.imag()), 
	     ::sin(z.real()) * ::sinh(-z.imag()));
    return z1;
  }

  //__________________________________________________________________
  /** Tangent of complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ \tan z@f$. */
  template <typename T> 
  inline complex<T> tan(const complex<T>& z)  
  {
    complex<T> z1(0,0);
    if (::abs(z.imag()) < 1) {
      T d = ::pow(::cos(z.real(), 2.) + ::pow(::sinh(z.imag(), 2.)));
      z1.real(.5 * ::sin(2 * z.real()) / d, 
		  .5 * ::sinh(2 * z.imag()) / d);
    }
    else {
      T u = ::exp(-z.imag());
      T c = 2 * u / (1 - u * u);
      T s = ::pow(c, 2.);
      T d = 1 + ::pow(::cos(z.real()), 2.) * s;
      T t = 1. / ::tanh(z.imag());
      z1.assign(.5 * ::sin(2 * z.real()) * s / d, t / d);
    }
    return z1;
  }

  //__________________________________________________________________
  /** Secant of a complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ 1 / \cos z @f$. */ 
  template <typename T> 
  inline complex<T> sec(const complex<T>& z)  
  {
    complex<T> z1 = cos(z); return z1.inverse(); 
  }

  //__________________________________________________________________
  /** Cosecant of a complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ 1 / \sin z @f$. */ 
  template <typename T> 
  inline complex<T> csc(const complex<T>& z)  
  {
    complex<T> z1 = sin(z); return z1.inverse(); 
  }

  //__________________________________________________________________
  /** Cotangent of a complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ 1 / \tan z @f$. */ 
  template <typename T> 
  inline complex<T> cot(const complex<T>& z)  
  {
    complex<T> z1 = tan(z); return z1.inverse(); 
  }

  //__________________________________________________________________
  /** Inverse sinus of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \sin^{-1} z@f$. */ 
  template <typename T> 
  inline complex<T> arcsin(const complex<T>& z)  
  {
    complex<T> z1(0,0);
    if (z.is_real()) 
      z = arcsin(z.real());
    else {
      T x = ::abs(z.real()), y = ::abs(z.imag());
      T r  = hypot(x + 1, y), s = hypot(x - 1, y);
      T a  = .5 * (r + s);
      T b  = x / a;
      T y2 = y * y;
      T real, imag;
      const T a_cross = 1.5, b_cross = .6417;
      if (b <= b_cross)
	real = ::asin(b);
      else {
	if (x <= 1) {
	  T d  = .5 * (a + x ) * (y2 / (r + x + 1) + s + 1 - x);
	  real = ::atan(x / ::sqrt(d));
	}
	else {
	  T d  = .5 * ((a + x) / (r + x + 1) + (a + x) / (s + x - 1));
	  real = ::atan(x / y / ::sqrt(d));
	}
      }
      if (a <= a_cross) {
	T am1;
	if (x < 1) 
	  am1 = .5 * (y2 / (r + x + 1) + y2 / (s + 1 - x));
	else 
	  am1 = .5 * (y2 / (r + x + 1) + s + x - 1);
	imag = log1p(am1 + ::sqrt(am1 * (a + 1)));
      }
      else 
	imag = ::log(a + ::sqrt(a * a - 1));
      z1.assign(z.real() >= 0 ? real : -real, z.imag() >= 0 ? imag : -imag);
    }
    return z1;
  }

  //__________________________________________________________________
  /** inverse sine of real number. 
      @ingroup complex
      @param x real number. 
      @return @f$ \sin^{-1} z@f$ */ 
  template <typename T> 
  inline complex<T> arcsin(const T x)  
  {
    complex<T> z1(0,0);
    if (::abs(x) <= 1.) 
      z1.assign(::asin(x), 0.);
    else {
      if (x < 0.)
	z1.assign(-M_PI_2, ::acosh(-x));
      else 
	z1.assign(M_PI_2, -::acosh(x));
    }
    return z1;
  }

  //__________________________________________________________________
  /** Inverse cosine of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \cos^{-1} z@f$. */ 
  template <typename T> 
  inline complex<T> arccos(const complex<T>& z)  
  {
    complex<T> z1(0,0);
    if (z.is_real()) 
      z = arccos(z.real());
    else {
      T x  = ::abs(z.real()), y = ::abs(z.imag());
      T r  = hypot(x + 1, y), s = hypot(x - 1, y);
      T a  = .5 * (r + s);
      T b  = x / a;
      T y2 = y * y;
      T real, imag;
      const T a_cross = 1.5, b_cross = .6417;
      if (b <= b_cross)
	real = ::acos(b);
      else {
	if (x <= 1) {
	  T d  = .5 * (a + x) * (y2 / (r + x + 1) + s + 1 - x);
	  real = ::atan(::sqrt(d) / x);
	}
	else {
	  T d  = .5 * ((a + x) / (r + x + 1) + (a + x) / (s + x - 1));
	  real = ::atan(y * ::sqrt(d) / x);
	}
      }
      if (a <= a_cross) {
	T am1;
	if (x < 1) 
	  am1 = .5 * (y2 / (r + x + 1) + y2 / (s + 1 - x));
	else 
	  am1 = .5 * (y2 / (r + x + 1) + s + x - 1);
	imag = log1p(am1 + ::sqrt(am1 * (a + 1)));
      }
      else 
	imag = ::log(a + ::sqrt(a * a - 1));
      z1.assign(z.real() >= 0 ? real : M_PI -real, 
	     z.imag() >= 0 ? -imag : imag);
    }
    return z1;
  }

  //__________________________________________________________________
  /** inverse cosine of real number. 
      @ingroup complex
      @param x real number. 
      @return @f$ \cos^{-1} x@f$ */ 
  template <typename T> 
  inline complex<T> arccos(const T x)  
  {
    complex<T> z1(0,0);
    if (::abs(x) <= 1.) 
      z1.assign(::acos(x), 0.);
    else {
      if (x < 0.)
	z1.assign(M_PI, -::acosh(-x));
      else 
	z1.assign(0, ::acosh(-x));
    }
    return z1;
  }

  //__________________________________________________________________
  /** Inverse tangent of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \tan^{-1} z@f$. */ 
  template <typename T> 
  inline complex<T> arctan(const complex<T>& z)  
  {
    complex<T> z1(0,0);
    if (z.is_real()) 
      z1.assign(::atan(z.real()), 0);
    else {
      T r = hypot(z.real(), z.imag());
      T u = 2 * z.imag() / (1 + r * r);
      T imag;
      if (::abs(u) < .1) 
	imag = .25 * (log1p(u) - log1p(-u));
      else {
	T a  = hypot(z.real(), z.imag() + 1);
	T b  = hypot(z.real(), z.imag() - 1);
	imag = .5 * :: log(a / b);
      }
      if (z.is_complex()) {
	if (z.imag()) 
	  z1.assign(M_PI_2, imag);
	else if (z.imag() < -1) 
	  z1.assign(-M_PI_2, imag);
	else 
	  z1.assign(0, imag);
      }
      else 
	z1.assign(.5 * :: atan2(2 * z.real, (1 + r) * (1 - r)), imag); 
    }
    return z1;
  }

  //__________________________________________________________________
  /** inverse tangent of real number. 
      Mainly for consitency 
      @ingroup complex
      @param x real number. 
      @return @f$ \tan^{-1} x@f$ */ 
  template <typename T> 
  inline complex<T> arctan(const T x)  
  {
    return complex<T>(::atan(x),0);
  }

  //__________________________________________________________________
  /** Inverse secant of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \sec^{-1} z@f$. */ 
  template <typename T> 
  inline complex<T> arcsec(const complex<T>& z)  
  {
    complex<T> z1 = z.inverse();
    return arccos(z1);
  }

  //__________________________________________________________________
  /** Inverse secant of real number. 
      @ingroup complex
      @param x real number. 
      @return @f$ \sec^{-1} x@f$. */ 
  template <typename T> 
  inline complex<T> arcsec(const T x)  
  {
    complex<T> z1; 
    if (x <= -1. || x >= 1.) 
      z1.assign(::acos(1 / x), 0);
    else {
      if (x >= 0) 
	z1.assign(0, ::acosh(1 / x));
      else 
	z1.assign(M_PI, -::acosh(-1 / x));
    }
    return z1;
  }

  //__________________________________________________________________
  /** Inverse cosecant of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \sec^{-1} z@f$. */ 
  template <typename T> 
  inline complex<T> arccsc(const complex<T>& z)  
  {
    complex<T> z1 = z.inverse();
    return arcsin(z1);
  }

  //__________________________________________________________________
  /** Inverse cosecant of real number. 
      @ingroup complex
      @param x real number. 
      @return @f$ \csc^{-1} x@f$. */ 
  template <typename T> 
  inline complex<T> arccsc(const T x)  
  {
    complex<T> z1; 
    if (x <= -1. || x >= 1.) 
      z1.assign(::asin(1 / x), 0);
    else {
      if (x >= 0) 
	z1.assign(M_PI_2, -::acosh(1 / x));
      else 
	z1.assign(M_PI_2, -::acosh(-1 / x));
    }
    return z1;
  }

  //__________________________________________________________________
  /** Inverse cotangent of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \sec^{-1} z@f$. */ 
  template <typename T> 
  inline complex<T> arccot(const complex<T>& z)  
  {
    complex<T> z1;
    if (z.real() == 0. && z.imag() == 0) 
      z1.assign(M_PI_2, 0);
    else 
      z1 = arctan(z.inverse());
    return z1;
  }

  //__________________________________________________________________
  /** inverse cotangent of real number. 
      Mainly for consitency 
      @ingroup complex
      @param x real number. 
      @return @f$ \tan^{-1} x@f$ */ 
  template <typename T> 
  inline complex<T> arccot(const T x)  
  {
    return gslmm::arccot(gslmm::complex<T>(x,0));
  }
  //@}

  //==================================================================
  /// @name Hyperbolic functions on complex numbers 
  //@{
  /** Hyberbolic Sinus of complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ \sinh(x)\cos(y) + i\cosh(x)\sin(y)@f$. */
  template <typename T> 
  inline complex<T> sinh(const complex<T>& z)  
  {
    return complex<T>(::sinh(z.real()) * ::cos(z.imag()), 
			     ::cosh(z.real()) * ::sin(z.imag()));
  }

  //__________________________________________________________________
  /** Hyberbolic Cosine of complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ \cosh(x)\cos(y) + i\sinh(x)\sin(-y)@f$. */
  template <typename T> 
  inline complex<T> cosh(const complex<T>& z)  
  {
    return complex<T>(::cosh(z.real()) * ::cos(z.imag()), 
			     ::sinh(z.real()) * ::sin(z.imag()));
  }

  //__________________________________________________________________
  /** Hyberbolic Tangent of complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ \tan z@f$. */
  template <typename T> 
  inline complex<T> tanh(const complex<T>& z)  
  {
    complex<T> z1(0,0);
    T d = ::pow(::cos(z.imag(), 2.) + ::pow(::sinh(z.real()), 2.));
    if (::abs(z.real()) < 1.0) 
      z1.assign(::sinh(z.real()) * ::cosh(z.real()) / d, 
	     .5 * ::sin(2 * z.imag()) / d);
    else {
      T f = 1 + ::pow(::cos(z.imag()) / ::sinh(z.real()), 2.);
      z1.assign(1. / ::tanh(z.real()) / f, .5 * ::sin(2 * z.imag()) / d);
    }
    return z1;
  }

  //__________________________________________________________________
  /** Hyberbolic Secant of a complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ 1 / \cosh z @f$. */ 
  template <typename T> 
  inline complex<T> sech(const complex<T>& z)  
  {
    complex<T> z1 = cosh(z);
    return z1.inverse(); 
  }

  //__________________________________________________________________
  /** Hyberbolic Cosecant of a complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ 1 / \sinh z @f$. */ 
  template <typename T> 
  inline complex<T> csch(const complex<T>& z)  
  {
    complex<T> z1 = sinh(z);
    return z1.inverse(); 
  }

  //__________________________________________________________________
  /** Hyberbolic Cotangent of a complex number. 
      @ingroup complex
      @param z argument. 
      @return @f$ 1 / \tanh z @f$. */ 
  template <typename T> 
  inline complex<T> coth(const complex<T>& z)  
  {
    complex<T> z1 = tanh(z); 
    return z1.inverse(); 
  }

  //__________________________________________________________________
  /** Inverse Hyberbolic sinus of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \sin^{-1} z@f$. */ 
  template <typename T> 
  inline complex<T> arcsinh(const complex<T>& z)  
  {
    complex<T> z1 = arcsin(z);
    z1.imag(-1 * z1.imag()); 
    return z1;
  }

  //__________________________________________________________________
  /** inverse Hyberbolic sine of real number. 
      @ingroup complex
      @param x real number. 
      @return @f$ \sin^{-1} z@f$ */ 
  template <typename T> 
  inline complex<T> arcsinh(const T x)  
  {
    return complex<T>(::asinh(x), 0);
  }

  //__________________________________________________________________
  /** Inverse Hyberbolic cosine of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \cos^{-1} z@f$. */ 
  template <typename T> 
  inline complex<T> arccosh(const complex<T>& z)  
  {
    complex<T> z1 = arccos(z);
    if (z1.imag() > 0)
      z1.imag(-1 * z1.imag()); 
    return z1;
  }

  //__________________________________________________________________
  /** inverse Hyberbolic cosine of real number. 
      @ingroup complex
      @param x real number. 
      @return @f$ \arccos x@f$ */ 
  template <typename T> 
  inline complex<T> arccosh(const T x)  
  {
    complex<T> z1(0,0);
    if (x >= 1) 
      z1.assign(::acosh(x), 0);
    else {
      if (x >= -1.) 
	z1.assign(0, ::acos(x));
      else 
	z1.assign(::acosh(-x), M_PI);
    }
    return z1;
  }

  //__________________________________________________________________
  /** Inverse Hyberbolic tangent of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \tanh^{-1} z@f$. */ 
  template <typename T> 
  inline complex<T> arctanh(const complex<T>& z)  
  {
    complex<T> z1(0,0);
    if (z.is_real()) 
      z1 = arctanh(z.real());
    else {
      z1 = arctan(z);
      z1.imag(-1 * z1.imag()); 
    }
    return z1;
  }

  //__________________________________________________________________
  /** inverse Hyberbolic tangent of real number. 
      Mainly for consitency 
      @ingroup complex
      @param x real number. 
      @return @f$ \tanh^{-1} x@f$ */ 
  template <typename T> 
  inline complex<T> arctanh(const T x)  
  {
    complex<T> z1;
    if (x > -1. && x < 1.) 
      z1.assign(::atanh(x), 0);
    else 
      z1.assign(::atanh(1 / x), (x < 0 ? M_PI_2 : -M_PI_2));
    return z1;
  }

  //__________________________________________________________________
  /** Inverse Hyberbolic secant of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \cosh^{-1} 1 / z @f$. */ 
  template <typename T> 
  inline complex<T> arcsech(const complex<T>& z)  
  {
    complex<T> z1 = z.inverse();
    return arccosh(z1);
  }

  //__________________________________________________________________
  /** Inverse Hyberbolic secant of real number. 
      @ingroup complex
      @param x real number. 
      @return @f$ \mbox{csch}^{-1} x@f$. */ 
  template <typename T> 
  inline complex<T> arcsech(const T x)  
  {
    return gslmm::arcsech(complex<T>(x, 0));
  }

  //__________________________________________________________________
  /** Inverse Hyberbolic cosecant of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \sinh^{-1} 1 / z z@f$. */ 
  template <typename T> 
  inline complex<T> arccsch(const complex<T>& z)  
  {
    complex<T> z1 = z.inverse();
    return arcsinh(z1);
  }

  //__________________________________________________________________
  /** Inverse Hyberbolic  cosecant of real number. 
      @ingroup complex
      @param x real number. 
      @return @f$ \csc x@f$. */ 
  template <typename T> 
  inline complex<T> arccsch(const T x)  
  {
    return gslmm::arccsch(complex<T>(x, 0));
  }

  //__________________________________________________________________
  /** Inverse Hyberbolic  cotangent of complex number. 
      @ingroup complex
      @param z Complex number. 
      @return @f$ \tanh^{-1} 1 / z@f$. */ 
  template <typename T> 
  inline complex<T> arccoth(const complex<T>& z)  
  {
    complex<T> z1 = z.inverse();
    return arctanh(z1);
  }

  //__________________________________________________________________
  /** Inverse hyberbolic  cotangent of real number. 
      Mainly for consitency 
      @ingroup complex
      @param x real number. 
      @return @f$ \tanh^{-1} x@f$ */ 
  template <typename T> 
  inline complex<T> arccoth(const T x)  
  {
    return gslmm::arccoth(complex<T>(x,0));
  }
  //@}

  //__________________________________________________________________
  template <typename T> 
  inline 
  std::ostream& 
  operator  <<(std::ostream& o, const gslmm::complex<T>& z) 
  {
    return o <<  "{" << z.real() << "," << z.imag()  << "}";
  }
  
}

#endif
//____________________________________________________________________
//
// EOF
//
