#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

    
//____________________________________________________________________
template <typename T> 
void
test_ger(int order, size_t m, size_t n, 
	 const T& alpha, T* a, T* x, T* y, T* r, 
	 gslmm::test_suite& ts,const T& tol, const char* msg) 
{
  using namespace gslmm;

  // size_t ma = (CblasNoTrans == trans) ? n : m;
  // size_t na = (CblasNoTrans == trans) ? m : n;
  // if (CblasNoTrans == trans) trans = CblasConjTrans;
  // else trans = CblasNoTrans;
			       
  if (order == 102) {
    // Input is in column order mode - restructure 
    make_row_major(m, n, a);
  }
  matrix<T> A(a, m, n);
  vector<T> X(x, n);
  vector<T> Y(y, n);
  matrix<T> R(r, m,  n);

  rank_1_update(alpha, X, Y, A);
  test_result(A, R, ts, tol, msg);
}

//____________________________________________________________________
template <typename T> 
void
test_complex_ger(int order,
		 size_t m, size_t n, 
		 const gslmm::complex<T>& alpha, 
		 T* a, T* x, T* y, T* r, 
		 bool conj, 
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
    make_complex_row_major(m, n, a);
  }
  matrix<complex<T> > A(a, m, n);
  vector<complex<T> > X(x, n);
  vector<complex<T> > Y(y, n);
  matrix<complex<T> > R(r, m,  n);

  rank_1_update(alpha, X, Y, A, conj);
  test_complex_result(A, R, ts, tol, msg);
}


//____________________________________________________________________
void
do_test_ger (gslmm::test_suite& ts) 
{
  const double flteps = 1e-4, dbleps = 1e-6;
  {
    float A[] = { -0.515f };
    float X[] = { 0.611f };
    float Y[] = { -0.082f };
    float R[] = { -0.565102f };
    test_ger<float>(101,1,1,1.0f,A,X,Y,R,ts,flteps,"sger(case 1390)");
  };


  {
    float A[] = { -0.515f };
    float X[] = { 0.611f };
    float Y[] = { -0.082f };
    float R[] = { -0.565102f };
    test_ger<float>(102,1,1,1.0f,A,X,Y,R,ts,flteps,"sger(case 1391)");
  };


  {
    double A[] = { -0.809 };
    double X[] = { -0.652 };
    double Y[] = { 0.712 };
    double R[] = { -1.273224 };
    test_ger<double>(101,1,1,1,A,X,Y,R,ts,dbleps,"dger(case 1392)");
  };


  {
    double A[] = { -0.809 };
    double X[] = { -0.652 };
    double Y[] = { 0.712 };
    double R[] = { -1.273224 };
    test_ger<double>(102,1,1,1,A,X,Y,R,ts,dbleps,"dger(case 1393)");
  };

#if 0
  {
    gslmm::complex<float> alpha(0.0f, 0.0f);
    float A[] = { -0.651f, 0.856f };
    float X[] = { -0.38f, -0.235f };
    float Y[] = { -0.627f, 0.757f };
    float R[] = { -0.651f, 0.856f };
    test_complex_ger<float>(101,1,1,alpha,A,X,Y,R,false,ts,flteps,"cgeru(case 1394)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.0f);
    float A[] = { -0.651f, 0.856f };
    float X[] = { -0.38f, -0.235f };
    float Y[] = { -0.627f, 0.757f };
    float R[] = { -0.651f, 0.856f };
    test_complex_ger<float>(101,1,1,alpha,A,X,Y,R,true,ts,flteps,"cgerc(case 1395)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.0f);
    float A[] = { -0.651f, 0.856f };
    float X[] = { -0.38f, -0.235f };
    float Y[] = { -0.627f, 0.757f };
    float R[] = { -0.651f, 0.856f };
    test_complex_ger<float>(102,1,1,alpha,A,X,Y,R,false,ts,flteps,"cgeru(case 1396)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.0f);
    float A[] = { -0.651f, 0.856f };
    float X[] = { -0.38f, -0.235f };
    float Y[] = { -0.627f, 0.757f };
    float R[] = { -0.651f, 0.856f };
    test_complex_ger<float>(102,1,1,alpha,A,X,Y,R,true,ts,flteps,"cgerc(case 1397)");
  };
#endif

  {
    gslmm::complex<double> alpha(-1, 0);
    double A[] = { -0.426, 0.757 };
    double X[] = { -0.579, -0.155 };
    double Y[] = { 0.831, 0.035 };
    double R[] = { 0.049724, 0.90607 };
    test_complex_ger<double>(101,1,1,alpha,A,X,Y,R,false,ts,dbleps,"zgeru(case 1398)");
  };


  {
    gslmm::complex<double> alpha(-1, 0);
    double A[] = { -0.426, 0.757 };
    double X[] = { -0.579, -0.155 };
    double Y[] = { 0.831, 0.035 };
    double R[] = { 0.060574, 0.86554 };
    test_complex_ger<double>(101,1,1,alpha,A,X,Y,R,true,ts,dbleps,"zgerc(case 1399)");
  };


  {
    gslmm::complex<double> alpha(-1, 0);
    double A[] = { -0.426, 0.757 };
    double X[] = { -0.579, -0.155 };
    double Y[] = { 0.831, 0.035 };
    double R[] = { 0.049724, 0.90607 };
    test_complex_ger<double>(102,1,1,alpha,A,X,Y,R,false,ts,dbleps,"zgeru(case 1400)");
  };


  {
    gslmm::complex<double> alpha(-1, 0);
    double A[] = { -0.426, 0.757 };
    double X[] = { -0.579, -0.155 };
    double Y[] = { 0.831, 0.035 };
    double R[] = { 0.060574, 0.86554 };
    test_complex_ger<double>(102,1,1,alpha,A,X,Y,R,true,ts,dbleps,"zgerc(case 1401)");
  };


}
/*
   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int M\\s-*=\\s-*\\(.*\\);
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
    test_ger<\\4>(\\1,\\2,\\3,\\5,A,X,Y,R);")

   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int M\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);
   int lda.*
   \\(float\\|double\\) alpha\\[2\\]\\s-*=\\s-*\\(.*\\);
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
    test_complex_ger<\\4>(\\1,\\2,\\3,alpha,A,X,Y,R);")

   (query-replace-regexp 
   "test_ger<\\(float\\|double\\)>(\\(.*\\));
   cblas_.ger(.*);
   {
     int i;
     for.*{
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps, \"\\(.*\\)\");
     };?
   };" "test_ger<\\1>(\\2,ts,\\3eps,\"\\4\");")

   (query-replace-regexp 
   "test_complex_ger<\\(float\\|double\\)>(\\(.*\\));
   cblas_.ger\\(u\\|c\\)(.*);
   {
     int i;
     for.*{
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps, \"\\(.*)\\) real\");
       gsl_test_rel(A\\[.*\\],\\s-*A_expected\\[.*\\],\\s-*\\4eps, \"\\5 imag\");
     };?
   };" "test_ger<\\1>(\\2,\\3,ts,\\4eps,\"\\5\");")
*/
