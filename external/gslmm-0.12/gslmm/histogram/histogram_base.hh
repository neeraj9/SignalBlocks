//
// $Id: histogram_base.hh,v 1.3 2004-03-14 04:45:41 cholm Exp $ 
//  
//  gslmm::histogram_base
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
#ifndef GSLMM_histogram_base
#define GSLMM_histogram_base

/** @file   histogram/histogram_base.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  histogram class */

namespace gslmm 
{
  /** @class histogram histogram_base.hh <gslmm/histogram/histogram.hh>
      @brief Base template for histogram classes. 
      
      1D and 2D histogram are defined via the specialisations 
      gslmm::histogram<double,size_t> and 
      gslmm::histogram<std::pair<double,double>,std::pair<size_t,size_t> >.  
      Higher dimensional histograms could easily be defined in terms of a
      specialisation like 
      gslmm::histogram<std::vector<double>,std::vector<size_t> >
      
      @ingroup histogram
      @param Value The bin content and axis type 
      @param Bin The bin index type
  */
  template <typename Value, typename Bin>
  class histogram
  {};
  
}

#endif
//____________________________________________________________________
//
// EOF
//
