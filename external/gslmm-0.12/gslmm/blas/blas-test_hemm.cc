#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

//____________________________________________________________________
template <typename T> 
void
test_complex_hemm(int order, int uplo, int sid, 
		  size_t m, size_t n, 
		  const gslmm::complex<T>& alpha, 
		  const gslmm::complex<T>& beta, 
		  T* a, T* b, T* c, T* r, 
		  gslmm::test_suite& ts, 
		  const T& tol, const char* msg) 
{
  using namespace gslmm;
  
  size_t ma = (sid == left_side ? m : n);
  if (order == 102) {
    // Input is in column order mode - restructure 
    make_complex_row_major(ma, ma, a);
    make_complex_row_major(m, n, b);
    make_complex_row_major(m, n, r);
    make_complex_row_major(m, n, c);
  }
  
  matrix<complex<T> > A(a, ma, ma);
  matrix<complex<T> > B(b, m, n);
  matrix<complex<T> > C(c, m, n);
  matrix<complex<T> > R(r, m, n);

  matrix_matrix_product(alpha, 
			matrix_manip<complex<T> >(A, 
						  hermitian_type,
						  no_transform, 
						  location(uplo), 
						  non_unit_diagonal, 
						  side(sid)),
			matrix_manip<complex<T> >(B),
			beta, C);
  test_complex_result(C, R, ts, tol, msg);
}


//____________________________________________________________________
void
do_test_hemm (gslmm::test_suite& ts) 
{
  const double /* flteps = 1e-4, */ dbleps = 1e-6;
#if 0
  {
   gslmm::complex<float> alpha(0.0f, 0.1f);
   gslmm::complex<float> beta(0.0f, 0.1f);
    float A[]	= { -0.126f, 0.079f };
    float B[]	= { -0.954f, -0.059f, 0.296f, -0.988f };
    float C[]	= { -0.859f, -0.731f, 0.737f, 0.593f };
    float R[]	= { 0.0723566f, -0.0738796f, -0.0717488f, 0.0699704f };
    test_complex_hemm<float>(101,121,141,1,2,alpha,beta,A,B,C,R,ts,flteps,"chemm(case 1550)");
  };


  {
   gslmm::complex<float> alpha(0.0f, 0.1f);
   gslmm::complex<float> beta(0.0f, 0.1f);
    float A[]	= { 0.652f, 0.584f };
    float B[]	= { -0.983f, -0.734f, -0.422f, -0.825f };
    float C[]	= { 0.387f, 0.341f, -0.734f, 0.632f };
    float R[]	= { 0.0137568f, -0.0253916f, -0.00941f, -0.100914f };
    test_complex_hemm<float>(102,121,141,1,2,alpha,beta,A,B,C,R,ts,flteps,"chemm(case 1551)");
  };


  {
   gslmm::complex<float> alpha(0.0f, 1.0f);
   gslmm::complex<float> beta(-1.0f, 0.0f);
    float A[]	= { 0.78f, 0.885f, 0.507f, 0.765f, 0.911f, -0.461f, 0.707f, 0.508f };
    float B[]	= { -0.905f, 0.633f, 0.85f, -0.943f };
    float C[]	= { 0.045f, -0.237f, 0.078f, -0.252f };
    float R[]	= { 0.589611f, -0.759345f, 0.960095f, -0.09013f };
    test_complex_hemm<float>(101,121,142,1,2,alpha,beta,A,B,C,R,ts,flteps,"chemm(case 1552)");
  };


  {
   gslmm::complex<float> alpha(0.0f, 1.0f);
   gslmm::complex<float> beta(-1.0f, 0.0f);
    float A[]	= { 0.947f, 0.939f, -0.267f, -0.819f, -0.827f, -0.937f, 0.991f, 0.838f };
    float B[]	= { 0.871f, -0.988f, -0.232f, -0.434f };
    float C[]	= { -0.261f, 0.927f, -0.351f, -0.203f };
    float R[]	= { 1.0551f, 0.496359f, 0.780145f, -1.67298f };
    test_complex_hemm<float>(102,121,142,1,2,alpha,beta,A,B,C,R,ts,flteps,"chemm(case 1553)");
  };


  {
   gslmm::complex<float> alpha(-1.0f, 0.0f);
   gslmm::complex<float> beta(0.0f, 0.0f);
    float A[]	= { -0.593f, -0.9f };
    float B[]	= { -0.861f, 0.747f, -0.984f, 0.595f };
    float C[]	= { -0.589f, -0.671f, -0.011f, -0.417f };
    float R[]	= { -0.510573f, 0.442971f, -0.583512f, 0.352835f };
    test_complex_hemm<float>(101,122,141,1,2,alpha,beta,A,B,C,R,ts,flteps,"chemm(case 1554)");
  };


  {
   gslmm::complex<float> alpha(-1.0f, 0.0f);
   gslmm::complex<float> beta(0.0f, 0.0f);
    float A[]	= { -0.79f, 0.132f };
    float B[]	= { -0.243f, -0.12f, 0.633f, -0.556f };
    float C[]	= { -0.658f, -0.74f, -0.47f, 0.481f };
    float R[]	= { -0.19197f, -0.0948f, 0.50007f, -0.43924f };
    test_complex_hemm<float>(102,122,141,1,2,alpha,beta,A,B,C,R,ts,flteps,"chemm(case 1555)");
  };


  {
   gslmm::complex<float> alpha(-0.3f, 0.1f);
   gslmm::complex<float> beta(0.0f, 1.0f);
    float A[]	= { -0.114f, -0.515f, -0.513f, -0.527f, -0.995f, 0.986f, 0.229f, -0.076f };
    float B[]	= { 0.084f, 0.522f, 0.61f, 0.694f };
    float C[]	= { 0.802f, 0.136f, -0.161f, -0.364f };
    float R[]	= { 0.269101f, 0.716492f, 0.237088f, 0.0290666f };
    test_complex_hemm<float>(101,122,142,1,2,alpha,beta,A,B,C,R,ts,flteps,"chemm(case 1556)");
  };


  {
   gslmm::complex<float> alpha(-0.3f, 0.1f);
   gslmm::complex<float> beta(0.0f, 1.0f);
    float A[]	= { 0.798f, -0.324f, -0.693f, -0.893f, -0.223f, 0.749f, 0.102f, -0.357f };
    float B[]	= { -0.572f, -0.569f, -0.391f, -0.938f };
    float C[]	= { 0.152f, -0.834f, -0.633f, -0.473f };
    float R[]	= { 1.08642f, -0.113853f, 0.234826f, -0.48289f };
    test_complex_hemm<float>(102,122,142,1,2,alpha,beta,A,B,C,R,ts,flteps,"chemm(case 1557)");
  };
#endif

  {
   gslmm::complex<double> alpha(0., 0.1);
   gslmm::complex<double> beta(0., 0.1);
    double A[]	= { -0.359, 0.089 };
    double B[]	= { -0.451, -0.337, -0.901, -0.871 };
    double C[]	= { 0.729, 0.631, 0.364, 0.246 };
    double R[]	= { -0.0751983, 0.0890909, -0.0558689, 0.0687459 };
    test_complex_hemm<double>(101,121,141,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zhemm(case 1558)");
  };


  {
   gslmm::complex<double> alpha(0., 0.1);
   gslmm::complex<double> beta(0., 0.1);
    double A[]	= { 0.044, -0.496 };
    double B[]	= { -0.674, 0.281, 0.366, 0.888 };
    double C[]	= { -0.9, 0.919, 0.857, -0.049 };
    double R[]	= { -0.0931364, -0.0929656, 0.0009928, 0.0873104 };
    test_complex_hemm<double>(102,121,141,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zhemm(case 1559)");
  };


  {
   gslmm::complex<double> alpha(0., 0.);
   gslmm::complex<double> beta(0., 0.1);
    double A[]	= { -0.314, 0.115, 0.114, 0.878, 0.961, -0.224, 0.973, 0.771 };
    double B[]	= { 0.5, -0.016, -0.5, 0.149 };
    double C[]	= { -0.054, 0.064, 0.02, 0.245 };
    double R[]	= { -0.0064, -0.0054, -0.0245, 0.002 };
    test_complex_hemm<double>(101,121,142,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zhemm(case 1560)");
  };


  {
   gslmm::complex<double> alpha(0., 0.);
   gslmm::complex<double> beta(0., 0.1);
    double A[]	= { 0.186, 0.578, 0.797, -0.957, -0.539, -0.969, -0.21, 0.354 };
    double B[]	= { 0.641, -0.968, 0.15, -0.569 };
    double C[]	= { -0.556, -0.9, 0.197, 0.31 };
    double R[]	= { 0.09, -0.0556, -0.031, 0.0197 };
    test_complex_hemm<double>(102,121,142,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zhemm(case 1561)");
  };


  {
   gslmm::complex<double> alpha(1., 0.);
   gslmm::complex<double> beta(1., 0.);
    double A[]	= { 0.323, 0.641 };
    double B[]	= { -0.188, 0.091, -0.235, 0.523 };
    double C[]	= { 0.919, 0.806, 0.823, -0.94 };
    double R[]	= { 0.858276, 0.835393, 0.747095, -0.771071 };
    test_complex_hemm<double>(101,122,141,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zhemm(case 1562)");
  };


  {
   gslmm::complex<double> alpha(1., 0.);
   gslmm::complex<double> beta(1., 0.);
    double A[]	= { -0.688, 0.915 };
    double B[]	= { 0.914, -0.204, 0.205, -0.476 };
    double C[]	= { 0.27, -0.628, -0.079, 0.507 };
    double R[]	= { -0.358832, -0.487648, -0.22004, 0.834488 };
    test_complex_hemm<double>(102,122,141,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zhemm(case 1563)");
  };


  {
   gslmm::complex<double> alpha(0., 1.);
   gslmm::complex<double> beta(0., 0.1);
    double A[]	= { 0.681, 0.574, -0.425, -0.64, 0.792, 0.661, -0.009, 0.005 };
    double B[]	= { -0.221, 0.554, -0.465, -0.95 };
    double C[]	= { 0.331, -0.958, -0.826, -0.972 };
    double R[]	= { 0.778291, 0.142269, -0.496199, 0.112747 };
    test_complex_hemm<double>(101,122,142,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zhemm(case 1564)");
  };


  {
   gslmm::complex<double> alpha(0., 1.);
   gslmm::complex<double> beta(0., 0.1);
    double A[]	= { 0.959, 0.34, -0.23, 0.064, 0.516, -0.275, 0.714, 0.899 };
    double B[]	= { -0.502, -0.987, -0.134, 0.215 };
    double C[]	= { 0.929, 0.181, -0.16, -0.921 };
    double R[]	= { 0.986459, -0.371458, -0.320548, -0.059384 };
    test_complex_hemm<double>(102,122,142,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zhemm(case 1565)");
  };


}
/*
   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int side\\s-*=\\s-*\\(.*\\);
   int uplo\\s-*=\\s-*\\(.*\\);
   int M\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);
   \\(float\\|double\\) alpha\\[2\\]\\s-*=\\s-*{\\(.*\\)};
   \\6 beta\\[2\\]\\s-*=\\s-*{\\(.*\\)};"
  "gslmm::complex<\\6> alpha(\\7);
   gslmm::complex<\\6> beta(\\8);
   test_hemm<\\6>(\\1,\\3,\\2,\\4,\\5,alpha,beta);")

   (search-forward-regexp 
   "test_hemm<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\1 A\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int lda.*;")

   (query-replace-regexp 
   "test_hemm<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\1 A\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int lda.*;
\\s-*\\1 B\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldb.*;
\\s-*\\1 C\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldc.*;
\\s-*\\1 C_expected\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*cblas_.hemm(.*);
\\s-*{
\\s-*int i;
\\s-*for.*{
\\s-*gsl_test_rel(C\\[2\\*i\\], C_expected\\[2\\*i\\], \\(flt\\|dbl\\)eps, \"\\(.*\\) real\");
\\s-*gsl_test_rel(C\\[2\\*i\\+1\\], C_expected\\[2\\*i\\+1\\], \\7eps, \"\\8 imag\");
\\s-*};?
\\s-*};"
    " \\1 A[]	= {\\3};
    \\1 B[]	= {\\4};
    \\1 C[]	= {\\5};
    \\1 R[]	= {\\6};
    test_complex_hemm<\\1>(\\2,A,B,C,R,ts,\\7eps,\"\\8\");")
    
*/
