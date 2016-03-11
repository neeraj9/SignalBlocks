#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

    
//____________________________________________________________________
template <typename T> 
void
test_her2k(int order, int trans, int uplo,
	  size_t m, size_t n, 
	  const gslmm::complex<T>& alpha, 
	  const T& beta, 
	  T* a, T* b, T* c, T* r, 
	  gslmm::test_suite& ts, 
	  const T& tol, const char* msg) 
{
  using namespace gslmm;

  size_t ma = (CblasNoTrans == trans) ? m : n;
  size_t na = (CblasNoTrans == trans) ? n : m;
  // if (CblasNoTrans == trans) trans = CblasConjTrans;
  // else trans = CblasNoTrans;
			       
  if (order == 102) {
    // Input is in column order mode - restructure 
    make_complex_row_major(ma, na, a);
    make_complex_row_major(ma, na, b);
    make_complex_row_major(m, m, c);
    make_complex_row_major(m, m, r);
  }
  matrix<complex<T> > A(a, ma, na);
  matrix<complex<T> > B(b, ma, na);
  matrix<complex<T> > C(c, m,  m);
  matrix<complex<T> > R(r, m,  m);
  complex<T>          bb(beta, 0);

  rank_2k_update(alpha, 
		 matrix_manip<complex<T> >(A, 
					   general_type,
					   transform(trans)),
		 matrix_manip<complex<T> >(B), 
		 bb,
		 matrix_manip<complex<T> >(C, hermitian_type,
					   no_transform, 
					   location(uplo)));
  test_complex_result(C, R, ts, tol, msg);
}


//____________________________________________________________________
void
do_test_her2k (gslmm::test_suite& ts) 
{
  const double /* flteps = 1e-4,*/ dbleps = 1e-6;
#if 0
  {
    gslmm::complex<float> alpha(-1.0f, 0.0f);
    float A[]	= { 0.178f, 0.545f, -0.491f, 0.979f };
    float B[]	= { -0.665f, -0.531f, -0.4f, 0.227f };
    float C[]	= { 0.115f, -0.193f };
    float R[]	= { -0.056236f, 0.0f };
    test_her2k<float>(101,111,121,1,2,alpha,-0.3f,A,B,C,R,ts,flteps,"cher2k(case 1646)");
  };


  {
    gslmm::complex<float> alpha(-1.0f, 0.0f);
    float A[]	= { -0.808f, 0.447f, 0.145f, -0.226f };
    float B[]	= { -0.413f, 0.904f, -0.585f, 0.717f };
    float C[]	= { -0.725f, -0.244f };
    float R[]	= { -0.76435f, 0.0f };
    test_her2k<float>(101,111,122,1,2,alpha,-0.3f,A,B,C,R,ts,flteps,"cher2k(case 1647)");
  };


  {
    gslmm::complex<float> alpha(-1.0f, 0.0f);
    float A[]	= { 0.337f, -0.737f, -0.993f, 0.69f };
    float B[]	= { -0.39f, -0.836f, -0.32f, 0.368f };
    float C[]	= { 0.844f, -0.763f };
    float R[]	= { -2.36596f, 0.0f };
    test_her2k<float>(102,111,121,1,2,alpha,-0.3f,A,B,C,R,ts,flteps,"cher2k(case 1648)");
  };


  {
    gslmm::complex<float> alpha(-1.0f, 0.0f);
    float A[]	= { 0.386f, -0.465f, 0.719f, -0.378f };
    float B[]	= { 0.099f, -0.879f, 0.864f, 0.141f };
    float C[]	= { -0.599f, -0.47f };
    float R[]	= { -1.85003f, 0.0f };
    test_her2k<float>(102,111,122,1,2,alpha,-0.3f,A,B,C,R,ts,flteps,"cher2k(case 1649)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 1.0f);
    float A[]	= { 0.128f, 0.431f, -0.26f, 0.75f };
    float B[]	= { 0.276f, 0.058f, 0.904f, -0.116f };
    float C[]	= { 0.914f, -0.262f };
    float R[]	= { 0.604744f, 0.0f };
    test_her2k<float>(101,113,121,1,2,alpha,-1.0f,A,B,C,R,ts,flteps,"cher2k(case 1650)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 1.0f);
    float A[]	= { 0.72f, 0.783f, -0.737f, 0.375f };
    float B[]	= { 0.531f, 0.167f, 0.203f, -0.221f };
    float C[]	= { 0.618f, 0.392f };
    float R[]	= { -0.200438f, 0.0f };
    test_her2k<float>(101,113,122,1,2,alpha,-1.0f,A,B,C,R,ts,flteps,"cher2k(case 1651)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 1.0f);
    float A[]	= { -0.372f, -0.735f, -0.711f, 0.051f };
    float B[]	= { 0.257f, 0.097f, 0.338f, -0.484f };
    float C[]	= { -0.142f, -0.197f };
    float R[]	= { -0.817394f, 0.0f };
    test_her2k<float>(102,113,121,1,2,alpha,-1.0f,A,B,C,R,ts,flteps,"cher2k(case 1652)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 1.0f);
    float A[]	= { 0.1f, -0.878f, 0.28f, -0.381f };
    float B[]	= { -0.208f, 0.309f, -0.276f, 0.123f };
    float C[]	= { 0.483f, -0.541f };
    float R[]	= { -0.03812f, 0.0f };
    test_her2k<float>(102,113,122,1,2,alpha,-1.0f,A,B,C,R,ts,flteps,"cher2k(case 1653)");
  };
#endif

  {
    gslmm::complex<double> alpha(1, 0);
    double A[]	= { 0.515, -0.034, 0.067, 0.66 };
    double B[]	= { 0.408, -0.85, -0.945, -0.799 };
    double C[]	= { -0.918, -0.985 };
    double R[]	= { -1.62127, 0.0 };
    test_her2k<double>(101,111,121,1,2,alpha,1,A,B,C,R,ts,dbleps,"zher2k(case 1654)");
  };


  {
    gslmm::complex<double> alpha(1, 0);
    double A[]	= { -0.009, 0.495, -0.008, -0.973 };
    double B[]	= { -0.239, -0.373, -0.032, -0.539 };
    double C[]	= { 0.443, -0.245 };
    double R[]	= { 1.127438, 0.0 };
    test_her2k<double>(101,111,122,1,2,alpha,1,A,B,C,R,ts,dbleps,"zher2k(case 1655)");
  };


  {
    gslmm::complex<double> alpha(1, 0);
    double A[]	= { 0.531, 0.721, -0.848, 0.826 };
    double B[]	= { -0.711, -0.2, -0.92, -0.676 };
    double C[]	= { -0.447, 0.701 };
    double R[]	= { -1.046914, 0.0 };
    test_her2k<double>(102,111,121,1,2,alpha,1,A,B,C,R,ts,dbleps,"zher2k(case 1656)");
  };


  {
    gslmm::complex<double> alpha(1, 0);
    double A[]	= { 0.68, 0.079, 0.837, -0.814 };
    double B[]	= { -0.986, 0.024, 0.584, -0.248 };
    double C[]	= { 0.477, -0.551 };
    double R[]	= { 0.521192, 0.0 };
    test_her2k<double>(102,111,122,1,2,alpha,1,A,B,C,R,ts,dbleps,"zher2k(case 1657)");
  };


  {
    gslmm::complex<double> alpha(-1, 0);
    double A[]	= { -0.63, 0.787, 0.426, -0.568 };
    double B[]	= { -0.228, 0.302, 0.83, 0.023 };
    double C[]	= { 0.354, -0.85 };
    double R[]	= { -1.40826, 0.0 };
    test_her2k<double>(101,113,121,1,2,alpha,0.1,A,B,C,R,ts,dbleps,"zher2k(case 1658)");
  };


  {
    gslmm::complex<double> alpha(-1, 0);
    double A[]	= { 0.224, -0.191, 0.46, 0.464 };
    double B[]	= { -0.815, 0.634, 0.066, -0.873 };
    double C[]	= { -0.49, -0.606 };
    double R[]	= { 1.307732, 0.0 };
    test_her2k<double>(101,113,122,1,2,alpha,0.1,A,B,C,R,ts,dbleps,"zher2k(case 1659)");
  };


  {
    gslmm::complex<double> alpha(-1, 0);
    double A[]	= { 0.943, 0.075, 0.15, -0.141 };
    double B[]	= { -0.962, 0.422, -0.592, -0.789 };
    double C[]	= { 0.728, 0.601 };
    double R[]	= { 1.778934, 0.0 };
    test_her2k<double>(102,113,121,1,2,alpha,0.1,A,B,C,R,ts,dbleps,"zher2k(case 1660)");
  };


  {
    gslmm::complex<double> alpha(-1, 0);
    double A[]	= { -0.93, -0.386, 0.565, 0.141 };
    double B[]	= { -0.801, 0.022, 0.558, -0.932 };
    double C[]	= { 0.068, 0.501 };
    double R[]	= { -1.833792, 0.0 };
    test_her2k<double>(102,113,122,1,2,alpha,0.1,A,B,C,R,ts,dbleps,"zher2k(case 1661)");
  };
}

/*
   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int uplo\\s-*=\\s-*\\(.*\\);
   int trans\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);
   int K\\s-*=\\s-*\\(.*\\);
   \\(float\\|double\\) alpha\\[2\\]\\s-*=\\s-*\\(.*\\);
   \\6 beta\\s-*=\\s-*\\(.*\\);"
  " gslmm::complex<\\6> alpha(\\7);
    test_herk2<\\6>(\\1,\\3,\\2,\\4,\\5,alpha,\\8);")

   (search-forward-regexp 
   "test_herk2<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\1 A\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int lda.*;")

   (query-replace-regexp 
   "test_herk2<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\1 A\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int lda.*;
\\s-*\\1 B\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldb.*;
\\s-*\\1 C\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldc.*;
\\s-*\\1 C_expected\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*cblas_.her2k(.*);
\\s-*{
\\s-*int i;
\\s-*for.*{
\\s-*gsl_test_rel(C\\[.*\\], C_expected\\[.*], \\(flt\\|dbl\\)eps, \"\\(.*\\) real\");
\\s-*gsl_test_rel(C\\[.*], C_expected\\[.*], \\7eps, \"\\8 imag\");
\\s-*};?
\\s-*};"
    "\\1 A[]	 = {\\3};
    \\1 B[]     = {\\4};
    \\1 C[]	= {\\5};
    \\1 R[]	= {\\6};
    test_her2k<\\1>(\\2,A,B,C,R,ts,\\7eps,\"\\8\");")
    
*/
