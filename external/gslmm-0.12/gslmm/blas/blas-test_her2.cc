#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

    
//____________________________________________________________________
template <typename T> 
void
test_her2(int order, int uplo,
	  size_t n, 
	  const gslmm::complex<T>& alpha, 
	  T* a, T* x, T* y, T* r, 
	  gslmm::test_suite& ts, 
	  const T& tol, const char* msg) 
{
  using namespace gslmm;

  // size_t ma = (CblasNoTrans == trans) ? n : m;
  // size_t na = (CblasNoTrans == trans) ? m : n;
  // if (CblasNoTrans == trans) trans = CblasConjTrans;
  // else trans = CblasNoTrans;
			       
  if (order == 102) {
    // Input is in column order mode - restructure 
    make_complex_row_major(n, n, a);
  }
  matrix<complex<T> > A(a, n, n);
  vector<complex<T> > X(x, n);
  vector<complex<T> > Y(y, n);
  matrix<complex<T> > R(r, n,  n);

  rank_2_update(alpha, X, Y, 
		matrix_manip<complex<T> >(A, 
					  hermitian_type,
					  no_transform,
					  location(uplo)));
  test_complex_result(A, R, ts, tol, msg);
}

//____________________________________________________________________
void
do_test_her2 (gslmm::test_suite& ts) 
{
  const double /* flteps = 1e-4, */ dbleps = 1e-6;
#if 0
  {
    gslmm::complex<float> alpha(-1.0f, 0.0f);
    float A[] = { -0.821f, 0.954f };
    float X[] = { 0.532f, 0.802f };
    float Y[] = { 0.016f, -0.334f };
    float R[] = { -0.302288f, 0.0f };
    test_her2<float>(101,121,1,alpha,A,X,Y,R,ts,flteps,"cher2(case 1450)");
  };


  {
    gslmm::complex<float> alpha(-1.0f, 0.0f);
    float A[] = { -0.821f, 0.954f };
    float X[] = { 0.532f, 0.802f };
    float Y[] = { 0.016f, -0.334f };
    float R[] = { -0.302288f, 0.0f };
    test_her2<float>(101,122,1,alpha,A,X,Y,R,ts,flteps,"cher2(case 1451)");
  };


  {
    gslmm::complex<float> alpha(-1.0f, 0.0f);
    float A[] = { -0.821f, 0.954f };
    float X[] = { 0.532f, 0.802f };
    float Y[] = { 0.016f, -0.334f };
    float R[] = { -0.302288f, 0.0f };
    test_her2<float>(102,121,1,alpha,A,X,Y,R,ts,flteps,"cher2(case 1452)");
  };


  {
    gslmm::complex<float> alpha(-1.0f, 0.0f);
    float A[] = { -0.821f, 0.954f };
    float X[] = { 0.532f, 0.802f };
    float Y[] = { 0.016f, -0.334f };
    float R[] = { -0.302288f, 0.0f };
    test_her2<float>(102,122,1,alpha,A,X,Y,R,ts,flteps,"cher2(case 1453)");
  };
#endif

  {
    gslmm::complex<double> alpha(-0.3, 0.1);
    double A[] = { -0.334, 0.286 };
    double X[] = { -0.14, -0.135 };
    double Y[] = { 0.455, 0.358 };
    double R[] = { -0.264521, 0.0 };
    test_her2<double>(101,121,1,alpha,A,X,Y,R,ts,dbleps,"zher2(case 1454)");
  };


  {
    gslmm::complex<double> alpha(-0.3, 0.1);
    double A[] = { -0.334, 0.286 };
    double X[] = { -0.14, -0.135 };
    double Y[] = { 0.455, 0.358 };
    double R[] = { -0.264521, 0.0 };
    test_her2<double>(101,122,1,alpha,A,X,Y,R,ts,dbleps,"zher2(case 1455)");
  };


  {
    gslmm::complex<double> alpha(-0.3, 0.1);
    double A[] = { -0.334, 0.286 };
    double X[] = { -0.14, -0.135 };
    double Y[] = { 0.455, 0.358 };
    double R[] = { -0.264521, 0.0 };
    test_her2<double>(102,121,1,alpha,A,X,Y,R,ts,dbleps,"zher2(case 1456)");
  };


  {
    gslmm::complex<double> alpha(-0.3, 0.1);
    double A[] = { -0.334, 0.286 };
    double X[] = { -0.14, -0.135 };
    double Y[] = { 0.455, 0.358 };
    double R[] = { -0.264521, 0.0 };
    test_her2<double>(102,122,1,alpha,A,X,Y,R,ts,dbleps,"zher2(case 1457)");
  };


}
/*
   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int uplo\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);
   int lda.*
   \\(float\\|double\\) alpha\\[2\\]\\s-*=\\s-*{\\(.*\\)};
   \\4 A\\[\\]\\s-*=\\s-*\\(.*\\);
   \\4 X\\[\\]\\s-*=\\s-*\\(.*\\);
   int incX.*
   \\4 Y\\[\\]\\s-*=\\s-*\\(.*\\);
   int incY.*
   \\4 A_expected\\[\\]\\s-*=\\s-*\\(.*\\);"
  " gslmm::complex<\\4> alpha(\\5);
    \\4 A[] = \\6;
    \\4 X[] = \\7;
    \\4 Y[] = \\8;
    \\4 R[] = \\9;
    test_her<\\4>(\\1,\\2,\\3,alpha,A,X,Y,R);")

   (query-replace-regexp 
   "test_her<\\(float\\|double\\)>(\\(.*\\));
   cblas_.her2(.*);
   {
     int i;
     for.*{
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps, \"\\(.*\\) real\");
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\3eps, \"\\4 imag\");
     };?
   };" "test_her2<\\1>(\\2,ts,\\3eps,\"\\4\");")
*/
