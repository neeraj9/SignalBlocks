#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

//____________________________________________________________________
template <typename T> 
void
test_hemv(int order, int uplo,
	  size_t n,
	  const gslmm::complex<T>& alpha, const gslmm::complex<T>& beta, 
	  T* a, T* x, T* y, T* r, 
	  gslmm::test_suite& ts, const T& tol, const char* msg) 
{
  using namespace gslmm;
  
  size_t ma = n;
  size_t na = n;

  if (order == 102) {
    // Input is in column order mode - restructure 
    make_row_major(ma, na, a);
  }
  
  matrix<gslmm::complex<T> > A(a, ma, na);
  vector<gslmm::complex<T> > X(x, ma);
  vector<gslmm::complex<T> > Y(y, ma);
  vector<gslmm::complex<T> > R(r, ma);

  matrix_vector_product(alpha, 
			matrix_manip<gslmm::complex<T> >(A, hermitian_type, 
							 no_transform,
							 location(uplo)), 
			X, beta, Y);
  test_complex_result(Y, R, ts, tol, msg);
}


//____________________________________________________________________
void
do_test_hemv (gslmm::test_suite& ts) 
{
  const double /* flteps = 1e-4,*/ dbleps = 1e-6;
#if  0
  {
    gslmm::complex<float> alpha(1.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[] = { -0.434f, 0.837f };
    float X[] = { 0.209f, -0.935f };
    float Y[] = { 0.346f, -0.412f };
    float R[] = { -0.153306f, 0.56399f };
    test_hemv<float>(101,121,1,alpha,beta,A,X,Y,R,ts,flteps,"chemv(case 1070)");
  };


  {
    gslmm::complex<float> alpha(1.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[] = { -0.434f, 0.837f };
    float X[] = { 0.209f, -0.935f };
    float Y[] = { 0.346f, -0.412f };
    float R[] = { -0.153306f, 0.56399f };
    test_hemv<float>(101,121,1,alpha,beta,A,X,Y,R,ts,flteps,"chemv(case 1071)");
  };


  {
    gslmm::complex<float> alpha(1.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[] = { -0.434f, 0.837f };
    float X[] = { 0.209f, -0.935f };
    float Y[] = { 0.346f, -0.412f };
    float R[] = { -0.153306f, 0.56399f };
    test_hemv<float>(101,122,1,alpha,beta,A,X,Y,R,ts,flteps,"chemv(case 1072)");
  };


  {
    gslmm::complex<float> alpha(1.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[] = { -0.434f, 0.837f };
    float X[] = { 0.209f, -0.935f };
    float Y[] = { 0.346f, -0.412f };
    float R[] = { -0.153306f, 0.56399f };
    test_hemv<float>(101,122,1,alpha,beta,A,X,Y,R,ts,flteps,"chemv(case 1073)");
  };


  {
    gslmm::complex<float> alpha(1.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[] = { -0.434f, 0.837f };
    float X[] = { 0.209f, -0.935f };
    float Y[] = { 0.346f, -0.412f };
    float R[] = { -0.153306f, 0.56399f };
    test_hemv<float>(102,121,1,alpha,beta,A,X,Y,R,ts,flteps,"chemv(case 1074)");
  };


  {
    gslmm::complex<float> alpha(1.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[] = { -0.434f, 0.837f };
    float X[] = { 0.209f, -0.935f };
    float Y[] = { 0.346f, -0.412f };
    float R[] = { -0.153306f, 0.56399f };
    test_hemv<float>(102,121,1,alpha,beta,A,X,Y,R,ts,flteps,"chemv(case 1075)");
  };


  {
    gslmm::complex<float> alpha(1.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[] = { -0.434f, 0.837f };
    float X[] = { 0.209f, -0.935f };
    float Y[] = { 0.346f, -0.412f };
    float R[] = { -0.153306f, 0.56399f };
    test_hemv<float>(102,122,1,alpha,beta,A,X,Y,R,ts,flteps,"chemv(case 1076)");
  };


  {
    gslmm::complex<float> alpha(1.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[] = { -0.434f, 0.837f };
    float X[] = { 0.209f, -0.935f };
    float Y[] = { 0.346f, -0.412f };
    float R[] = { -0.153306f, 0.56399f };
    test_hemv<float>(102,122,1,alpha,beta,A,X,Y,R,ts,flteps,"chemv(case 1077)");
  };


  {
    gslmm::complex<double> alpha(0, 0);
    gslmm::complex<double> beta(1, 0);
    double A[] = { 0.036, -0.966 };
    double X[] = { -0.695, 0.886 };
    double Y[] = { 0.486, 0.629 };
    double R[] = { 0.486, 0.629 };
    test_hemv<double>(101,121,1,alpha,beta,A,X,Y,R,ts,dbleps,"zhemv(case 1078)");
  };
#endif

  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(1., 0.);
    double A[] = { 0.036, -0.966 };
    double X[] = { -0.695, 0.886 };
    double Y[] = { 0.486, 0.629 };
    double R[] = { 0.486, 0.629 };
    test_hemv<double>(101,121,1,alpha,beta,A,X,Y,R,ts,dbleps,"zhemv(case 1079)");
  };


  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(1., 0.);
    double A[] = { 0.036, -0.966 };
    double X[] = { -0.695, 0.886 };
    double Y[] = { 0.486, 0.629 };
    double R[] = { 0.486, 0.629 };
    test_hemv<double>(101,122,1,alpha,beta,A,X,Y,R,ts,dbleps,"zhemv(case 1080)");
  };


  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(1., 0.);
    double A[] = { 0.036, -0.966 };
    double X[] = { -0.695, 0.886 };
    double Y[] = { 0.486, 0.629 };
    double R[] = { 0.486, 0.629 };
    test_hemv<double>(101,122,1,alpha,beta,A,X,Y,R,ts,dbleps,"zhemv(case 1081)");
  };


  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(1., 0.);
    double A[] = { 0.036, -0.966 };
    double X[] = { -0.695, 0.886 };
    double Y[] = { 0.486, 0.629 };
    double R[] = { 0.486, 0.629 };
    test_hemv<double>(102,121,1,alpha,beta,A,X,Y,R,ts,dbleps,"zhemv(case 1082)");
  };


  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(1., 0.);
    double A[] = { 0.036, -0.966 };
    double X[] = { -0.695, 0.886 };
    double Y[] = { 0.486, 0.629 };
    double R[] = { 0.486, 0.629 };
    test_hemv<double>(102,121,1,alpha,beta,A,X,Y,R,ts,dbleps,"zhemv(case 1083)");
  };


  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(1., 0.);
    double A[] = { 0.036, -0.966 };
    double X[] = { -0.695, 0.886 };
    double Y[] = { 0.486, 0.629 };
    double R[] = { 0.486, 0.629 };
    test_hemv<double>(102,122,1,alpha,beta,A,X,Y,R,ts,dbleps,"zhemv(case 1084)");
  };


  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(1., 0.);
    double A[] = { 0.036, -0.966 };
    double X[] = { -0.695, 0.886 };
    double Y[] = { 0.486, 0.629 };
    double R[] = { 0.486, 0.629 };
    test_hemv<double>(102,122,1,alpha,beta,A,X,Y,R,ts,dbleps,"zhemv(case 1085)");
  };


}
/*
  (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
\\s-*int uplo\\s-*=\\s-*\\(.*\\);
\\s-*\\(float\\|double\\) alpha\\[.\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*\\3 beta\\[.\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int N\\s-*=\\s-*\\(.*\\);
\\s-*int lda\\s-*.*;
\\s-*\\3 A\\[\\]\\s-*=\\s-*\\(.*\\);
\\s-*\\3 X\\[\\]\\s-*=\\s-*\\(.*\\);
\\s-*int incX.*
\\s-*\\3 Y\\[\\]\\s-*=\\s-*\\(.*\\);
\\s-*int incY.*"
  " gslmm::complex<\\3> alpha(\\4);
    gslmm::complex<\\3> beta(\\5);
    \\3 A[] = \\7;
    \\3 X[] = \\8;
    \\3 Y[] = \\9;
    test_hemv<\\3>(\\1,\\2,\\6,alpha,beta,A,X,Y,R);")

  (query-replace-regexp 
  "test_hemv<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\1 y_expected\\[\\]\\s-*=\\s-*\\(.*\\);
\\s-*cblas_.hemv.*
\\s-*{
\\s-*int i;
\\s-*for.*{
\\s-*gsl_test_rel(Y\\[.*\\],\\s-*y_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps,\\s-*\"\\(.*)\\).*\");[^}]*
\\s-*};?
\\s-*};"
  " \\1 R[] = \\3;
    test_hemv<\\1>(\\2,ts,\\4eps,\"\\5\");")

*/
