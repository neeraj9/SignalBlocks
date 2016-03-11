//
// $Id: error_handler.hh,v 1.13 2006-04-19 09:32:02 cholm Exp $ 
//  
//  gslmm::error_handler
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
#ifndef GSLMM_error_handler
#define GSLMM_error_handler
#ifndef __IOSTREAM__
#include <iostream>
#endif
#ifndef __STRING__
#include <string>
#endif
#ifndef __SSTREAM__
#include <sstream>
#endif
#ifndef __STDEXCEPT__
#include <stdexcept>
#endif
#ifndef __CINT__
# ifndef __GSL_ERRNO_H__
#  include <gsl/gsl_errno.h>
# endif
#endif
#ifndef GSLMM_util
#include <gslmm/gslmm-util.hh>
#endif

/**  @file   error/error_handler.hh
     @author Christian Holm
     @date   Fri Sep 20 13:39:10 2002
     @brief  Declaration of gslmm::error_handler.  */

/** @defgroup error Error Handling */
namespace gslmm
{  
  //_____________________________________________________________________
  /** @class cerr_policy error_handler.hh <gslmm/error/error_handler.hh>
      An error handling policy.  
      Just prints errors to @c std::cerr, and exists by calling 
      @c abort 
      @ingroup error 
  */
  class cerr_policy 
  {
  public:
    /** Handle an error.  Write to standard error and exit by calling 
	@c abort(). 
	@param reason Reason for the error 
	@param file In which file the error occured
	@param line on which code line the error occured 
	@param code The error code. */
    void handle(const std::string& reason, 
		const std::string& file, 
		int line, int code) {
      std::cerr << "Error in '" << file << "' line " << line << ": " 
		<< gsl_strerror(code) << ": " << reason << std::endl;
      if (code) abort();
    }
  };
  
  //_____________________________________________________________________
  /** @class empty_policy error_handler.hh <gslmm/error/error_handler.hh>
      An error handling policy. 
      Does nothing.
      @ingroup error */
  class empty_policy 
  {
  public:
    /** Handle an error.  Does nothing.
	@param reason Reason for the error 
	@param file In which file the error occured
	@param line on which code line the error occured 
	@param code The error code. */
    void handle(const std::string& reason, 
		const std::string& file, 
		int line, int code) 
    {
      (void)reason;
      (void)file;
      (void)line;
      (void)code;
    }
  };
  
  //_____________________________________________________________________
  /** @class exception_policy error_handler.hh <gslmm/error/error_handler.hh>
      An error handling policy. 
      Unfortunately, this does work, as the @c gsl_error function is
      specified as @e not throwing exceptions.  Really annoying. 
      @ingroup error 
  */
  class exception_policy 
  {
  public:
    /** Handle an error.  Throws an exception with the appropiate
	error string. 
	@param reason Reason for the error 
	@param file In which file the error occured
	@param line on which code line the error occured 
	@param code The error code. 
	@exception std::runtime_error Always thrown. */
    void handle(const std::string& reason, 
		const std::string& file, 
		int line, int code) {
      std::stringstream s;
      s << "Error in '" << file << "' line " << line << ": " 
	<< gsl_strerror(code) << ": " << reason;
      throw std::runtime_error(s.str());
    }
  };

  template <typename Policy, typename Lock> class error_handler;
  
  //_____________________________________________________________________
  /** Friend function to access the error handler. 
      @param reason Reason for the error 
      @param file In which file the error occured
      @param line on which code line the error occured 
      @param code The error code. */
  template <typename Policy, typename Lock>
  void error_handler_callback(const char* reason, const char* file, 
			      int line, int code) 
  {
    if (error_handler<Policy,Lock>::instance().is_on())
      error_handler<Policy,Lock>::instance().handle(reason,file,line,code); 
  }

  //_____________________________________________________________________
  /** @class error_handler error_handler.hh <gslmm/error/error_handler.hh>
      Error handling class. 
      Per default, the native GSL error handling is used.  To use this
      interface, simply execute the member function 
      gslmm::error_handler<Policy,Lock>::instance() with the
      appropiate template arguments.  For example 
      @code 
      gslmm::error_handler<gslmm::cerr_policy,mt_lock>::instance(); 
      @endcode 
      and this interface will handle all errors.  Hence, this is a
      good candidate for something you'll do as one of the first
      things in your @c main function. 
      @ingroup error
      @param Policy The default policy to use.  Now policies can be
      installed via the member function gslmm::error_handler::install,
      and removed again with gslmm::error_handler::remove. 
      @param Lock The locking machanism used for multi-threaded
      thread-safe access to these services. */ 
  template <typename Policy=cerr_policy, 
	    typename Lock=single_thread_lock>
  class error_handler 
  {
  public:
    //_____________________________________________________________________
    /** Base class for policy adaptors.  
	This class is needed, so that the error_handler class can have
	a member that can point to various adaptors. */
    struct policy_base
    {
      virtual ~policy_base() {}
      /// Interface that must be implemented. 
      virtual void handle(const std::string& reason, 
			  const std::string& file, 
			  int line, int code) = 0;
    };

    //_____________________________________________________________________
    /** Generic policy adaptors.  
	This class adapts a policy to a policy_base interface.  The
	class is a templated class with the policy as the template 
	parameter.  In this way, a user may use any kind of class to
	do the error handling. 
	@param PolicyType The policy class to forward to. */
    template <typename PolicyType>
    struct basic_policy : public policy_base
    {
      /// An instance of the policy type 
      PolicyType _policy;
      /// CTOR
      basic_policy() : _policy() {}
      /// CTOR
      virtual ~basic_policy()  {}
      /// Forward calls to the policy class 
      virtual void handle(const std::string& reason, 
			  const std::string& file, 
			  int line, int code) {
	_policy.handle(reason,file,line,code);
      }
    };
    /// The type of thread locks. 
    typedef Lock lock_type;
  protected:
    /// The lock to ensure thread-safety
    static Lock _lock;
    /// The static instance of this class 
    static error_handler<Policy,Lock>* _instance;
    /// The static instance of this class 
    policy_base* _default;
    /// The static instance of this class 
    policy_base* _current;
    /// Wether error handling is on or off
    bool _is_on;
    /// Protected copy constructor 
    error_handler(const error_handler<Policy,Lock>& other) {}
    /// Protected assignmen operator 
    error_handler<Policy,Lock> 
    operator=(const error_handler<Policy,Lock>& other) { return *this; }    
  public:
    /** Contrsuctor */
    error_handler();
    /** Contrsuctor */
    virtual ~error_handler() {}
    /** Get reference to static instance. */
    static error_handler<Policy,Lock>& instance();
    /** Check if error_handling is on. */
    bool is_on() { return _is_on; }
    /** Turn error handling on. */
    void on() { guard<lock_type> g(_lock); _is_on = true; }
    /** Turn error handling off. */
    void off() { guard<lock_type> g(_lock); _is_on = false; }
    /** Helper function to turn an GSL error code into a string. 
	@param code The error code. 
	@return a string describing the error. */ 
    static std::string code_to_string(int code);
    /** Helper function to set an error condition. 
	@param reason Reason for the error 
	@param file In which file the error occured
	@param line on which code line the error occured 
	@param code The error code. */
    void error(const std::string& reason, 
	       const std::string& file, 
	       int line, int code);
    /** Install a new error_handler policy as the error handler. 
	All errors will be handled by this policy until it is
	removed by gslmm::error_handler::remove. */
    template <typename PolicyType>
    void install();
    /** Remove the current error handler policy.  It restores the
	default error handler. */
    void remove();
    /** Handle an error.  Is implmented in gslmm::error_handler
	@param reason Reason for the error 
	@param file In which file the error occured
	@param line on which code line the error occured 
	@param code The error code. */
    virtual void handle(const std::string& reason, 
			const std::string& file, 
			int line, int code) { 
      _current->handle(reason,file,line,code); 
    }
  };
//_____________________________________________________________________
  template <typename Policy, typename Lock>
  inline error_handler<Policy,Lock>::error_handler() 
    : _default(0), _current(0), _is_on(true)
  {
    // typedef void (*callback_type)(const char*, const char*, int, int);
    gsl_set_error_handler(error_handler_callback<Policy, Lock>);
    _current = _default = new basic_policy<Policy>();
  }

  //_____________________________________________________________________
  template <typename Policy, typename Lock>
  inline error_handler<Policy,Lock>& 
  error_handler<Policy,Lock>::instance()
  {
    if (!_instance) {
      guard<lock_type> g(_lock);
      if (!_instance)  
	_instance = new error_handler<Policy,Lock>();
    }
    return *_instance;
  }
  
  //_____________________________________________________________________
  template <typename Policy, typename Lock>
  inline std::string
  error_handler<Policy,Lock>::code_to_string(int code) 
  {
    return std::string(gsl_strerror(code));
  }

  //_____________________________________________________________________
  template <typename Policy, typename Lock>
  inline void
  error_handler<Policy,Lock>::error(const std::string& reason, 
			     const std::string& file, 
			     int line, int code) 
  { 
    gsl_error(reason.c_str(), file.c_str(), line, code);
  }

  //_____________________________________________________________________
  template <typename Policy, typename Lock>
  template <typename PolicyType>
  inline void
  error_handler<Policy,Lock>::install() 
  {
    guard<lock_type> g(_lock);
    _current = new basic_policy<PolicyType>();
  }

  //_____________________________________________________________________
  template <typename Policy, typename Lock>
  inline void
  error_handler<Policy,Lock>::remove() 
  {
    guard<lock_type> g(_lock);
    if (_current == _default) return;
    delete _current;
    _current = _default;
  }

  //_____________________________________________________________________
  template <typename Policy, typename Lock>
  error_handler<Policy,Lock>* error_handler<Policy,Lock>::_instance = 0;
  //_____________________________________________________________________
  template <typename Policy, typename Lock>
  Lock error_handler<Policy,Lock>::_lock;
}

#if 0
#ifdef GSL_ERROR
#undef GSL_ERROR
#endif
#ifdef GSL_ERROR_VAL
#undef GSL_ERROR_VAL
#endif
/** Redefine GSL_ERROR_VAL to call the static member function of the
    error_handler class, rather than the C API function directly. 
    @ingroup error 
    @param r The reason for the error,
    @param c The error code, 
    @param v The return value. 
*/
#define GSL_ERROR_VAL(r,c,v) \
  do { gslmm::error_handler<GSLMM_LOCK>::instance().error(r,__FILE__,__LINE__,c);  \
       return v; } while(0)
/** Redefine GSL_ERROR to call the static member function of the
    error_handler class, rather than the C API function directly. 
    @param r The reason for the error,
    @param c The error code
    @ingroup error 
*/ 
#define GSL_ERROR(r,c) GSL_ERROR_VAL(r,c,c)
#endif
#endif
//____________________________________________________________________
//
// EOF
//
