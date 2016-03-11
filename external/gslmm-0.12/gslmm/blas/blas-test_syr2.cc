#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

    
//____________________________________________________________________
template <typename T> 
void
test_syr2(int order, int uplo,
	  size_t n, 
	  const T& alpha, 
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
    make_row_major(n, n, a);
  }
  matrix<T> A(a, n, n);
  vector<T> X(x, n);
  vector<T> Y(y, n);
  matrix<T> R(r, n,  n);

  rank_2_update(alpha, X, Y, 
		matrix_manip<T>(A, symmetric_type,
				no_transform,
				location(uplo)));
  test_result(A, R, ts, tol, msg);
}

//____________________________________________________________________
void
do_test_syr2 (gslmm::test_suite& ts) 
{
  const double flteps = 1e-4, dbleps = 1e-6;
  {
    float A[] = { 0.862f };
    float X[] = { 0.823f };
    float Y[] = { 0.699f };
    float R[] = { 0.862f };
    test_syr2<float>(101,121,1,0.0f,A,X,Y,R,ts,flteps,"ssyr2(case 1434)");
  };


  {
    float A[] = { 0.862f };
    float X[] = { 0.823f };
    float Y[] = { 0.699f };
    float R[] = { 0.862f };
    test_syr2<float>(101,122,1,0.0f,A,X,Y,R,ts,flteps,"ssyr2(case 1435)");
  };


  {
    float A[] = { 0.862f };
    float X[] = { 0.823f };
    float Y[] = { 0.699f };
    float R[] = { 0.862f };
    test_syr2<float>(102,121,1,0.0f,A,X,Y,R,ts,flteps,"ssyr2(case 1436)");
  };


  {
    float A[] = { 0.862f };
    float X[] = { 0.823f };
    float Y[] = { 0.699f };
    float R[] = { 0.862f };
    test_syr2<float>(102,122,1,0.0f,A,X,Y,R,ts,flteps,"ssyr2(case 1437)");
  };


  {
    double A[] = { -0.824 };
    double X[] = { 0.684 };
    double Y[] = { 0.965 };
    double R[] = { -0.824 };
    test_syr2<double>(101,121,1,0,A,X,Y,R,ts,dbleps,"dsyr2(case 1438)");
  };


  {
    double A[] = { -0.824 };
    double X[] = { 0.684 };
    double Y[] = { 0.965 };
    double R[] = { -0.824 };
    test_syr2<double>(101,122,1,0,A,X,Y,R,ts,dbleps,"dsyr2(case 1439)");
  };


  {
    double A[] = { -0.824 };
    double X[] = { 0.684 };
    double Y[] = { 0.965 };
    double R[] = { -0.824 };
    test_syr2<double>(102,121,1,0,A,X,Y,R,ts,dbleps,"dsyr2(case 1440)");
  };


  {
    double A[] = { -0.824 };
    double X[] = { 0.684 };
    double Y[] = { 0.965 };
    double R[] = { -0.824 };
    test_syr2<double>(102,122,1,0,A,X,Y,R,ts,dbleps,"dsyr2(case 1441)");
  };


}
/*
   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int uplo\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);
   int lda.*
   \\(float\\|double\\) alpha\\s-*=\\s-*\\(.*\\);
   \\4 A\\[\\]\\s-*=\\s-*\\(.*\\);
   \\4 X\\[\\]\\s-*=\\s-*\\(.*\\);
   int incX.*
   \\4 Y\\[\\]\\s-*=\\s-*\\(.*\\);
   int incY.*
   \\4 A_expected\\[\\]\\s-*=\\s-*\\(.*\\);"
  " \\4 A[] = \\6;
    \\4 X[] = \\7;
    \\4 Y[] = \\8;
    \\4 R[] = \\9;
    test_syr2<\\4>(\\1,\\2,\\3,\\5,A,X,Y,R);")

   (query-replace-regexp 
   "test_syr2<\\(float\\|double\\)>(\\(.*\\));
   cblas_.syr2(.*);
   {
     int i;
     for.*{
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps, \"\\(.*\\)\");
     };?
   };" "test_syr2<\\1>(\\2,ts,\\3eps,\"\\4\");")
*/

