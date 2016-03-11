//
// $Id: linear.hh,v 1.8 2006-05-09 07:40:34 cholm Exp $ 
//  
//  gslmm::linear
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
#ifndef GSLMM_linear_linear
#define GSLMM_linear_linear

/** @file   linear/linear.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  linear class */

/** @defgroup linear Linear Algebra
    @todo Have implemented 
    - LU-decompostion
    - LQ-decompostion
    - PTLQ-decompostion
    - QR-decompostion
    - QRPT-decompostion
    - SV-decompostion
    - Cholesky-decompostion

    implement the rest these classes, that is

    - Tridiagnoal decomposition for real-symmetric/Hermitian matrices
    - Bi-diagonalization 
    - Householder transforms 
    - Householder solver for linear systems 
    - Tri-diagonal systems 
*/ 

#ifndef GSLMM_linear_lu_decomposition_double_hh
# include <gslmm/linear/lu_decomposition_double.hh>
#endif
#ifndef GSLMM_linear_lu_decomposition_complex_double_hh
# include <gslmm/linear/lu_decomposition_complex_double.hh>
#endif
#ifndef GSLMM_linear_lq_decomposition_double_hh
# include <gslmm/linear/lq_decomposition_double.hh>
#endif
#ifndef GSLMM_linear_lq_decomposition_pivot_double_hh
# include <gslmm/linear/lq_decomposition_pivot_double.hh>
#endif
#ifndef GSLMM_linear_qr_decomposition_double_hh
# include <gslmm/linear/qr_decomposition_double.hh>
#endif
#ifndef GSLMM_linear_qr_decomposition_pivot_double_hh
# include <gslmm/linear/qr_decomposition_pivot_double.hh>
#endif
#ifndef GSLMM_linear_sv_decomposition_double_hh
# include <gslmm/linear/sv_decomposition_double.hh>
#endif
#ifndef GSLMM_linear_cholesky_decomposition_double_hh
# include <gslmm/linear/cholesky_decomposition_double.hh>
#endif

#endif
//____________________________________________________________________
//
// EOF
//
