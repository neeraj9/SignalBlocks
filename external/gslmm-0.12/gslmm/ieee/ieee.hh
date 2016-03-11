//
// $Id: ieee.hh,v 1.6 2005-05-31 21:02:22 cholm Exp $ 
//  
//  gslmm::ieee
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
#ifndef GSLMM_ieee
#define GSLMM_ieee
#ifndef __GSL_IEEE_UTILS_H__
# include <gsl/gsl_ieee_utils.h>
#endif
#ifndef GSLMM_util
# include <gslmm/gslmm-util.hh>
#endif
#ifndef __STRING__
# include <string>
#endif


/** @file   ieee.hh
    @author Christian Holm
    @date   Wed Mar 05 12:17:10 2003
    @brief  Set up the IEEE floating point environment */
/** @defgroup ieee Floating point number representation */
namespace gslmm
{
  /** @class ieee ieee.hh <gslmm/ieee/ieee.hh>
      Set up the IEEE floating point environment. 
      @param Lock Lock type to lock singleton for thread safe access. 
      @ingroup ieee 
  */
  template <typename Lock=GSLMM_LOCK>
  class ieee
  {
  public:
    /// 
    typedef Lock lock_type;
    /// Precision mode
    enum {
      single_precision      = GSL_IEEE_SINGLE_PRECISION,
      double_precision      = GSL_IEEE_DOUBLE_PRECISION,
      extended_precision    = GSL_IEEE_EXTENDED_PRECISION
    };
    /// Rounding mode 
    enum {
      round_to_nearest      = GSL_IEEE_ROUND_TO_NEAREST,
      round_down            = GSL_IEEE_ROUND_DOWN,
      round_up              = GSL_IEEE_ROUND_UP,
      round_to_zero         = GSL_IEEE_ROUND_TO_ZERO
    };
    /// Exception bits 
    enum {
      mask_invalid          = GSL_IEEE_MASK_INVALID, 
      mask_denormalized     = GSL_IEEE_MASK_DENORMALIZED,
      mask_division_by_zero = GSL_IEEE_MASK_DIVISION_BY_ZERO, 
      mask_overflow         = GSL_IEEE_MASK_OVERFLOW, 
      mask_underflow        = GSL_IEEE_MASK_UNDERFLOW, 
      mask_all              = GSL_IEEE_MASK_ALL, 
      trap_inexact          = GSL_IEEE_TRAP_INEXACT
    };
  protected:
    /// 
    static lock_type _lock;
    /// 
    static ieee* _instance;
    /** Constructor. */
    ieee() {}
    /** Constructor. */
    ieee(const ieee<lock_type>& o) {}
    /** Assignment */
    ieee<lock_type>& operator=(const ieee<lock_type>& o) { return *this; }
  public:
    /** Destructor. */
    virtual ~ieee() {}
    /** Get ingleton */
    static ieee<lock_type>& instance();
    /** Setup from the environment. */
    void setup();
    /** Setup from string argument. 
	Argument should be a string consisting of the various modes,
	seperated by commas.  The modes are the same as the enums of
	this class, except that @c _ is replaced by @c -. */
    void setup(const std::string& options);
    /** Setup from explicit arguments. 
	@param precision One of gslmm::ieee::single_precision, 
	gslmm::ieee::double_precision, or
	gslmm::ieee::extended_precision.
	@param rounding one of gslmm::ieee::round_to_nearest, 
	gslmm::ieee::round_down, gslmm::ieee::round_up, 
	gslmm::ieee::round_to_zero. 
	@param exception_mask a bit pattern (@c | combination) of 
	gslmm::ieee::mask_invalid, gslmm::ieee::mask_denormalized, 
	gslmm::ieee::mask_divisin_by_zero, gslmm::ieee::mask_overflow, 
	gslmm::ieee::mask_underflow, and
	gslmm::ieee::trap_inexact. The value gslmm::ieee::mask_all, is
	the same as specifiying all options, except
	gslmm::ieee::trap_inexact. */
    void setup(int precision, int rounding, int exception_mask);
  };
  //__________________________________________________________________
  template <typename Lock> ieee<Lock>* ieee<Lock>::_instance = 0;
  //__________________________________________________________________
  template <typename Lock> Lock ieee<Lock>::_lock;
  //__________________________________________________________________
  template <typename Lock> 
  inline ieee<Lock>& ieee<Lock>::instance() 
  {
    if (!_instance) {
      _lock.lock();
      if (!_instance) 
	_instance = new ieee<Lock>();
      _lock.unlock();
    }
    return *_instance;
  }
  //__________________________________________________________________
  template <typename Lock> 
  inline void ieee<Lock>::setup() 
  {
    _lock.lock();
    gsl_ieee_env_setup();
    _lock.unlock();
  }
  //__________________________________________________________________
  template <typename Lock> 
  inline void ieee<Lock>::setup(const std::string& options) 
  {
    _lock.lock();
    int p, r, m;
    gsl_ieee_read_mode_string(options.c_str(), &p, &r, &m);
    setup(p, r, m);
    _lock.unlock();
  }
  //__________________________________________________________________
  template <typename Lock> 
  inline void ieee<Lock>::setup(int p, int r, int m) 
  {
    _lock.lock();
    gsl_ieee_set_mode(p, r, m);
    _lock.unlock();
  }
}

#endif
//____________________________________________________________________
//
// EOF
//
