#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

    
//____________________________________________________________________
template <typename T> 
void
test_her(int order, int uplo,
	 size_t n, 
	 const T& alpha, 
	 T* a, T* x, T* r, 
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
  matrix<complex<T> > R(r, n,  n);
  complex<T>          aa(alpha, 0);

  rank_1_update(aa, X, X, 
		matrix_manip<complex<T> >(A, 
					  hermitian_type,
					  no_transform,
					  location(uplo)));
  test_complex_result(A, R, ts, tol, msg);
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
   \\4 A_expected\\[\\]\\s-*=\\s-*\\(.*\\);"
  " \\4 A[] = \\6;
    \\4 X[] = \\7;
    \\4 R[] = \\8;
    test_her<\\4>(\\1,\\2,\\3,\\5,A,X,R);")

   (query-replace-regexp 
   "test_her<\\(float\\|double\\)>(\\(.*\\));
   cblas_.her(.*);
   {
     int i;
     for.*{
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps, \"\\(.*\\) real\");
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\3eps, \"\\4 imag\");
     };?
   };" "test_her<\\1>(\\2,ts,\\3eps,\"\\4\");")
*/

//____________________________________________________________________
void
do_test_her (gslmm::test_suite& ts) 
{
  const double /* flteps = 1e-4, */ dbleps = 1e-6;
#if 0
  {
    float A[] = { 0.188f, 0.856f };
    float X[] = { -0.832f, -0.151f };
    float R[] = { 0.903025f, 0.0f };
    test_her<float>(101,121,1,1.0f,A,X,R,ts,flteps,"cher(case 1410)");
  };


  {
    float A[] = { 0.188f, 0.856f };
    float X[] = { -0.832f, -0.151f };
    float R[] = { 0.903025f, 0.0f };
    test_her<float>(101,122,1,1.0f,A,X,R,ts,flteps,"cher(case 1411)");
  };


  {
    float A[] = { 0.188f, 0.856f };
    float X[] = { -0.832f, -0.151f };
    float R[] = { 0.903025f, 0.0f };
    test_her<float>(102,121,1,1.0f,A,X,R,ts,flteps,"cher(case 1412)");
  };


  {
    float A[] = { 0.188f, 0.856f };
    float X[] = { -0.832f, -0.151f };
    float R[] = { 0.903025f, 0.0f };
    test_her<float>(102,122,1,1.0f,A,X,R,ts,flteps,"cher(case 1413)");
  };
#endif


  {
    double A[] = { 0.257, 0.326 };
    double X[] = { 0.319, -0.009 };
    double R[] = { 0.2671842, 0.0 };
    test_her<double>(101,121,1,0.1,A,X,R,ts,dbleps,"zher(case 1414)");
  };


  {
    double A[] = { 0.257, 0.326 };
    double X[] = { 0.319, -0.009 };
    double R[] = { 0.2671842, 0.0 };
    test_her<double>(101,122,1,0.1,A,X,R,ts,dbleps,"zher(case 1415)");
  };


  {
    double A[] = { 0.257, 0.326 };
    double X[] = { 0.319, -0.009 };
    double R[] = { 0.2671842, 0.0 };
    test_her<double>(102,121,1,0.1,A,X,R,ts,dbleps,"zher(case 1416)");
  };


  {
    double A[] = { 0.257, 0.326 };
    double X[] = { 0.319, -0.009 };
    double R[] = { 0.2671842, 0.0 };
    test_her<double>(102,122,1,0.1,A,X,R,ts,dbleps,"zher(case 1417)");
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
   \\4 A_expected\\[\\]\\s-*=\\s-*\\(.*\\);"
  " \\4 A[] = \\6;
    \\4 X[] = \\7;
    \\4 R[] = \\8;
    test_her<\\4>(\\1,\\2,\\3,\\5,A,X,R);")

   (query-replace-regexp 
   "test_her<\\(float\\|double\\)>(\\(.*\\));
   cblas_.her(.*);
   {
     int i;
     for.*{
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps, \"\\(.*\\) real\");
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\3eps, \"\\4 imag\");
     };?
   };" "test_her<\\1>(\\2,ts,\\3eps,\"\\4\");")
*/
