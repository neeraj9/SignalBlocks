//// $Id: histogram.cc,v 1.1 2004-04-16 16:03:15 cholm Exp $ 
//  
//  random/distribution-test.cc
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
# include <iostream>
#endif
#ifndef __STDEXCEPT__
# include <stdexcept>
#endif
#ifndef gslmm_histogram_histogram_hh
# include <gslmm/histogram/histogram.hh>
#endif
#ifndef optionmm_command_line_hh
# include <optionmm/command_line.hh>
#endif
#ifdef HAVE_CONFIG_H
# include "config.hh"
#else 
# define VERSION "?.?"
#endif

/** @file    histogram.cc
    @author  Christian Holm Christensen <cholm@nbi.dk>
    @date    Fri Apr 16 17:07:36 2004
    @brief   Make a 1D histogram of the data input 

    Try for example: 
    @verbatim 
    ./gslmm-randist 1 | \
      ./gslmm-histogram -m -3 -M 3  -n 100 | \
      awk '{print $1, $3 ; print $2, $3}' | \
      graph -T X
    @endverbatim
    To draw a histogram of a guassian distribution with mean 0, and
    spread 1. 
*/
namespace optionmm 
{
  typedef basic_option<double,true,false> double_option;
  typedef basic_option<size_t,true,false> size_option;
}

int
main (int argc, char **argv)
{
  try {
    optionmm::command_line cl("gslmm-histogram",  VERSION, 
			      "Copyright (C) 2004 Christian Holm "
			      "<cholm@nbi.dk>", "", argc, argv);
    optionmm::double_option min_option('m', "minimum", "sets XMIN", 0);
    optionmm::double_option max_option('M', "Maximum", "sets XMAX", 0);
    optionmm::size_option   n_option('n', "bins", "sets N", 0);
    optionmm::bool_option   stat_option('s', "statistics", 
					"Show statistics", false);
    cl.add(min_option);
    cl.add(max_option);
    cl.add(n_option);
    cl.add(stat_option);

    if (!cl.process()) throw std::runtime_error("bad command line");
    if (cl.help()) {
      std::cout << "Histograms data on standard input in a histogram of\n"
		<< "N bins between XMIN and XMAX\n"
		<< "If N is unspecified or 0 then integer bins will be used." 
		<< std::endl;
      return 0;
    }
    cl.version();

    size_t nbins = (n_option.value() == 0 ? 
		    size_t(max_option.value() - min_option.value()) : 
		    n_option.value());
    gslmm::histogram<double,size_t> h(nbins,
				      min_option.value(), 
				      max_option.value());
    
    do {
      double x;
      std::cin >> x;
      if (std::cin.eof()) break;
      if (std::cin.fail())
	throw std::runtime_error("bad input");

      h.fill(x);
    } while (true);

    if (stat_option.value()) {
      double mean  = h.mean();
      double sigma = h.sigma();
      std::cout << "mean  = " << mean << std::endl
		<< "sigma = " << sigma << std::endl;
    }
    h.print(stdout);
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  
  return 0;
}
