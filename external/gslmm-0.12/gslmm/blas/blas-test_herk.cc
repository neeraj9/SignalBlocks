#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

    
//____________________________________________________________________
template <typename T> 
void
test_herk(int order, int trans, int uplo,
	  size_t m, size_t n, 
	  const T& alpha, 
	  const T& beta, 
	  T* a, T* b, T* r, 
	  gslmm::test_suite& ts, 
	  const T& tol, const char* msg) 
{
  using namespace gslmm;

#if GSL_VERSION_CODE >= 10800
  size_t ma = (CblasNoTrans == trans) ? m : n;
  size_t na = (CblasNoTrans == trans) ? n : m;
#else
  size_t ma = (CblasNoTrans == trans) ? n : m;
  size_t na = (CblasNoTrans == trans) ? m : n;
#endif
  // if (CblasNoTrans == trans) trans = CblasConjTrans;
  // else trans = CblasNoTrans;
			       
  if (order == 102) {
    // Input is in column order mode - restructure 
    make_complex_row_major(ma, na, a);
    make_complex_row_major(m, m, b);
    make_complex_row_major(m, m, r);
  }
  matrix<complex<T> > A(a, ma, na);
  matrix<complex<T> > B(b, m,  m);
  matrix<complex<T> > R(r, m,  m);
  complex<T>          aa(alpha, 0);
  complex<T>          bb(beta, 0);

  rank_k_update(aa, 
		matrix_manip<complex<T> >(A, 
					  general_type,
					  transform(trans)),
		bb,
		matrix_manip<complex<T> >(B, 
					  hermitian_type,
					  no_transform,
					  location(uplo)));
  test_complex_result(B, R, ts, tol, msg);
}


//____________________________________________________________________
void
do_test_herk (gslmm::test_suite& ts) 
{
  const double /* flteps = 1e-4, */ dbleps = 1e-6;
#if 0
  {
    float A[]	= { -0.617f, 0.179f, -0.626f, 0.334f };
    float C[]	= { 0.346f, -0.903f, 0.022f, -0.839f, -0.715f, 0.049f, -0.338f, 0.149f };
    float R[]	= { 0.0346f, 0.0f, 0.0022f, -0.0839f, -0.715f, 0.049f, -0.0338f, 0.0f };
    test_herk<float>(101,111,121,2,1,0.0f,0.1f,A,B,C,R,ts,flteps,"cherk(case 1598)");
  };


  {
    float A[]	= { -0.356f, -0.308f, 0.493f, -0.351f };
    float C[]	= { -0.898f, -0.905f, 0.002f, -0.219f, 0.881f, 0.879f, 0.275f, -0.351f };
    float R[]	= { -0.0898f, 0.0f, 0.002f, -0.219f, 0.0881f, 0.0879f, 0.0275f, 0.0f };
    test_herk<float>(102,111,121,2,1,0.0f,0.1f,A,B,C,R,ts,flteps,"cherk(case 1599)");
  };


  {
    float A[]	= { -0.103f, -0.951f, -0.601f, -0.041f };
    float C[]	= { -0.918f, -0.018f, 0.991f, -0.789f, -0.698f, -0.067f, 0.956f, -0.599f };
    float R[]	= { -0.826499f, 0.0f, 1.00109f, -0.845733f, -0.698f, -0.067f, 0.992288f, 0.0f };
    test_herk<float>(101,113,121,2,1,0.1f,1.0f,A,B,C,R,ts,flteps,"cherk(case 1600)");
  };


  {
    float A[]	= { -0.237f, 0.925f, -0.904f, -0.091f };
    float C[]	= { -0.572f, 0.915f, 0.398f, 0.222f, 0.016f, 0.288f, -0.078f, -0.507f };
    float R[]	= { -0.480821f, 0.0f, 0.398f, 0.222f, 0.0290073f, 0.373777f, 0.0045497f, 0.0f };
    test_herk<float>(102,113,121,2,1,0.1f,1.0f,A,B,C,R,ts,flteps,"cherk(case 1601)");
  };


  {
    float A[]	= { 0.963f, -0.23f, -0.435f, 0.289f };
    float C[]	= { 0.282f, -0.272f, -0.516f, -0.594f, -0.001f, 0.155f, -0.39f, -0.354f };
    float R[]	= { -0.294081f, 0.0f, -0.516f, -0.594f, 0.145613f, -0.0534771f, -0.0818238f, 0.0f };
    test_herk<float>(101,111,122,2,1,-0.3f,0.0f,A,B,C,R,ts,flteps,"cherk(case 1602)");
  };


  {
    float A[]	= { 0.674f, 0.1f, -0.098f, 0.552f };
    float C[]	= { 0.089f, -0.523f, -0.551f, 0.618f, 0.67f, 0.247f, 0.975f, -0.714f };
    float R[]	= { -0.139283f, 0.0f, 0.0032556f, -0.114554f, 0.67f, 0.247f, -0.0942924f, 0.0f };
    test_herk<float>(102,111,122,2,1,-0.3f,0.0f,A,B,C,R,ts,flteps,"cherk(case 1603)");
  };


  {
    float A[]	= { 0.033f, -0.864f, 0.168f, 0.524f };
    float C[]	= { 0.788f, 0.016f, -0.436f, 0.749f, -0.89f, -0.87f, 0.421f, -0.203f };
    float R[]	= { 0.826385f, 0.0f, -0.436f, 0.749f, -0.536192f, -0.249444f, 0.3449f, 0.0f };
    test_herk<float>(101,113,122,2,1,1.0f,0.1f,A,B,C,R,ts,flteps,"cherk(case 1604)");
  };


  {
    float A[]	= { 0.957f, -0.079f, 0.935f, 0.232f };
    float C[]	= { -0.744f, -0.061f, 0.195f, -0.574f, 0.551f, 0.478f, -0.337f, 0.1f };
    float R[]	= { 0.84769f, 0.0f, 0.895967f, -0.353289f, 0.551f, 0.478f, 0.894349f, 0.0f };
    test_herk<float>(102,113,122,2,1,1.0f,0.1f,A,B,C,R,ts,flteps,"cherk(case 1605)");
  };
#endif

  {
    double A[]	= { 0.934, 0.664, 0.426, 0.263 };
    double C[]	= { 0.251, -0.97, 0.76, -0.349, 0.152, -0.899, -0.17, 0.707 };
    double R[]	= { 1.564252, 0.0, 1.332516, -0.311778, 0.152, -0.899, 0.080645, 0.0 };
    test_herk<double>(101,111,121,2,1,1,1,A,C,R,ts,dbleps,"zherk(case 1606)");
  };


  {
    double A[]	= { 0.16, 0.464, -0.623, 0.776 };
    double C[]	= { 0.771, -0.449, 0.776, 0.112, -0.134, 0.317, 0.547, -0.551 };
    double R[]	= { 1.011896, 0.0, 0.776, 0.112, 0.126384, -0.096232, 1.537305, 0.0 };
    test_herk<double>(102,111,121,2,1,1,1,A,C,R,ts,dbleps,"zherk(case 1607)");
  };


  {
    double A[]	= { 0.787, 0.057, -0.49, 0.47 };
    double C[]	= { -0.758, 0.912, 0.992, -0.356, 0.584, 0.806, 0.965, 0.674 };
    double R[]	= { -0.6957382, 0.0, 0.956116, -0.316218, 0.584, 0.806, 1.0111, 0.0 };
    test_herk<double>(101,113,121,2,1,0.1,1,A,C,R,ts,dbleps,"zherk(case 1608)");
  };
  

  {
    double A[]	= { 0.961, -0.384, 0.165, 0.395 };
    double C[]	= { -0.186, 0.404, -0.873, 0.09, -0.451, -0.972, -0.203, -0.304 };
    double R[]	= { -0.0789023, 0.0, -0.873, 0.09, -0.4503115, -0.9277045, -0.184675, 0.0 };
    test_herk<double>(102,113,121,2,1,0.1,1,A,C,R,ts,dbleps,"zherk(case 1609)");
  };


  {
    double A[]	= { 0.04, 0.608, 0.21, -0.44 };
    double C[]	= { 0.285, -0.943, 0.581, -0.56, 0.112, 0.529, 0.16, -0.913 };
    double R[]	= { -0.0855, 0.0, 0.581, -0.56, -0.0336, -0.1587, -0.048, 0.0 };
    test_herk<double>(101,111,122,2,1,0,-0.3,A,C,R,ts,dbleps,"zherk(case 1610)");
  };


  {
    double A[]	= { -0.984, -0.398, -0.379, 0.919 };
    double C[]	= { -0.44, -0.087, 0.156, -0.945, -0.943, -0.355, 0.577, 0.053 };
    double R[]	= { 0.132, 0.0, -0.0468, 0.2835, -0.943, -0.355, -0.1731, 0.0 };
    test_herk<double>(102,111,122,2,1,0,-0.3,A,C,R,ts,dbleps,"zherk(case 1611)");
  };


  {
    double A[]	= { 0.269, -0.428, -0.029, 0.964 };
    double C[]	= { 0.473, -0.932, -0.689, -0.072, -0.952, -0.862, 0.001, 0.282 };
    double R[]	= { -0.217455, 0.0, -0.689, -0.072, 0.531607, 0.615096, 0.929137, 0.0 };
    test_herk<double>(101,113,122,2,1,1,-1,A,C,R,ts,dbleps,"zherk(case 1612)");
  };


  {
    double A[]	= { -0.303, -0.037, -0.411, -0.243 };
    double C[]	= { 0.652, -0.227, -0.849, 0.87, -0.051, -0.535, 0.418, -0.681 };
    double R[]	= { -0.558822, 0.0, 0.982524, -0.928422, -0.051, -0.535, -0.19003, 0.0 };
    test_herk<double>(102,113,122,2,1,1,-1,A,C,R,ts,dbleps,"zherk(case 1613)");
  };


}
/*
   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int uplo\\s-*=\\s-*\\(.*\\);
   int trans\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);
   int K\\s-*=\\s-*\\(.*\\);
   \\(float\\|double\\) alpha\\s-*=\\s-*\\(.*\\);
   \\6 beta\\s-*=\\s-*\\(.*\\);"
  "test_herk<\\6>(\\1,\\3,\\2,\\4,\\5,\\7,\\8);")

   (search-forward-regexp 
   "test_herk<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\1 A\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int lda.*;")

   (query-replace-regexp 
   "test_herk<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\1 A\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int lda.*;
\\s-*\\1 C\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldc.*;
\\s-*\\1 C_expected\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*cblas_.herk(.*);
\\s-*{
\\s-*int i;
\\s-*for.*{
\\s-*gsl_test_rel(C\\[.*\\], C_expected\\[.*], \\(flt\\|dbl\\)eps, \"\\(.*\\) real\");
\\s-*gsl_test_rel(C\\[.*], C_expected\\[.*], \\6eps, \"\\7 imag\");
\\s-*};?
\\s-*};"
    " \\1 A[]	= {\\3};
    \\1 C[]	= {\\4};
    \\1 R[]	= {\\5};
    test_herk<\\1>(\\2,A,C,R,ts,\\6eps,\"\\7\");")
    
*/
