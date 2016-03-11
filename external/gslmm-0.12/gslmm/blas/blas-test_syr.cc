#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

    
//____________________________________________________________________
template <typename T> 
void
test_syr(int order, int uplo,
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
    make_row_major(n, n, a);
  }
  matrix<T>       A(a, n, n);
  const vector<T> X(x, n);
  matrix<T>       R(r, n,  n);

  rank_1_update(alpha, X, X, 
		matrix_manip<T>(A, 
				symmetric_type,
				no_transform,
				location(uplo)));
  test_result(A, R, ts, tol, msg);
}


//____________________________________________________________________
void
do_test_syr (gslmm::test_suite& ts) {
const double flteps = 1e-4, dbleps = 1e-6;
  {
    float A[] = { -0.291f };
    float X[] = { 0.845f };
    float R[] = { -0.219597f };
    test_syr<float>(101,121,1,0.1f,A,X,R,ts,flteps,"ssyr(case 1402)");
  };


  {
    float A[] = { -0.291f };
    float X[] = { 0.845f };
    float R[] = { -0.219597f };
    test_syr<float>(101,122,1,0.1f,A,X,R,ts,flteps,"ssyr(case 1403)");
  };


  {
    float A[] = { -0.291f };
    float X[] = { 0.845f };
    float R[] = { -0.219597f };
    test_syr<float>(102,121,1,0.1f,A,X,R,ts,flteps,"ssyr(case 1404)");
  };


  {
    float A[] = { -0.291f };
    float X[] = { 0.845f };
    float R[] = { -0.219597f };
    test_syr<float>(102,122,1,0.1f,A,X,R,ts,flteps,"ssyr(case 1405)");
  };


  {
    double A[] = { -0.65 };
    double X[] = { -0.891 };
    double R[] = { -0.8881643 };
    test_syr<double>(101,121,1,-0.3,A,X,R,ts,dbleps,"dsyr(case 1406)");
  };


  {
    double A[] = { -0.65 };
    double X[] = { -0.891 };
    double R[] = { -0.8881643 };
    test_syr<double>(101,122,1,-0.3,A,X,R,ts,dbleps,"dsyr(case 1407)");
  };


  {
    double A[] = { -0.65 };
    double X[] = { -0.891 };
    double R[] = { -0.8881643 };
    test_syr<double>(102,121,1,-0.3,A,X,R,ts,dbleps,"dsyr(case 1408)");
  };


  {
    double A[] = { -0.65 };
    double X[] = { -0.891 };
    double R[] = { -0.8881643 };
    test_syr<double>(102,122,1,-0.3,A,X,R,ts,dbleps,"dsyr(case 1409)");
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
    test_syr<\\4>(\\1,\\2,\\3,\\5,A,X,R);")

   (query-replace-regexp 
   "test_syr<\\(float\\|double\\)>(\\(.*\\));
   cblas_.syr(.*);
   {
     int i;
     for.*{
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps, \"\\(.*\\)\");
     };?
   };" "test_syr<\\1>(\\2,ts,\\3eps,\"\\4\");")
*/
