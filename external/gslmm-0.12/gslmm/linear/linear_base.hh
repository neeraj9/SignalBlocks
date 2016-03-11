//
// $Id: linear_base.hh,v 1.6 2006-05-09 07:40:34 cholm Exp $ 
//  
//  gslmm::linear_base
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
#ifndef GSLMM_linear_linear_base
#define GSLMM_linear_linear_base

/** @file   linear/linear_base.hh
    @author Christian Holm
    @date   Mon Mar 10 14:50:12 2003
    @brief  Base template for matricies classes */

namespace gslmm
{
  //__________________________________________________________________
  /** @defgroup lu_decomposition_group LU decomposition
      @ingroup linear 
   */
  /** @struct lu_decomposition linear_base.hh <gslmm/linear/linear_base.hh>
      @brief Basic template for LU decompositions of matricies.  
      @ingroup lu_decomposition_group 
   */ 
  template <typename T> struct lu_decomposition;

  //__________________________________________________________________
  /** @defgroup lq_decomposition_group LQ decomposition
      @ingroup linear 
   */
  /** @struct lq_decomposition linear_base.hh <gslmm/linear/linear_base.hh>
      @brief Basic template for LQ decompositions of matricies.  
      @ingroup lq_decomposition_group 
   */ 
  template <typename T> struct lq_decomposition;
  /** @struct lq_decomposition_pivot linear_base.hh <gslmm/linear/linear_base.hh>
      @brief Basic template for LQ decompositions with pivot of matricies.  
      @ingroup lq_decomposition_group 
   */ 
  template <typename T> struct lq_decomposition_pivot;

  //__________________________________________________________________
  /** @defgroup qr_decomposition_group QR decomposition
      @ingroup linear 
   */
  /** @struct qr_decomposition linear_base.hh <gslmm/linear/linear_base.hh>
      @brief Basic template for QR decompositions of matricies.  
      @ingroup qr_decomposition_group 
   */ 
  template <typename T> struct qr_decomposition;
  /** @struct qr_decomposition_pivot linear_base.hh <gslmm/linear/linear_base.hh>
      @brief Basic template for QR decompositions with pivot of matricies.  
      @ingroup qr_decomposition_group 
   */ 
  template <typename T> struct qr_decomposition_pivot;

  //__________________________________________________________________
  /** @defgroup sv_decomposition_group Single value decomposition
      @ingroup linear 
   */
  /** @struct sv_decomposition linear_base.hh <gslmm/linear/linear_base.hh>
      @brief Basic template for SV decompositions of matricies.  
      @ingroup sv_decomposition_group 
   */ 
  template <typename T> struct sv_decomposition;

  //__________________________________________________________________
  /** @defgroup cholesky_decomposition_group Cholesky decomposition
      @ingroup linear 
   */

  /** @struct cholesky_decomposition linear_base.hh <gslmm/linear/linear_base.hh>
      @brief Basic template for Cholesky decompositions of matricies.  
      @ingroup cholesky_decomposition_group 
   */ 
  template <typename T>
  struct cholesky_decomposition;
}

#endif
//____________________________________________________________________
//
// EOF
//
