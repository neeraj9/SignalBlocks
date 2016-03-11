//
// $Id: blas_base.hh,v 1.12 2006-05-09 07:40:34 cholm Exp $ 
//  
//  gslmm::blas_base
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
#ifndef GSLMM_blas_base_hh
#define GSLMM_blas_base_hh
/** @file   blas_base.hh
    @author Christian Holm
    @date   Mon Mar 10 14:50:12 2003
    @brief  Basic declarations for BLAS module */

#ifndef GSLMM_vectormatrix_vector_base
# include <gslmm/vectormatrix/vector_base.hh>
#endif
#ifndef GSLMM_vectormatrix_matrix_base
# include <gslmm/vectormatrix/matrix_base.hh>
#endif
#ifndef __GSL_CBLAS_H__
# include <gsl/gsl_cblas.h>
#endif

namespace gslmm
{
  //=================================================================
  // BLAS Level 1
  /** @defgroup blas_lvl1 Level 1.  
      @ingroup blas 
      General operations on vectors
  */
  //_________________________________________________________________
  /** Compute the inner product of two vectors.  
      That is @f$ v^T u@f$.  However, if the vectors are complex, and
      if @a herm is @c true then @f$ v^H u@f$ is returned. 
      @ingroup blas_lvl1
      @param v first vector 
      @param u second vector 
      @param herm Whether the first vector is hermitian conjugated 
      @return  @f$ v^T u@f$ or @f$ v^H u@f$, if @a herm is @c true */
  template <typename Type>
  Type 
  dot(const vector<Type>& v, const vector<Type>& u, bool herm=false);

#if 0
  /** computes the sum @f$ a + v^T u @f$ for the vectors @a v and @a u
      @ingroup blas_lvl1
      @param a Constant 
      @param v first vector 
      @param u second vector 
      @return @f$ a + v^T u @f$ */
  template <>
  float 
  dot(float a, const vector<float>& v, const vector<float>& u) 
  {
    float r;
    int re = gsl_blas_sdsdot(a, v._vector, u._vector, r);
    return r;
  }
#endif

  //_________________________________________________________________
  /** Compute the Euclidean norm of the vector @a v 
      @f$ ||v||_2 = \sqrt {\sum v_i^2}@f$ or 
      @f$ ||v||_2 = \sqrt {\sum (\Re(v_i)^2 + \Im(v_i)^2)} @f$
      @ingroup blas_lvl1
      @param v The vector to compute the norm of
      @return  The Euclidean norm, or 2-norm */
  template <typename Type>
  Type
  norm(const vector<Type>& v);

  /** Compute the aboslute sum of the vector @a v 
      @f$ \sum |v_i|@f$ or 
      @f$ \sum |\Re(v_i)| + |v_i| @f$
      @ingroup blas_lvl1
      @param v The vector to absolute sum of
      @return  The absolute sum, or 1-norm */
  template <typename Type>
  typename vector<Type>::element_type
  absolute_sum(const vector<Type>& v);

  //_________________________________________________________________
  /** These functions return the index of the largest element of the
      vector @a v. The largest element is determined by its absolute 
      magnitude for real vector and by the sum of the magnitudes of
      the real and imaginary parts @f$ |\Re(v_i)| + |\Im(v_i)| @f$ for
      complex vectors.  If the largest value occurs several times then
      the index of the first occurrence is returned.  
      @ingroup blas_lvl1
      @param v 
      @return Index @f$ i@f$ where @f$ v_i@f$ is the largest element
      in the vector */
  template <typename Type>
  typename vector<Type>::iterator 
  max(const vector<Type>& v);
  
  //_________________________________________________________________
  /** exchange the elements of the vectors @a v and @a u
      @param v First vector 
      @param u Second vector */
  template <typename Type>
  void 
  swap(vector<Type>& v, vector<Type>& u);
  

  //_________________________________________________________________
  /** Copies the vector @a v into the vector @a u
      @ingroup blas_lvl1
      @param u vector to copy to
      @param v vector to copy from */
  template <typename Type>
  void
  copy(const vector<Type>& v, vector<Type>& u);


  //_________________________________________________________________
  /** Compute the sum @f$ u  = \alpha v + u@f$ for the vectors @f$ u@f$ 
      and @f$ v@f$
      @ingroup blas_lvl1
      @param a scale parameter @f$ \alpha@f$
      @param v First operand @f$ v@f$.
      @param u Second operand @f$ u@f$.  The vector is overwritten by
      @f$ \alpha v + u@f$ */
  template <typename Type>
  void 
  scaled_add(const Type& a, const vector<Type>& v, vector<Type>& u);


  //_________________________________________________________________
  /** These functions rescale the vector @f$ v@f$ by the multiplicative
      factor @f$ \alpha@f$
      @ingroup blas_lvl1
      @param a Scale @f$ \alpha@f$
      @param v The vector @f$ v@f$.  It is overwritten by 
      @f$ \alpha v @f$ on return.  */
  template <typename Type, typename Type1>
  void 
  scale(const Type& a, vector<Type1>& v);
  
  //_________________________________________________________________
  /** @struct givens_rotation blas/blas.hh <gslmm/blas/blas.hh>
      @ingroup blas_lvl1
      Compute and apply a Givens rotation: 
      @f[
         \left(\begin{array}{cc}
	     c  & s \\
	     -s & c
         \end{array}\right)
         \left(\begin{array}{c}
	     a \\ 
	     b
         \end{array}\right)
	 =
         \left(\begin{array}{c}
	     r \\ 
	     0
         \end{array}\right)
       @f]

       The function operator applies this rotation to the vector @f$
       u,v@f$ so that 
      @f[
        v_i = c v_i + s u_i \quad u_i = -s v_i + c u_i
      @f]
   */
  template <typename Type>
  struct givens_rotation : public matrix<Type>
  {};

  //_________________________________________________________________
  /** @struct modified_givens_rotation blas/blas.hh <gslmm/blas/blas.hh>
      @ingroup blas_lvl1
      Compute and apply a modified Givens rotation.  Given @f$ a, b@f$
      in factored form 
      @f[ 
        a = \sqrt{d_1} x_1 \quad b = \sqrt{d_2} y_1\quad,
      @f]
      this structure construct the modified Givens plane rotation @f$
      d_1', d_2' @f$ and the matrix @f$ H@f$ such that 
      @f[
         \left(\begin{array}{cc}
	     \sqrt{d_1'}  & 0 \\
	     0            & \sqrt{d_2'}
         \end{array}\right) H 
         \left(\begin{array}{c}
	     x_1 \\ 
	     y_1
         \end{array}\right)
	 = 
	 G 
         \left(\begin{array}{c}
	     a \\ 
	     b
         \end{array}\right)
         \left(\begin{array}{c}
	     r \\ 
	     0
         \end{array}\right)
      @f]
      where @f$ G @f$ is a 2 by 2 Givens plane rotation matrix which
      annihilates @f$ b@f$, and where @f$ H@f$ us chosen for numerical
      stability and computational efficiency. 

      The function operator applies the modified Givens rotation to a
      pair of real vectors @f$ v, u@f$ each with @f$ n@f$ elements: 
      @f[
         \left(\begin{array}{c}
	     x_i \\ 
	     y_i
         \end{array}\right)
	 =
	 H
         \left(\begin{array}{c}
	     x_i \\ 
	     y_i
         \end{array}\right)
      @f]
      These vectors may be either rows or columns of matrices and the
      indexing of the vectors may be either forward or backwards. */
  template <typename Type>
  struct modified_givens_rotation : public givens_rotation<Type>
  {};
  
  //=================================================================
  // BLAS Level 2 
  /** @defgroup blas_lvl2 Level 2.  
      @ingroup blas 
      Rank 1 and 2 updates of matrices 
  */
  /** compute the rank-1 update  @f$ A = \alpha v u^T + A @f$ or, if
      @a c is @c true and @a A is complex, @f$ A = \alpha v u^H + A
      @f$ of the matrix @f$ A@f$.
      @ingroup blas_lvl2
      @param alpha The scalar @f$ \alpha@f$
      @param v     The first vector @f$ v@f$
      @param u     The second vector @f$ u@f$
      @param a     The matrix @f$ A@f$ to update
      @param c     Whether to conjugate the second vector
      @internal This is `ger', `gerc', `geru' */
  template <typename Type>
  void
  rank_1_update(const Type& alpha, 
		const vector<Type>& v, 
		const vector<Type>& u,
		matrix<Type>&       a, 
		bool                c=false);
  /** compute the rank-1 update  @f$ A = \alpha v u^T + A @f$ or, if
      @a c is @c true and @f$ A@f$ is complex, 
      @f$ A = \alpha v u^H + A @f$ of the matrix @f$ A@f$.
      @ingroup blas_lvl2
      @param alpha The scalar @f$ \alpha@f$
      @param v     The first vector @f$ v@f$
      @param u     The second vector @f$ u@f$
      @param a     The matrix @f$ A@f$ to update
      @param c     Whether to conjugate the second vector
      @internal This is `ger', `gerc', `geru' */
  template <typename Type> 
  void
  rank_1_update(const Type& alpha, 
		const vector<Type>& v, 
		const vector<Type>& u,
		matrix_manip<Type>  a, 
		bool                c=false);

  //__________________________________________________________________
  /** compute the symmetric rank-2 update  @f$ A = \alpha v u^T +
      \alpha u v^T @f$ or, if @f$ A@f$ is complex the hermitian rank-2
      update @f$ A = \alpha v u^H + \alpha u v^H @f$, of the matrix
      @f$ A@f$. 
      @ingroup blas_lvl2
      @param alpha The scalar @f$ \alpha@f$
      @param v     The first vector @f$ v@f$
      @param u     The second vector @f$ u@f$ 
      @param a     The matrix @f$ A@f$ to update
      @internal This is `syr2' or `her2' */
  template <typename Type>
  void
  rank_2_update(const Type&         alpha, 
		const vector<Type>& v, 
		const vector<Type>& u, 
		matrix_manip<Type>  a);

  //=================================================================
  // BLAS Level 3 
  /** @defgroup blas_lvl3 Level 3.  
      @ingroup blas 
      Higher order updats 
  */
  /** Compute the rank k update of the matrix 
      @f$ A = \alpha B B^T + \beta B@f$ 
      or  @f$ A = \alpha B B^H + \beta B@f$ if @f$ A@f$ is complex.
      Note, that the input matrix @a A must be symmetric or hermitian.  
      @ingroup blas_lvl3
      @param alpha The constant @f$ \alpha@f$
      @param a The matrix @f$ A@f$ On return, this is overwritten by
      the update
      @param b The matrix @f$ B@f$
      @param beta The constant @f$ \beta@f$ */
  template <typename Type> 
  void 
  rank_k_update(const Type&         alpha, 
		matrix<Type>&       a,
		const Type&         beta,
		const matrix<Type>& b);
  /** Compute the rank k update of the matrix 
      @f$ A = \alpha B B^T + \beta B@f$ 
      or  @f$ A = \alpha B B^H + \beta B@f$ if @f$ A@f$ is complex.
      Note, that the input matrix @f$ A@f$ must be symmetric or
      hermitian.  
      @ingroup blas_lvl3
      @param alpha The constant @f$ \alpha@f$
      @param a The matrix @f$ A@f$ On return, this is overwritten by
      the update
      @param b The matrix @f$ B@f$
      @param beta The constant @f$ \beta@f$ */
  template <typename Type> 
  void 
  rank_k_update(const Type&              alpha, 
		matrix_manip<Type>       a,
		const Type&              beta,
		const matrix_manip<Type> b); 
  /** Compute the rank 2k update of the matrix @f$ C = \alpha (B A^T + A
      B^T) + \beta C@f$ or  @f$ C = \alpha (A B^H + B A^H) + b C@f$ if
      @f$ A@f$ is complex.   Note, that the input matrix @f$ A@f$ must
      be symmetric or hermitian.  
      @ingroup blas_lvl3
      @param alpha The constant @f$ \alpha@f$
      @param a The matrix @f$ A@f$ 
      @param b The matrix @f$ B@f$
      @param beta The constant @f$ \beta@f$
      @param c The matrix @f$ C@f$. On return, this is overwritten by
      the update */
  template <typename Type> 
  void 
  rank_2k_update(const Type&         alpha, 
		 const matrix<Type>& a,
		 const matrix<Type>& b,
		 const Type&         beta,
		 matrix<Type>&       c);
  
  /** Compute the rank 2k update of the matrix @f$ C = \alpha (B A^T + A
      B^T) + \beta C@f$ or  @f$ C = \alpha (A B^H + B A^H) + b C@f$ if
      @a is complex.   Note, that the input matrix @a A must be
      symmetric or hermitian.  
      @ingroup blas_lvl3
      @param alpha The constant @f$ \alpha@f$
      @param a The matrix @f$ A@f$ 
      @param b The matrix @f$ B@f$
      @param beta The constant @f$ \beta@f$
      @param c The matrix @f$ C@f$. On return, this is overwritten by
      the update */
  template <typename Type> 
  void 
  rank_2k_update(const Type&               alpha, 
		 const matrix_manip<Type>  a,
		 const matrix_manip<Type>  b,
		 const Type&               beta,
		 matrix_manip<Type>        c);
}

#endif
//____________________________________________________________________
//
// EOF
//
