#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

//____________________________________________________________________
template <typename T> 
void
test_gemv(int order, int trans,
	  size_t m, size_t n,
	  const T& alpha, const T& beta, 
	  T* a, T* x, T* y, T* r, 
	  gslmm::test_suite& ts, const T& tol, const char* msg) 
{
  using namespace gslmm;
  
  size_t ma = (trans == CblasNoTrans) ? m : n;
  size_t na = (trans == CblasNoTrans) ? n : m;

  if (order == 102) {
    // Input is in column order mode - restructure 
    make_row_major(ma, na, a);
  }
  
  matrix<T> A(a, ma, na);
  vector<T> X(x, ma);
  vector<T> Y(y, ma);
  vector<T> R(r, ma);

  matrix_vector_product(alpha, 
			matrix_manip<T>(A, general_type, transform(trans)), 
			X, beta, Y);
  test_result(Y, R, ts, tol, msg);
}

//____________________________________________________________________
template <typename T> 
void
test_complex_gemv(int order, int trans,
		  size_t m, size_t n,
		  const gslmm::complex<T>& alpha, 
		  const gslmm::complex<T>& beta, T* a, T* x, T* y, T* r, 
		  gslmm::test_suite& ts, const T& tol, const char* msg) 
{
  using namespace gslmm;
  
  size_t ma = (trans == CblasNoTrans) ? m : n;
  size_t na = (trans == CblasNoTrans) ? n : m;

  if (order == 102) {
    // Input is in column order mode - restructure 
    make_complex_row_major(ma, na, a);
  }
  
  matrix<complex<T> > A(a, ma, na);
  vector<complex<T> > X(x, ma);
  vector<complex<T> > Y(y, ma);
  vector<complex<T> > R(r, ma);

  matrix_vector_product(alpha, 
			matrix_manip<complex<T> >(A, general_type, 
						  transform(trans)), 
			X, beta, Y);
  test_complex_result(Y, R, ts, tol, msg);
}

//____________________________________________________________________
void
do_test_gemv (gslmm::test_suite& ts) 
{
  const double flteps = 1e-4, dbleps = 1e-6;
  {
   float A[] = { -0.805f };
   float X[] = { -0.965f };
   float Y[] = { 0.537f };
   float R[] = { 0.615725f };
test_gemv<float>(101,111,1,1,1.0f,-0.3f,A,X,Y,R,ts,flteps,"sgemv(case 774)");
  };


  {
   float A[] = { -0.805f };
   float X[] = { -0.965f };
   float Y[] = { 0.537f };
   float R[] = { 0.615725f };
    test_gemv<float>(102,111,1,1,1.0f,-0.3f,A,X,Y,R,ts,flteps,"sgemv(case 775)");
  };


  {
   float A[] = { -0.805f };
   float X[] = { -0.965f };
   float Y[] = { 0.537f };
   float R[] = { 0.776825f };
    test_gemv<float>(101,112,1,1,1.0f,0.0f,A,X,Y,R,ts,flteps,"sgemv(case 776)");
  };


  {
   float A[] = { -0.805f };
   float X[] = { -0.965f };
   float Y[] = { 0.537f };
   float R[] = { 0.776825f };
    test_gemv<float>(102,112,1,1,1.0f,0.0f,A,X,Y,R,ts,flteps,"sgemv(case 777)");
  };


  {
   double A[] = { -0.047 };
   double X[] = { 0.672 };
   double Y[] = { 0.554 };
   double R[] = { -0.5445248 };
    test_gemv<double>(101,111,1,1,-0.3,-1,A,X,Y,R,ts,dbleps,"dgemv(case 778)");
  };


  {
   double A[] = { -0.047 };
   double X[] = { 0.672 };
   double Y[] = { 0.554 };
   double R[] = { -0.5445248 };
    test_gemv<double>(102,111,1,1,-0.3,-1,A,X,Y,R,ts,dbleps,"dgemv(case 779)");
  };


  {
   double A[] = { -0.047 };
   double X[] = { 0.672 };
   double Y[] = { 0.554 };
   double R[] = { 0.585584 };
    test_gemv<double>(101,112,1,1,-1,1,A,X,Y,R,ts,dbleps,"dgemv(case 780)");
  };


  {
   double A[] = { -0.047 };
   double X[] = { 0.672 };
   double Y[] = { 0.554 };
   double R[] = { 0.585584 };
    test_gemv<double>(102,112,1,1,-1,1,A,X,Y,R,ts,dbleps,"dgemv(case 781)");
  };

#if 0
  {
   gslmm::complex<float> alpha(0.0f, 0.1f);
   gslmm::complex<float> beta(0.0f, 1.0f);
   float A[] = { 0.629f, 0.801f };
   float X[] = { 0.778f, -0.073f };
   float Y[] = { -0.976f, -0.682f };
   float R[] = { 0.624274f, -0.921216f };
    test_complex_gemv<float>(101,111,1,1,alpha,beta,A,X,Y,R,ts,flteps,"cgemv(case 782)");
  };


  {
   gslmm::complex<float> alpha(0.0f, 0.1f);
   gslmm::complex<float> beta(0.0f, 1.0f);
   float A[] = { 0.629f, 0.801f };
   float X[] = { 0.778f, -0.073f };
   float Y[] = { -0.976f, -0.682f };
   float R[] = { 0.624274f, -0.921216f };
    test_complex_gemv<float>(102,111,1,1,alpha,beta,A,X,Y,R,ts,flteps,"cgemv(case 783)");
  };


  {
   gslmm::complex<float> alpha(0.0f, 1.0f);
   gslmm::complex<float> beta(-0.3f, 0.1f);
   float A[] = { 0.629f, 0.801f };
   float X[] = { 0.778f, -0.073f };
   float Y[] = { -0.976f, -0.682f };
   float R[] = { -0.216261f, 0.654835f };
    test_complex_gemv<float>(101,112,1,1,alpha,beta,A,X,Y,R,ts,flteps,"cgemv(case 784)");
  };


  {
   gslmm::complex<float> alpha(0.0f, 1.0f);
   gslmm::complex<float> beta(-0.3f, 0.1f);
   float A[] = { 0.629f, 0.801f };
   float X[] = { 0.778f, -0.073f };
   float Y[] = { -0.976f, -0.682f };
   float R[] = { -0.216261f, 0.654835f };
    test_complex_gemv<float>(102,112,1,1,alpha,beta,A,X,Y,R,ts,flteps,"cgemv(case 785)");
  };


  {
   gslmm::complex<float> alpha(0.0f, 0.1f);
   gslmm::complex<float> beta(-0.3f, 0.1f);
   float A[] = { 0.629f, 0.801f };
   float X[] = { 0.778f, -0.073f };
   float Y[] = { -0.976f, -0.682f };
   float R[] = { 0.427909f, 0.150089f };
    test_complex_gemv<float>(101,113,1,1,alpha,beta,A,X,Y,R,ts,flteps,"cgemv(case 786)");
  };


  {
   gslmm::complex<float> alpha(0.0f, 0.1f);
   gslmm::complex<float> beta(-0.3f, 0.1f);
   float A[] = { 0.629f, 0.801f };
   float X[] = { 0.778f, -0.073f };
   float Y[] = { -0.976f, -0.682f };
   float R[] = { 0.427909f, 0.150089f };
    test_complex_gemv<float>(102,113,1,1,alpha,beta,A,X,Y,R,ts,flteps,"cgemv(case 787)");
  };
#endif

  {
   gslmm::complex<double> alpha(0, 0.1);
   gslmm::complex<double> beta(1, 0);
   double A[] = { 0.932, -0.724 };
   double X[] = { 0.334, -0.317 };
   double Y[] = { 0.348, 0.07 };
   double R[] = { 0.401726, 0.078178 };
    test_complex_gemv<double>(101,111,1,1,alpha,beta,A,X,Y,R,ts,dbleps,"zgemv(case 788)");
  };


  {
   gslmm::complex<double> alpha(0, 0.1);
   gslmm::complex<double> beta(1, 0);
   double A[] = { 0.932, -0.724 };
   double X[] = { 0.334, -0.317 };
   double Y[] = { 0.348, 0.07 };
   double R[] = { 0.401726, 0.078178 };
    test_complex_gemv<double>(102,111,1,1,alpha,beta,A,X,Y,R,ts,dbleps,"zgemv(case 789)");
  };


  {
   gslmm::complex<double> alpha(-0.3, 0.1);
   gslmm::complex<double> beta(0., 1.);
   double A[] = { 0.932, -0.724 };
   double X[] = { 0.334, -0.317 };
   double Y[] = { 0.348, 0.07 };
   double R[] = { -0.040808, 0.517356 };
    test_complex_gemv<double>(101,112,1,1,alpha,beta,A,X,Y,R,ts,dbleps,"zgemv(case 790)");
  };


  {
   gslmm::complex<double> alpha(-0.3, 0.1);
   gslmm::complex<double> beta(0., 1.);
   double A[] = { 0.932, -0.724 };
   double X[] = { 0.334, -0.317 };
   double Y[] = { 0.348, 0.07 };
   double R[] = { -0.040808, 0.517356 };
    test_complex_gemv<double>(102,112,1,1,alpha,beta,A,X,Y,R,ts,dbleps,"zgemv(case 791)");
  };


  {
   gslmm::complex<double> alpha(1, 0);
   gslmm::complex<double> beta(0., 0.);
   double A[] = { 0.932, -0.724 };
   double X[] = { 0.334, -0.317 };
   double Y[] = { 0.348, 0.07 };
   double R[] = { 0.540796, -0.053628 };
    test_complex_gemv<double>(101,113,1,1,alpha,beta,A,X,Y,R,ts,dbleps,"zgemv(case 792)");
  };


  {
   gslmm::complex<double> alpha(1., 0.);
   gslmm::complex<double> beta(0., 0.);
   double A[] = { 0.932, -0.724 };
   double X[] = { 0.334, -0.317 };
   double Y[] = { 0.348, 0.07 };
   double R[] = { 0.540796, -0.053628 };
    test_complex_gemv<double>(102,113,1,1,alpha,beta,A,X,Y,R,ts,dbleps,"zgemv(case 793)");
  };


}
/*
   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int trans\\s-*=\\s-*\\(.*\\);
   int M\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);
   int lda\\s-*.*;"
  "test_gemv(\\1,\\2,\\3,\\4);")

   (query-replace-regexp 
  "test_gemv(\\(.*\\));
   \\(float\\|double\\) alpha\\s-*=\\s-*\\(.*\\);
   \\2 beta\\s-*=\\s-*\\(.*\\);
   \\2 A\\[\\]\\s-*=\\s-*\\(.*\\);
   \\2 X\\[\\]\\s-*=\\s-*\\(.*\\);
   int incX.*
   \\2 Y\\[\\]\\s-*=\\s-*\\(.*\\);
   int incY.*
   \\2 y_expected\\[\\]\\s-*=\\s-*\\(.*\\);"
   "\\2 A[] = \\5;
   \\2 X[] = \\6;
   \\2 Y[] = \\7;
   \\2 R[] = \\8;
    test_gemv<\\2>(\\1,\\3,\\4,A,X,Y,R);")

   (query-replace-regexp 
  "test_gemv(\\(.*\\));
   \\(float\\|double\\) alpha\\[2\\]\\s-*=\\s-*{\\(.*\\)};
   \\2 beta\\[2\\]\\s-*=\\s-*{\\(.*\\)};
   \\2 A\\[\\]\\s-*=\\s-*\\(.*\\);
   \\2 X\\[\\]\\s-*=\\s-*\\(.*\\);
   int incX.*
   \\2 Y\\[\\]\\s-*=\\s-*\\(.*\\);
   int incY.*
   \\2 y_expected\\[\\]\\s-*=\\s-*\\(.*\\);"
   "gslmm::complex<\\2> alpha(\\3);
   gslmm::complex<\\2> beta(\\4);
   \\2 A[] = \\5;
   \\2 X[] = \\6;
   \\2 Y[] = \\7;
   \\2 R[] = \\8;
    test_gemv<\\2>(\\1,alpha,beta,A,X,Y,R);")

   (query-replace-regexp 
   "test_\\(complex_\\|\\)gemv<\\(float\\|double\\)>(\\(.*\\));
\\s-*cblas_.gemv.*
\\s-*{
\\s-*int i;
\\s-*for.*{
\\s-*gsl_test_rel(Y\\[.*\\],\\s-*y_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps,\\s-*\"\\(.*)\\).*\");[^}]*
\\s-*};?
\\s-*};"
    "test_\\1gemv<\\2>(\\3,ts,\\4eps,\"\\5\");")

*/
