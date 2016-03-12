//
// $Id: statistic_base.hh,v 1.2 2006-02-08 00:08:50 cholm Exp $ 
//  
//  gslmm::statistic
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
#ifndef GSLMM_statistic_base
#define GSLMM_statistic_base

/** @file   statistic/statistic_base.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  statistic class */

namespace gslmm 
{
  /** @class dataset /statistic_base.hh <gslmm/statistic/statitic.hh>
      @brief Class template for statistics on numbers
      @ingroup statistic
   */
  template <typename T>
  class dataset
  {};

  /** @class weighted_dataset statistic_base.hh <gslmm/statistic/statitic.hh>
      @brief Class template for statistics on numbers
      @ingroup statistic
   */
  template <typename T>
  class weighted_dataset : public dataset<T>
  {};
  
  
}

#endif
//____________________________________________________________________
//
// EOF
//