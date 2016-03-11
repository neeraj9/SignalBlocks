//
// $Id: divide_difference_skel.hh,v 1.8 2006-02-08 00:08:31 cholm Exp $ 
//  
// WARNING: automatically generated by @progname@
//          options: @skel@, @type@.  
//          Do not edit this file.
//
//  gslmm::divide_difference@name@.hh
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
#ifndef GSLMM_divide_difference@name@
#define GSLMM_divide_difference@name@

/** @file   divide_difference@name@.hh
    @author Christian Holm
    @date   Sat Mar 01 12:56:22 2003
    @brief  Newtons divide difference representation of a polynomial - 
    @type@ specialisation. */

#ifndef GSL_divide_difference_base
#include <gslmm/polynomial/divide_difference_base.hh>
#endif  


namespace gslmm
{
  /** @brief Newtons divide difference representation of a polynomial -
      @type@ specialisation. 
      @ingroup polynomial
   */
  template <>
  class divide_difference<@type@> : public polynomial<@type@> 
  {
  public:
    typedef polynomial<@type@> base_type;
    /// The type of the return values. 
    typedef polynomial<@type@>::value_type    value_type;
    /// The type of the argument values. 
    typedef polynomial<@type@>::argument_type argument_type;
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
      return (c ? _p[i] : _p[i]); }
    /** Get a coefficent */
    virtual value_type coefficent(size_t i) const { return _p[i]; }

    /** Get the dd array. */
    /// const std::vector<double> dd() const { return _p; } 
    /** Get the xa array. */
    /// const std::vector<double>& xa() const { return _xa; } 
    /** Get the dd array. */
    const value_type* dd() const { return &(_p[0]); } 
    /** Get the xa array. */
    const value_type* xa() const { return &(_xa[0]); } 

    /** Get the size of the polynomial */
    size_t size() const { return _p.size(); }
  };
  //__________________________________________________________________
  inline
  divide_difference<@type@>::divide_difference(const value_type* xa, 
					       const value_type* ya, 
					       size_t n) 
    : base_type(n), _xa(n)
  {
    initialize(xa,ya,n);
  }
  //__________________________________________________________________
  inline 
  divide_difference<@type@>::divide_difference(const value_type xa0,   
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
    : base_type(10), _xa(10)
  {
    initialize(xa0, ya0, xa1, ya1, xa2, ya2, xa3, ya3, xa4, ya4,
	       xa5, ya5, xa6, ya6, xa7, ya7, xa8, ya8, xa9, ya9);
  }
  //__________________________________________________________________
  inline
  divide_difference<@type@>::divide_difference(const std::vector<value_type>& 
					       xa,  
					       const std::vector<value_type>& 
					       ya) 
    : base_type(xa.size()), _xa(xa.size())
  {
    initialize(xa, ya);
  }
  //__________________________________________________________________
  inline const std::vector<@type@>& 
  divide_difference<@type@>::coefficents(const value_type* p,size_t n) 
  { 
    (void)n;
    (void)p;
    return _p;
  }
    

  //__________________________________________________________________
  inline const std::vector<@type@>& 
  divide_difference<@type@>::coefficents(const std::vector<value_type>& p) 
  { 
    (void)p;
    return _p;
  }
    
  //__________________________________________________________________
  inline const std::vector<@type@>& 
  divide_difference<@type@>::coefficents(const value_type p0,   
					 const value_type p1,   
					 const value_type p2, 
					 const value_type p3, 
					 const value_type p4, 
					 const value_type p5, 
					 const value_type p6, 
					 const value_type p7, 
					 const value_type p8, 
					 const value_type p9) 
  {
    // shut up GCC when -Wall -W -Werror
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    (void)p6;
    (void)p7;
    (void)p8;
    (void)p9;
    return _p;
  }
  //__________________________________________________________________
  inline @type@
  divide_difference<@type@>::evaluate(const argument_type& x) 
  {
    return gsl_poly@low@_dd_eval(&(_p[0]), &(_xa[0]), _p.size(), x); 
  }
  //__________________________________________________________________
  inline void
  divide_difference<@type@>::initialize(const value_type* xa, 
					const value_type* ya, 
					size_t n) 
  {
    _p.resize(n);
    _xa.resize(n);
    gsl_poly@low@_dd_init(&(_p[0]),xa,ya,n);
    for (size_t i = 0; i < n; i++) _xa[i] = xa[i];
  }
  //__________________________________________________________________
  inline void
  divide_difference<@type@>::initialize(const value_type xa0,   
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
    _xa.resize(10);   std::vector<value_type> ya(10);
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
    for (int i = 9; i >= 0; i++, d--) 
      if (_xa[i] != 0 && ya[i] != 0) break;
    initialize(&(_xa[0]), &(ya[0]), d);
  }
  //__________________________________________________________________
  inline void
  divide_difference<@type@>::initialize(const std::vector<value_type>& xa,  
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
