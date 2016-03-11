//
// $Id: blas.hh,v 1.5 2006-02-08 00:06:05 cholm Exp $ 
//  
//  gslmm::blas
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
#ifndef GSLMM_blas
#define GSLMM_blas

/** @file   blas/blas.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  blas class */

/** @defgroup blas Basic Linear Algebra Subprograms (BLAS)
    @todo Implement the rest of the tests of these functions and
    structures.   Use these function in the matrix and vector modules
    for matrix-vector and matrix-matrix products. */

#ifndef GSLMM_blas_blas_base_hh
# include <gslmm/blas/blas_base.hh>
#endif
#ifndef GSLMM_blas_blas_float_hh
# include <gslmm/blas/blas_float.hh>
#endif
#ifndef GSLMM_blas_blas_double_hh
# include <gslmm/blas/blas_double.hh>
#endif
#ifndef GSLMM_blas_blas_complex_double_hh
# include <gslmm/blas/blas_complex_double.hh>
#endif

#endif
//____________________________________________________________________
//
// EOF
//
