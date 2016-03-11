#ifndef GSLMM_blas_test
#define GSLMM_blas_test
#ifndef GSLMM_matrix
# include <gslmm/vectormatrix/matrix.hh>
#endif
#ifndef GSLMM_blas_blas
# include <gslmm/blas/blas.hh>
#endif
// #ifndef GSLMM_matrix_complex_float
// # include <gslmm/vectormatrix/matrix_complex_float.hh>
// #endif
#ifndef GSLMM_matrix_comple_double
# include <gslmm/vectormatrix/matrix_complex_double.hh>
#endif
#ifndef GSLMM_test_suite
# define NO_TESTSUITE_INSTANCE
# include <gslmm/test/test_suite.hh>
#endif

//____________________________________________________________________
template <typename T> 
void
make_row_major(size_t m, size_t n, T* a) 
{
  std::vector<T> tmp(m * n);
  for (size_t row = 0; row < m; row++) 
    for (size_t col = 0; col < n; col++) 
      tmp[row * n + col] = a[col * m + row];
  for (size_t i = 0; i < m * n; i++) 
    a[i] = tmp[i];
}

//____________________________________________________________________
template <typename T> 
void
make_complex_row_major(size_t m, size_t n, T* a) 
{
  std::vector<T> tmp(2 * m * n);
  for (size_t row = 0; row < m; row++) {
    for (size_t col = 0; col < n; col++) {
      tmp[2 * (row * n + col)]     = a[2 * (col * m + row)];
      tmp[2 * (row * n + col) + 1] = a[2 * (col * m + row) + 1];
    }
  }
  for (size_t i = 0; i < 2 * m * n; i++) 
    a[i] = tmp[i];
}

//____________________________________________________________________
template <typename T> 
void
test_result(gslmm::matrix<T>& o, gslmm::matrix<T>& r, 
	    gslmm::test_suite& ts, const T& tol, const char* msg) 
{
  std::string mmsg(msg);
  mmsg += " (%d,%d)";
  for (size_t row=0; row < o.row_size(); row++) 
    for (size_t column=0; column < o.column_size(); column++)
      ts.relative(o(row,column), r(row,column), tol, 
		  mmsg.c_str(), row, column);
}

//____________________________________________________________________
template <typename T> 
void
test_complex_result(gslmm::matrix<gslmm::complex<T> >& o, 
		    gslmm::matrix<gslmm::complex<T> >& r, 
		    gslmm::test_suite& ts, const T& tol, const char* msg) 
{
  std::string rmsg(msg);
  std::string imsg(msg);
  rmsg += " real(%d,%d)";
  imsg += " imag(%d,%d)";
  for (size_t row=0; row < o.row_size(); row++) {
    for (size_t column=0; column < o.column_size(); column++) {
      ts.relative(o(row,column).real(), r(row,column).real(), tol, 
		  rmsg.c_str(), row, column);
      ts.relative(o(row,column).imag(), r(row,column).imag(), tol, 
		  imsg.c_str(), row, column);
    }
  }
}

//____________________________________________________________________
template <typename T> 
void
test_result(gslmm::vector<T>& o, gslmm::vector<T>& r, 
	    gslmm::test_suite& ts, const T& tol, const char* msg) 
{
  std::string mmsg(msg);
  mmsg += " (%d)";
  for (size_t row=0; row < o.size(); row++) 
    ts.relative(o[row], r[row], tol, mmsg.c_str(), row);
}

//____________________________________________________________________
template <typename T> 
void
test_complex_result(gslmm::vector<gslmm::complex<T> >& o, 
		    gslmm::vector<gslmm::complex<T> >& r, 
		    gslmm::test_suite& ts, const T& tol, const char* msg) 
{
  std::string rmsg(msg);
  std::string imsg(msg);
  rmsg += " real(%d)";
  imsg += " imag(%d)";
  for (size_t row=0; row < o.size(); row++) {
    ts.relative(o[row].real(), r[row].real(), tol, rmsg.c_str(), row);
    ts.relative(o[row].imag(), r[row].imag(), tol, imsg.c_str(), row);
  }
}

#endif
//
// EOF
//
