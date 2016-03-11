//
// $Id: histogram-ex.cc,v 1.1 2004-04-13 14:52:05 cholm Exp $ 
//  
//  histogram/histogram-test.cc
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
#ifndef __IOSTREAM__
#include <iostream>
#endif
#ifndef __IOMANIP__
#include <iomanip>
#endif
#ifndef __SSTREAM__
#include <sstream>
#endif
#ifndef GSLMM_histogram
#include <gslmm/histogram/histogram.hh>
#endif

/** @file   histogram-ex.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Example of histogram classses.  */


int 
main(int argc, char** argv) 
{
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " XMIN XMAX N" << std::endl
	      << "Computes a histogram of the data on stdin using" << std::endl
	      << "N bins from XMIN to XMAX" << std::endl;
    return 1;
  }
  
  double a, b;
  size_t n;

  std::stringstream s1(argv[1]); s1 >> a;
  std::stringstream s2(argv[2]); s2 >> b;
  std::stringstream s3(argv[3]); s3 >> n;
  
  
  gslmm::histogram<double,size_t> h(n, a, b);
  do {
    double x;
    std::cin >> x;
    if (std::cin.eof()) break;
    if (std::cin.fail()) return 1;
    h.fill(x);
  } while (true);
  
  h.print(stdout);
  
  return 0;
}

//
// EOF
//
    
    
