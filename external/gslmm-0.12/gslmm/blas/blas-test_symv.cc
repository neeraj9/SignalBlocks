#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

//____________________________________________________________________
template <typename T> 
void
test_symv(int order, int uplo,
	  size_t n,
	  const T& alpha, const T& beta, 
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
  
  matrix<T> A(a, ma, na);
  vector<T> X(x, ma);
  vector<T> Y(y, ma);
  vector<T> R(r, ma);

  matrix_vector_product(alpha, 
			matrix_manip<T>(A, symmetric_type, 
					no_transform,
					location(uplo)), 
			X, beta, Y);
  test_result(Y, R, ts, tol, msg);
}


//____________________________________________________________________
void
do_test_symv (gslmm::test_suite& ts) 
{
  const double flteps = 1e-4, dbleps = 1e-6;
  {
    float A[] = { -0.428f };
    float X[] = { -0.34f };
    float Y[] = { -0.888f };
    float R[] = { 1.03352f };
    test_symv<float>(101,121,1,1.0f,-1.0f,A,X,Y,R,ts,flteps,"ssymv(case 1054)");
  };


  {
    float A[] = { -0.428f };
    float X[] = { -0.34f };
    float Y[] = { -0.888f };
    float R[] = { 1.03352f };
    test_symv<float>(101,121,1,1.0f,-1.0f,A,X,Y,R,ts,flteps,"ssymv(case 1055)");
  };


  {
    float A[] = { -0.428f };
    float X[] = { -0.34f };
    float Y[] = { -0.888f };
    float R[] = { 1.03352f };
    test_symv<float>(101,122,1,1.0f,-1.0f,A,X,Y,R,ts,flteps,"ssymv(case 1056)");
  };


  {
    float A[] = { -0.428f };
    float X[] = { -0.34f };
    float Y[] = { -0.888f };
    float R[] = { 1.03352f };
    test_symv<float>(101,122,1,1.0f,-1.0f,A,X,Y,R,ts,flteps,"ssymv(case 1057)");
  };


  {
    float A[] = { -0.428f };
    float X[] = { -0.34f };
    float Y[] = { -0.888f };
    float R[] = { 1.03352f };
    test_symv<float>(102,121,1,1.0f,-1.0f,A,X,Y,R,ts,flteps,"ssymv(case 1058)");
  };


  {
    float A[] = { -0.428f };
    float X[] = { -0.34f };
    float Y[] = { -0.888f };
    float R[] = { 1.03352f };
    test_symv<float>(102,121,1,1.0f,-1.0f,A,X,Y,R,ts,flteps,"ssymv(case 1059)");
  };


  {
    float A[] = { -0.428f };
    float X[] = { -0.34f };
    float Y[] = { -0.888f };
    float R[] = { 1.03352f };
    test_symv<float>(102,122,1,1.0f,-1.0f,A,X,Y,R,ts,flteps,"ssymv(case 1060)");
  };


  {
    float A[] = { -0.428f };
    float X[] = { -0.34f };
    float Y[] = { -0.888f };
    float R[] = { 1.03352f };
    test_symv<float>(102,122,1,1.0f,-1.0f,A,X,Y,R,ts,flteps,"ssymv(case 1061)");
  };


  {
    double A[] = { 0.544 };
    double X[] = { -0.601 };
    double Y[] = { -0.852 };
    double R[] = { 0.2556 };
    test_symv<double>(101,121,1,0,-0.3,A,X,Y,R,ts,dbleps,"dsymv(case 1062)");
  };


  {
    double A[] = { 0.544 };
    double X[] = { -0.601 };
    double Y[] = { -0.852 };
    double R[] = { 0.2556 };
    test_symv<double>(101,121,1,0,-0.3,A,X,Y,R,ts,dbleps,"dsymv(case 1063)");
  };


  {
    double A[] = { 0.544 };
    double X[] = { -0.601 };
    double Y[] = { -0.852 };
    double R[] = { 0.2556 };
    test_symv<double>(101,122,1,0,-0.3,A,X,Y,R,ts,dbleps,"dsymv(case 1064)");
  };


  {
    double A[] = { 0.544 };
    double X[] = { -0.601 };
    double Y[] = { -0.852 };
    double R[] = { 0.2556 };
    test_symv<double>(101,122,1,0,-0.3,A,X,Y,R,ts,dbleps,"dsymv(case 1065)");
  };


  {
    double A[] = { 0.544 };
    double X[] = { -0.601 };
    double Y[] = { -0.852 };
    double R[] = { 0.2556 };
    test_symv<double>(102,121,1,0,-0.3,A,X,Y,R,ts,dbleps,"dsymv(case 1066)");
  };


  {
    double A[] = { 0.544 };
    double X[] = { -0.601 };
    double Y[] = { -0.852 };
    double R[] = { 0.2556 };
    test_symv<double>(102,121,1,0,-0.3,A,X,Y,R,ts,dbleps,"dsymv(case 1067)");
  };


  {
    double A[] = { 0.544 };
    double X[] = { -0.601 };
    double Y[] = { -0.852 };
    double R[] = { 0.2556 };
    test_symv<double>(102,122,1,0,-0.3,A,X,Y,R,ts,dbleps,"dsymv(case 1068)");
  };


  {
    double A[] = { 0.544 };
    double X[] = { -0.601 };
    double Y[] = { -0.852 };
    double R[] = { 0.2556 };
    test_symv<double>(102,122,1,0,-0.3,A,X,Y,R,ts,dbleps,"dsymv(case 1069)");
  };
}

/*
  (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
  int uplo\\s-*=\\s-*\\(.*\\);
  \\(float\\|double\\) alpha\\s-*=\\s-*\\(.*\\);
  \\3 beta\\s-*=\\s-*\\(.*\\);
  int N\\s-*=\\s-*\\(.*\\);
  int lda\\s-*.*;
  \\3 A\\[\\]\\s-*=\\s-*\\(.*\\);
  \\3 X\\[\\]\\s-*=\\s-*\\(.*\\);
  int incX.*
  \\3 Y\\[\\]\\s-*=\\s-*\\(.*\\);
  int incY.*"
  "\\3 A[] = \\7;
  \\3 X[] = \\8;
  \\3 Y[] = \\9;
  test_symv<\\3>(\\1,\\2,\\6,\\4,\\5,A,X,Y,R);")

  (query-replace-regexp 
  "test_symv<\\(float\\|double\\)>(\\(.*\\));
  \\1 y_expected\\[\\]\\s-*=\\s-*\\(.*\\);
  cblas_.symv.*
  \\s-*{
  \\s-*int i;
  \\s-*for.*{
  \\s-*gsl_test_rel(Y\\[.*\\],\\s-*y_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps,\\s-*\"\\(.*)\\).*\");[^}]*
  \\s-*};?
  \\s-*};"
  "\\1 R[] = \\3;
  test_symv<\\1>(\\2,ts,\\4eps,\"\\5\");")

*/
