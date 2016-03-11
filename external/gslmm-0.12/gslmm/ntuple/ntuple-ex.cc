//
// $Id: ntuple-ex.cc,v 1.6 2006-05-01 14:25:33 cholm Exp $ 
//  
//  ntuple/ntuple-test.cc
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
#ifndef GSLMM_math_type_trait
#include <gslmm/math/type_trait.hh>
#endif
#ifndef GSLMM_ntuple
#include <gslmm/ntuple/ntuple.hh>
#endif
#ifndef GSLMM_generator
# include <gslmm/random/generator.hh>
#endif
#ifndef GSLMM_gaussian
# include <gslmm/random/gaussian.hh>
#endif
#ifndef GSLMM_histogram
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

/** @file   ntuple-ex.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of ntuple classses.  

    Try running:
    @verbatim 
    ./ntuple-ex  -c 1 | awk '{ print $1, $3; print $2, $3 }' | graph -T X
    @endverbatim
    Try different values for the cut (specified by the optoin `-c')
*/

//____________________________________________________________________  
struct data 
{
  size_t i;
  double x;
  double y;
  double z;
};


//____________________________________________________________________  
void out(const std::string& file) 
{
  data row;
  gslmm::ntuple<data> n(file,&row,gslmm::ntuple<data>::create);
  
  gslmm::generator::env_setup();
  gslmm::generator rg;
  
  gslmm::gaussian g(1, rg);
  
  for (size_t i = 0; i < 10000; i++) {
    row.i = i;
    row.x = g.sample();
    row.y = g.sample();
    row.z = g.sample();

    n.fill();
  }
  n.close();
}

//____________________________________________________________________  
struct selector 
{
  double _scale;
  selector(double s=1.5) : _scale(s) {}
  bool operator()(const data& d) const
  {
    double e2 = d.x * d.x + d.y * d.y + d.z * d.z;
    return (e2 > _scale);
  }
};

//____________________________________________________________________  
struct value 
{
  double _scale;
  value(double s=1.5) : _scale(s) {}
  double operator()(const data& d) const
  {
    double e2 = d.x * d.x + d.y * d.y + d.z * d.z;
    return e2;
  }
};

//____________________________________________________________________  
void in(float cut, const std::string& file)
{
  data row;
  gslmm::ntuple<data> n(file,&row,gslmm::ntuple<data>::open);
  gslmm::histogram<double,size_t> h(100, cut, 10);
  value v;
  selector s(cut);
  
  n.project(h, v, s);
  
  h.print(stdout);
  
  n.close();
}


//____________________________________________________________________  
int main(int argc, char** argv) 
{
  // bool write = true;
  optionmm::command_line cl("ntuple-ex",  VERSION, 
  	  		    "Copyright (C) 2004 Christian Holm <cholm@nbi.dk>",
			    "",
  	  		    argc, argv);
  optionmm::string_option   out_option('o', "output", 
  	  			       "Output file", "ntuple-ex.dat");
  optionmm::string_option   in_option('i', "input", 
				       "input file", "ntuple-ex.dat");
  optionmm::float_option    cut_option('c', "cut", 
				       "Cut to use", 1.5);
  cl.add(in_option);
  cl.add(out_option);
  cl.add(cut_option);
  if (!cl.process()) throw std::runtime_error("bad command line");
  if (cl.help()) {
    std::cout << std::endl
	      << "Run the program like\n\n\t" << cl.program_name() 
	      << " -c <cut> | awk '{ print $1, $3; print $2, $3}' | "
	      << "graph -T X\n\nto produce a plot of the output" 
	      << std::endl;
    return 0;
  }
  cl.version();
  
  if (out_option.value().size() > 0) out(out_option.value());
  if (in_option.value().size() > 0) in(cut_option.value(),in_option.value());
  return 0;  
}

//____________________________________________________________________  
//
// EOF
//  
