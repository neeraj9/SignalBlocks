//
// $Id: error-test.cc,v 1.9 2004-11-06 00:49:46 cholm Exp $ 
//  
//  gslmm::error
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
#include <gslmm/error/error_handler.hh>
#endif
#ifndef GSLMM_test_suite
#include <gslmm/test/test_suite.hh>
#endif
#ifndef __IOSTREAM__
#include <iostream>
#endif

/** @file   error-test.cc
    @author Christian Holm
    @date   Fri Sep 20 13:34:33 2002
    @brief  Test of error handler routines. */


/** A user defined error handler class. */
struct my_error_handler : public gslmm::error_handler<> 
{
  /** Handle an error. 
      @param reason The reason for the error. 
      @param file Where (source code file) the occured. 
      @param line Where (source code line) the occured. 
      @param code The error code. */
  void handle(const std::string& reason, const std::string& file, 
	      int line, int code) {
    std::cout << "This is an error in '" << file << "' line " 
	      << line << ": " << reason << " code: " << code << std::endl;
    gslmm::test_suite::instance().check(true, 
					"gslmm::error_handler<>::install() " 
					"installs new handler");
  }
};

/** Test of error handler  
    @param argc command line arguments. 
    @param argv command line arguments. 
    @return 0 on success, number of errors otherwise. */
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("error", argc, argv);

  std::cout << "Only lines beginning with \"FAIL:\" are errors" << std::endl;

  /// Turn off error handling. 
  gslmm::error_handler<>::instance().off();
  
  /// set an error 
  gslmm::error_handler<>::instance().error("hello", __FILE__, __LINE__, 0);
  ts.check(true, "gslmm::error_handler<>::off() turns off error handling");

  /// Turn off error handling. 
  gslmm::error_handler<>::instance().on();

  /// Set an error with a type 
  gslmm::error_handler<>::instance().error("hello", __FILE__, __LINE__, 0);
  ts.check(true, "gslmm::error_handler<>::on() turns off error handling");

  /// Use a user defined error handler. 
  gslmm::error_handler<>::instance().install<my_error_handler>();

  /// Use a macro 
  gslmm::error_handler<>::instance().error("hello", __FILE__, __LINE__, 0);

  /// Reset to default
  gslmm::error_handler<>::instance().remove();
  ts.check(true, "gslmm::error_handler<>::remove() restores handler");

  /// Use a macro 
  gslmm::error_handler<>::instance().error("hello", __FILE__, __LINE__, 0);

#if 0
  // Try the exception one - doesn't work, 'cause gsl_error has a
  // `throw()' qualifier - sigh! 
  try {
    gslmm::error_handler<>::instance().install<gslmm::exception_policy>();
    gslmm::error_handler<>::instance().error("exception", 
					     __FILE__, __LINE__, 0);
  }
  catch(std::exception& e) {
    ts.check(true, "gslmm::error_handler<>::error() can throw");
    std::cerr << e.what() << std::endl;
  }
#endif

  // Use macro
  GSL_ERROR("fubar", (ts.summary() ? 0 : 1));
  return 0;
}

  
//
// EOF
//
