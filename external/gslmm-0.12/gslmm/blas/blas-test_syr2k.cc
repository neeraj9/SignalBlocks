#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif
    
//____________________________________________________________________
template <typename T> 
void
test_syr2k(int order, int trans, int uplo,
	  size_t m, size_t n, 
	  const T& alpha, 
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
    make_row_major(ma, na, a);
    make_row_major(ma, na, b);
    make_row_major(m, m, c);
    make_row_major(m, m, r);
  }
  matrix<T> A(a, ma, na);
  matrix<T> B(b, ma, na);
  matrix<T> C(c, m,  m);
  matrix<T> R(r, m,  m);

  rank_2k_update(alpha, matrix_manip<T>(A, general_type, transform(trans)),
		 matrix_manip<T>(B), beta, 
		 matrix_manip<T>(C, symmetric_type, no_transform, 
				 location(uplo)));
  test_result(C, R, ts, tol, msg);
}

    
//____________________________________________________________________
template <typename T> 
void
test_complex_syr2k(int order, int trans, int uplo,
	  size_t m, size_t n, 
	  const gslmm::complex<T>& alpha, 
	  const gslmm::complex<T>& beta, 
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

  rank_2k_update(alpha, 
		 matrix_manip<complex<T> >(A, 
					   general_type,
					   transform(trans)),
		 matrix_manip<complex<T> >(B), 
		 beta,
		 matrix_manip<complex<T> >(C, symmetric_type,
					   no_transform, 
					   location(uplo)));
  test_complex_result(C, R, ts, tol, msg);
}

//____________________________________________________________________
void
do_test_syr2k (gslmm::test_suite& ts) 
{
  const double flteps = 1e-4, dbleps = 1e-6;
  {
    float A[]	 = { -0.915f, 0.445f };
    float B[]     = { 0.213f, -0.194f };
    float C[]	= { -0.117f };
    float R[]	= { -0.173245f };
    test_syr2k<float>(101,111,121,1,2,0.1f,1.0f,A,B,C,R,ts,flteps,"ssyr2k(case 1614)");
  };


  {
    float A[]	 = { 0.089f, -0.889f };
    float B[]     = { -0.384f, 0.518f };
    float C[]	= { 0.069f };
    float R[]	= { -0.0299356f };
    test_syr2k<float>(101,111,122,1,2,0.1f,1.0f,A,B,C,R,ts,flteps,"ssyr2k(case 1615)");
  };


  {
    float A[]	 = { 0.492f, 0.021f };
    float B[]     = { -0.804f, -0.912f };
    float C[]	= { -0.851f };
    float R[]	= { -0.933944f };
    test_syr2k<float>(102,111,121,1,2,0.1f,1.0f,A,B,C,R,ts,flteps,"ssyr2k(case 1616)");
  };


  {
    float A[]	 = { -0.376f, 0.689f };
    float B[]     = { 0.21f, 0.406f };
    float C[]	= { -0.581f };
    float R[]	= { -0.540845f };
    test_syr2k<float>(102,111,122,1,2,0.1f,1.0f,A,B,C,R,ts,flteps,"ssyr2k(case 1617)");
  };


  {
    float A[]	 = { 0.629f, -0.883f };
    float B[]     = { -0.165f, 0.02f };
    float C[]	= { 0.236f };
    float R[]	= { -0.31369f };
    test_syr2k<float>(101,112,121,1,2,1.0f,-0.3f,A,B,C,R,ts,flteps,"ssyr2k(case 1618)");
  };


  {
    float A[]	 = { 0.412f, -0.411f };
    float B[]     = { 0.313f, 0.301f };
    float C[]	= { 0.222f };
    float R[]	= { -0.05611f };
    test_syr2k<float>(101,112,122,1,2,1.0f,-0.3f,A,B,C,R,ts,flteps,"ssyr2k(case 1619)");
  };


  {
    float A[]	 = { -0.02f, 0.593f };
    float B[]     = { -0.144f, 0.846f };
    float C[]	= { -0.645f };
    float R[]	= { 1.20262f };
    test_syr2k<float>(102,112,121,1,2,1.0f,-0.3f,A,B,C,R,ts,flteps,"ssyr2k(case 1620)");
  };


  {
    float A[]	 = { 0.253f, 0.937f };
    float B[]     = { 0.24f, -0.27f };
    float C[]	= { 0.128f };
    float R[]	= { -0.42294f };
    test_syr2k<float>(102,112,122,1,2,1.0f,-0.3f,A,B,C,R,ts,flteps,"ssyr2k(case 1621)");
  };


  {
    double A[]	 = { -0.225, 0.857 };
    double B[]     = { -0.933, 0.994 };
    double C[]	= { 0.177 };
    double R[]	= { 0.2123566 };
    test_syr2k<double>(101,111,121,1,2,0.1,0,A,B,C,R,ts,dbleps,"dsyr2k(case 1622)");
  };


  {
    double A[]	 = { -0.955, 0.112 };
    double B[]     = { -0.695, 0.719 };
    double C[]	= { 0.069 };
    double R[]	= { 0.1488506 };
    test_syr2k<double>(101,111,122,1,2,0.1,0,A,B,C,R,ts,dbleps,"dsyr2k(case 1623)");
  };


  {
    double A[]	 = { 0.216, 0.911 };
    double B[]     = { -0.074, -0.256 };
    double C[]	= { -0.621 };
    double R[]	= { -0.04984 };
    test_syr2k<double>(102,111,121,1,2,0.1,0,A,B,C,R,ts,dbleps,"dsyr2k(case 1624)");
  };


  {
    double A[]	 = { -0.343, -0.381 };
    double B[]     = { -0.433, -0.087 };
    double C[]	= { -0.889 };
    double R[]	= { 0.0363332 };
    test_syr2k<double>(102,111,122,1,2,0.1,0,A,B,C,R,ts,dbleps,"dsyr2k(case 1625)");
  };


  {
    double A[]	 = { -0.633, 0.219 };
    double B[]     = { 0.817, -0.683 };
    double C[]	= { -0.294 };
    double R[]	= { -1.039476 };
    test_syr2k<double>(101,112,121,1,2,1,-1,A,B,C,R,ts,dbleps,"dsyr2k(case 1626)");
  };


  {
    double A[]	 = { -0.887, -0.43 };
    double B[]     = { 0.557, 0.912 };
    double C[]	= { 0.831 };
    double R[]	= { -2.603438 };
    test_syr2k<double>(101,112,122,1,2,1,-1,A,B,C,R,ts,dbleps,"dsyr2k(case 1627)");
  };


  {
    double A[]	 = { 0.397, -0.173 };
    double B[]     = { 0.155, -0.99 };
    double C[]	= { 0.621 };
    double R[]	= { -0.15539 };
    test_syr2k<double>(102,112,121,1,2,1,-1,A,B,C,R,ts,dbleps,"dsyr2k(case 1628)");
  };


  {
    double A[]	 = { 0.833, -0.52 };
    double B[]     = { 0.28, 0.481 };
    double C[]	= { 0.455 };
    double R[]	= { -0.48876 };
    test_syr2k<double>(102,112,122,1,2,1,-1,A,B,C,R,ts,dbleps,"dsyr2k(case 1629)");
  };

#if 0
  {
    gslmm::complex<float> alpha(0.0f, 0.1f);
    gslmm::complex<float> beta(0.0f, 0.0f);
    float A[]	=  { -0.248f, -0.037f, -0.124f, 0.998f };
    float B[]	=  { -0.608f, -0.115f, -0.718f, -0.551f };
    float C[]	=  { 0.187f, -0.329f };
    float R[]	=  { 0.119445f, 0.157092f };
    test_complex_syr2k<float>(101,111,121,1,2,alpha,beta,A,B,C,R,ts,flteps,"csyr2k(case 1630)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.1f);
    gslmm::complex<float> beta(0.0f, 0.0f);
    float A[]	=  { 0.068f, 0.751f, -0.449f, -0.598f };
    float B[]	=  { 0.616f, 0.805f, -0.635f, 0.773f };
    float C[]	=  { -0.287f, 0.917f };
    float R[]	=  { -0.110002f, 0.0369404f };
    test_complex_syr2k<float>(101,111,122,1,2,alpha,beta,A,B,C,R,ts,flteps,"csyr2k(case 1631)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.1f);
    gslmm::complex<float> beta(0.0f, 0.0f);
    float A[]	=  { -0.396f, -0.603f, -0.131f, -0.288f };
    float B[]	=  { -0.64f, -0.444f, -0.085f, 0.936f };
    float C[]	=  { 0.375f, -0.434f };
    float R[]	=  { -0.0927216f, 0.0532822f };
    test_complex_syr2k<float>(102,111,121,1,2,alpha,beta,A,B,C,R,ts,flteps,"csyr2k(case 1632)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.1f);
    gslmm::complex<float> beta(0.0f, 0.0f);
    float A[]	=  { 0.655f, 0.16f, 0.45f, -0.747f };
    float B[]	=  { 0.923f, 0.432f, -0.986f, 0.259f };
    float C[]	=  { 0.752f, 0.576f };
    float R[]	=  { -0.256746f, 0.0570436f };
    test_complex_syr2k<float>(102,111,122,1,2,alpha,beta,A,B,C,R,ts,flteps,"csyr2k(case 1633)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.1f);
    gslmm::complex<float> beta(0.0f, 0.0f);
    float A[]	=  { -0.765f, 0.487f, 0.7f, 0.768f };
    float B[]	=  { -0.529f, 0.056f, -0.584f, 0.928f };
    float C[]	=  { -0.426f, 0.836f };
    float R[]	=  { 0.019875f, -0.148818f };
    test_complex_syr2k<float>(101,112,121,1,2,alpha,beta,A,B,C,R,ts,flteps,"csyr2k(case 1634)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.1f);
    gslmm::complex<float> beta(0.0f, 0.0f);
    float A[]	=  { 0.25f, 0.489f, 0.8f, -0.642f };
    float B[]	=  { -0.732f, -0.856f, -0.654f, 0.591f };
    float C[]	=  { -0.101f, 0.322f };
    float R[]	=  { -0.064144f, 0.0183612f };
    test_complex_syr2k<float>(101,112,122,1,2,alpha,beta,A,B,C,R,ts,flteps,"csyr2k(case 1635)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.1f);
    gslmm::complex<float> beta(0.0f, 0.0f);
    float A[]	=  { -0.579f, -0.971f, 0.521f, -0.824f };
    float B[]	=  { -0.227f, 0.907f, 0.457f, -0.274f };
    float C[]	=  { 0.21f, -0.718f };
    float R[]	=  { 0.164812f, 0.20489f };
    test_complex_syr2k<float>(102,112,121,1,2,alpha,beta,A,B,C,R,ts,flteps,"csyr2k(case 1636)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.1f);
    gslmm::complex<float> beta(0.0f, 0.0f);
    float A[]	=  { -0.83f, -0.512f, -0.667f, -0.436f };
    float B[]	=  { -0.443f, 0.82f, -0.259f, -0.618f };
    float C[]	=  { 0.583f, 0.668f };
    float R[]	=  { -0.0142692f, 0.138167f };
    test_complex_syr2k<float>(102,112,122,1,2,alpha,beta,A,B,C,R,ts,flteps,"csyr2k(case 1637)");
  };
#endif

  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(-0.3, 0.1);
    double A[]	=  { -0.315, 0.03, 0.281, 0.175 };
    double B[]	=  { -0.832, -0.964, 0.291, 0.476 };
    double C[]	=  { -0.341, 0.743 };
    double R[]	=  { 0.028, -0.257 };
    test_complex_syr2k<double>(101,111,121,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsyr2k(case 1638)");
  };


  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(-0.3, 0.1);
    double A[]	=  { -0.159, -0.489, -0.11, 0.611 };
    double B[]	=  { -0.285, -0.048, -0.673, -0.492 };
    double C[]	=  { 0.496, -0.626 };
    double R[]	=  { -0.0862, 0.2374 };
    test_complex_syr2k<double>(101,111,122,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsyr2k(case 1639)");
  };


  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(-0.3, 0.1);
    double A[]	=  { 0.796, 0.872, -0.919, 0.748 };
    double B[]	=  { -0.945, 0.915, -0.252, -0.276 };
    double C[]	=  { 0.07, -0.957 };
    double R[]	=  { 0.0747, 0.2941 };
    test_complex_syr2k<double>(102,111,121,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsyr2k(case 1640)");
  };


  {
    gslmm::complex<double> alpha(0., 0.);
    gslmm::complex<double> beta(-0.3, 0.1);
    double A[]	=  { 0.984, 0.526, 0.284, 0.806 };
    double B[]	=  { -0.509, -0.178, 0.188, -0.221 };
    double C[]	=  { -0.388, 0.795 };
    double R[]	=  { 0.0369, -0.2773 };
    test_complex_syr2k<double>(102,111,122,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsyr2k(case 1641)");
  };


  {
    gslmm::complex<double> alpha(1., 0.);
    gslmm::complex<double> beta(0., 0.1);
    double A[]	=  { 0.628, 0.846, -0.645, 0.032 };
    double B[]	=  { 0.545, -0.54, 0.493, -0.035 };
    double C[]	=  { -0.16, -0.06 };
    double R[]	=  { 0.97047, 0.304602 };
    test_complex_syr2k<double>(101,112,121,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsyr2k(case 1642)");
  };


  {
    gslmm::complex<double> alpha(1., 0.);
    gslmm::complex<double> beta(0., 0.1);
    double A[]	=  { -0.556, -0.946, 0.177, -0.859 };
    double B[]	=  { 0.423, -0.91, 0.736, -0.251 };
    double C[]	=  { -0.478, 0.519 };
    double R[]	=  { -2.41467, -1.189498 };
    test_complex_syr2k<double>(101,112,122,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsyr2k(case 1643)");
  };


  {
    gslmm::complex<double> alpha(1., 0.);
    gslmm::complex<double> beta(0., 0.1);
    double A[]	=  { -0.582, 0.09, -0.176, 0.784 };
    double B[]	=  { 0.687, -0.859, 0.945, 0.756 };
    double C[]	=  { -0.663, -0.186 };
    double R[]	=  { -2.144496, 2.272884 };
    test_complex_syr2k<double>(102,112,121,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsyr2k(case 1644)");
  };


  {
    gslmm::complex<double> alpha(1., 0.);
    gslmm::complex<double> beta(0., 0.1);
    double A[]	=  { 0.231, -0.452, -0.112, -0.837 };
    double B[]	=  { -0.258, 0.464, -0.224, 0.893 };
    double C[]	=  { -0.448, 0.046 };
    double R[]	=  { 1.840718, 0.577744 };
    test_complex_syr2k<double>(102,112,122,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsyr2k(case 1645)");
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
  " test_syr2k<\\6>(\\1,\\3,\\2,\\4,\\5,\\7,\\8);")

   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int uplo\\s-*=\\s-*\\(.*\\);
   int trans\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);
   int K\\s-*=\\s-*\\(.*\\);
   \\(float\\|double\\) alpha\\[2\\]\\s-*=\\s-*\\(.*\\);
   \\6 beta\\[2\\]\\s-*=\\s-*\\(.*\\);"
  " gslmm::complex<\\6> alpha(\\7);
    test_complex_syr2k<\\6>(\\1,\\3,\\2,\\4,\\5,alpha,\\8);")

   (query-replace-regexp 
   "test_\\(complex_\\|\\)syr2k<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\2 A\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int lda.*;
\\s-*\\2 B\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldb.*;
\\s-*\\2 C\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldc.*;
\\s-*\\2 C_expected\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*cblas_.syr2k(.*);
\\s-*{
\\s-*int i;
\\s-*for.*{
\\s-*gsl_test_rel(C\\[.*\\], C_expected\\[.*], \\(flt\\|dbl\\)eps, \"\\(.*)\\).*\");[^}]*
\\s-*};?
\\s-*};"
    "\\2 A[]	 = {\\4};
    \\2 B[]     = {\\5};
    \\2 C[]	= {\\6};
    \\2 R[]	= {\\7};
    test_\\1syr2k<\\2>(\\3,A,B,C,R,ts,\\8eps,\"\\9\");")
    
*/
