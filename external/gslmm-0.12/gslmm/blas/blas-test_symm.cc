#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

//____________________________________________________________________
template <typename T> 
void
test_symm(int order, int uplo, int sid, 
	  size_t m, size_t n, 
	  const T& alpha,  const T& beta,
	  T* a, T* b, T* c, T* r, 
	  gslmm::test_suite& ts, 
	  const T& tol, const char* msg) 
{
  using namespace gslmm;
  
  size_t ma = (sid == left_side ? m : n);
  if (order == 102) {
    // Input is in column order mode - restructure 
    make_row_major(ma, ma, a);
    make_row_major(m, n, b);
    make_row_major(m, n, r);
    make_row_major(m, n, c);
  }
  
  matrix<T> A(a, ma, ma);
  matrix<T> B(b, m, n);
  matrix<T> C(c, m, n);
  matrix<T> R(r, m, n);

  matrix_matrix_product(alpha, 
			matrix_manip<T>(A, 
					symmetric_type,
					no_transform, 
					location(uplo), 
					non_unit_diagonal, 
					side(sid)),
			matrix_manip<T>(B),
			beta, C);
  test_result(C, R, ts, tol, msg);
}

//____________________________________________________________________
template <typename T> 
void
test_complex_symm(int order, int uplo, int sid, 
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
						  symmetric_type,
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
do_test_symm (gslmm::test_suite& ts) 
{
  const double flteps = 1e-4, dbleps = 1e-6;
  {
    float A[]	= { -0.581f };
    float B[]	= { 0.157f, 0.451f };
    float C[]	= { -0.869f, -0.871f };
    float R[]	= { 0.896365f, 0.949609f };
    test_symm<float>(101,121,141,1,2,-0.3f,-1.0f,A,B,C,R,ts,flteps,"ssymm(case 1518)");
  };


  {
    float A[]	= { 0.874f };
    float B[]	= { 0.085f, 0.069f };
    float C[]	= { -0.495f, -0.828f };
    float R[]	= { 0.472713f, 0.809908f };
    test_symm<float>(102,121,141,1,2,-0.3f,-1.0f,A,B,C,R,ts,flteps,"ssymm(case 1519)");
  };


  {
    float A[]	= { -0.671f, -0.343f, 0.6f, 0.177f };
    float B[]	= { 0.043f, 0.01f };
    float C[]	= { 0.988f, 0.478f };
    float R[]	= { 0.032283f, 0.012979f };
    test_symm<float>(101,121,142,1,2,-1.0f,0.0f,A,B,C,R,ts,flteps,"ssymm(case 1520)");
  };


  {
    float A[]	= { 0.069f, 0.096f, 0.139f, -0.044f };
    float B[]	= { -0.448f, 0.07f };
    float C[]	= { 0.361f, 0.995f };
    float R[]	= { 0.021182f, 0.065352f };
    test_symm<float>(102,121,142,1,2,-1.0f,0.0f,A,B,C,R,ts,flteps,"ssymm(case 1521)");
  };


  {
    float A[]	= { 0.745f };
    float B[]	= { -0.269f, 0.448f };
    float C[]	= { -0.986f, 0.2f };
    float R[]	= { 0.2958f, -0.06f };
    test_symm<float>(101,122,141,1,2,0.0f,-0.3f,A,B,C,R,ts,flteps,"ssymm(case 1522)");
  };


  {
    float A[]	= { 0.96f };
    float B[]	= { 0.392f, -0.07f };
    float C[]	= { -0.235f, 0.554f };
    float R[]	= { 0.0705f, -0.1662f };
    test_symm<float>(102,122,141,1,2,0.0f,-0.3f,A,B,C,R,ts,flteps,"ssymm(case 1523)");
  };


  {
    float A[]	= { -0.839f, 0.498f, -0.215f, -0.314f };
    float B[]	= { -0.66f, 0.593f };
    float C[]	= { -0.806f, 0.525f };
    float R[]	= { -0.208474f, 0.0657906f };
    test_symm<float>(101,122,142,1,2,-0.3f,0.1f,A,B,C,R,ts,flteps,"ssymm(case 1524)");
  };


  {
    float A[]	= { 0.994f, -0.117f, -0.639f, 0.925f };
    float B[]	= { -0.478f, 0.147f };
    float C[]	= { -0.814f, 0.316f };
    float R[]	= { 0.0662993f, -0.0259703f };
    test_symm<float>(102,122,142,1,2,-0.3f,0.1f,A,B,C,R,ts,flteps,"ssymm(case 1525)");
  };


  {
    double A[]	= { -0.981 };
    double B[]	= { -0.823, 0.83 };
    double C[]	= { 0.991, 0.382 };
    double R[]	= { 0.7487911, 0.626269 };
    test_symm<double>(101,121,141,1,2,-0.3,1,A,B,C,R,ts,dbleps,"dsymm(case 1526)");
  };


  {
    double A[]	= { -0.248 };
    double B[]	= { 0.74, 0.068 };
    double C[]	= { -0.905, 0.742 };
    double R[]	= { -0.849944, 0.7470592 };
    test_symm<double>(102,121,141,1,2,-0.3,1,A,B,C,R,ts,dbleps,"dsymm(case 1527)");
  };


  {
    double A[]	= { 0.591, -0.01, -0.192, -0.376 };
    double B[]	= { 0.561, 0.946 };
    double C[]	= { 0.763, 0.189 };
    double R[]	= { 0.440909, 0.550306 };
    test_symm<double>(101,121,142,1,2,-1,1,A,B,C,R,ts,dbleps,"dsymm(case 1528)");
  };


  {
    double A[]	= { -0.786, 0.87, 0.222, -0.043 };
    double B[]	= { -0.503, -0.526 };
    double C[]	= { -0.027, -0.391 };
    double R[]	= { -0.305586, -0.301952 };
    test_symm<double>(102,121,142,1,2,-1,1,A,B,C,R,ts,dbleps,"dsymm(case 1529)");
  };


  {
    double A[]	= { -0.468 };
    double B[]	= { -0.881, 0.692 };
    double C[]	= { -0.812, -0.395 };
    double R[]	= { -0.0399692, -0.0718856 };
    test_symm<double>(101,122,141,1,2,0.1,0.1,A,B,C,R,ts,dbleps,"dsymm(case 1530)");
  };


  {
    double A[]	= { 0.849 };
    double B[]	= { -0.887, 0.518 };
    double C[]	= { 0.414, -0.251 };
    double R[]	= { -0.0339063, 0.0188782 };
    test_symm<double>(102,122,141,1,2,0.1,0.1,A,B,C,R,ts,dbleps,"dsymm(case 1531)");
  };


  {
    double A[]	= { 0.457, 0.624, 0.807, 0.349 };
    double B[]	= { -0.609, 0.03 };
    double C[]	= { 0.719, -0.624 };
    double R[]	= { 0.973103, -0.143007 };
    test_symm<double>(101,122,142,1,2,-1,1,A,B,C,R,ts,dbleps,"dsymm(case 1532)");
  };


  {
    double A[]	= { -0.133, -0.117, -0.163, 0.795 };
    double B[]	= { -0.882, 0.549 };
    double C[]	= { 0.715, -0.327 };
    double R[]	= { 0.661927, -0.866649 };
    test_symm<double>(102,122,142,1,2,-1,1,A,B,C,R,ts,dbleps,"dsymm(case 1533)");
  };

#if  0
  {
   gslmm::complex<float> alpha(-1.0f, 0.0f);
   gslmm::complex<float> beta(1.0f, 0.0f);
    float A[]	= { 0.476f, 0.816f };
    float B[]	= { 0.282f, 0.852f, -0.891f, -0.588f };
    float C[]	= { 0.9f, 0.486f, -0.78f, -0.637f };
    float R[]	= { 1.461f, -0.149664f, -0.835692f, 0.369944f };
    test_complex_symm<float>(101,121,141,1,2,alpha,beta,A,B,C,R,ts,flteps,"csymm(case 1534)");
  };


  {
   gslmm::complex<float> alpha(-1.0f, 0.0f);
   gslmm::complex<float> beta(1.0f, 0.0f);
    float A[]	= { 0.048f, 0.172f };
    float B[]	= { 0.786f, 0.783f, 0.809f, -0.569f };
    float C[]	= { -0.227f, -0.215f, 0.881f, 0.233f };
    float R[]	= { -0.130052f, -0.387776f, 0.7443f, 0.121164f };
    test_complex_symm<float>(102,121,141,1,2,alpha,beta,A,B,C,R,ts,flteps,"csymm(case 1535)");
  };


  {
   gslmm::complex<float> alpha(0.0f, 1.0f);
   gslmm::complex<float> beta(0.0f, 1.0f);
    float A[]	= { -0.495f, -0.012f, 0.843f, -0.986f, -0.243f, 0.833f, 0.921f, 0.004f };
    float B[]	= { 0.876f, 0.612f, 0.805f, -0.57f };
    float C[]	= { 0.938f, -0.24f, -0.874f, -0.062f };
    float R[]	= { 1.82769f, 0.628319f, 0.93157f, 1.21158f };
    test_complex_symm<float>(101,121,142,1,2,alpha,beta,A,B,C,R,ts,flteps,"csymm(case 1536)");
  };


  {
   gslmm::complex<float> alpha(0.0f, 1.0f);
   gslmm::complex<float> beta(0.0f, 1.0f);
    float A[]	= { -0.812f, 0.83f, 0.705f, 0.15f, -0.463f, 0.901f, -0.547f, -0.483f };
    float B[]	= { -0.808f, -0.664f, 0.352f, -0.102f };
    float C[]	= { -0.64f, 0.399f, 0.896f, -0.163f };
    float R[]	= { -0.631906f, 0.496142f, 0.697798f, 1.62656f };
    test_complex_symm<float>(102,121,142,1,2,alpha,beta,A,B,C,R,ts,flteps,"csymm(case 1537)");
  };


  {
   gslmm::complex<float> alpha(-1.0f, 0.0f);
   gslmm::complex<float> beta(0.0f, 1.0f);
    float A[]	= { 0.342f, -0.906f };
    float B[]	= { 0.676f, 0.863f, -0.517f, -0.138f };
    float C[]	= { 0.274f, 0.388f, -0.271f, 0.205f };
    float R[]	= { -1.40107f, 0.59131f, 0.096842f, -0.692206f };
    test_complex_symm<float>(101,122,141,1,2,alpha,beta,A,B,C,R,ts,flteps,"csymm(case 1538)");
  };


  {
   gslmm::complex<float> alpha(-1.0f, 0.0f);
   gslmm::complex<float> beta(0.0f, 1.0f);
    float A[]	= { 0.418f, 0.354f };
    float B[]	= { -0.74f, 0.018f, 0.395f, 0.248f };
    float C[]	= { -0.162f, 0.175f, -0.853f, 0.652f };
    float R[]	= { 0.140692f, 0.092436f, -0.729318f, -1.09649f };
    test_complex_symm<float>(102,122,141,1,2,alpha,beta,A,B,C,R,ts,flteps,"csymm(case 1539)");
  };


  {
   gslmm::complex<float> alpha(-0.3f, 0.1f);
   gslmm::complex<float> beta(0.0f, 0.1f);
    float A[]	= { 0.12f, 0.496f, 0.313f, -0.136f, 0.987f, 0.532f, 0.58f, -0.687f };
    float B[]	= { -0.587f, 0.278f, 0.857f, 0.136f };
    float C[]	= { 0.162f, 0.249f, -0.665f, 0.456f };
    float R[]	= { -0.22769f, -0.0269913f, 0.0502096f, 0.0841558f };
    test_complex_symm<float>(101,122,142,1,2,alpha,beta,A,B,C,R,ts,flteps,"csymm(case 1540)");
  };


  {
   gslmm::complex<float> alpha(-0.3f, 0.1f);
   gslmm::complex<float> beta(0.0f, 0.1f);
    float A[]	= { 0.579f, -0.859f, 0.192f, -0.737f, 0.396f, -0.498f, 0.751f, -0.379f };
    float B[]	= { 0.84f, -0.755f, -0.019f, -0.063f };
    float C[]	= { 0.04f, 0.639f, -0.876f, -0.778f };
    float R[]	= { 0.115459f, 0.329813f, 0.288206f, 0.110315f };
    test_complex_symm<float>(102,122,142,1,2,alpha,beta,A,B,C,R,ts,flteps,"csymm(case 1541)");
  };
#endif

  {
   gslmm::complex<double> alpha(0., 0.);
   gslmm::complex<double> beta(0., 0.);
    double A[]	= { 0.511, -0.486 };
    double B[]	= { 0.985, -0.923, -0.234, -0.756 };
    double C[]	= { -0.16, 0.049, 0.618, -0.349 };
    double R[]	= { 0.0, 0.0, 0.0, 0.0 };
    test_complex_symm<double>(101,121,141,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsymm(case 1542)");
  };


  {
   gslmm::complex<double> alpha(0., 0.);
   gslmm::complex<double> beta(0., 0.);
    double A[]	= { 0.46, -0.816 };
    double B[]	= { 0.404, 0.113, -0.904, -0.627 };
    double C[]	= { 0.114, 0.318, 0.636, -0.839 };
    double R[]	= { 0.0, 0.0, 0.0, 0.0 };
    test_complex_symm<double>(102,121,141,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsymm(case 1543)");
  };


  {
   gslmm::complex<double> alpha(-1., 0.);
   gslmm::complex<double> beta(-0.3, 0.1);
    double A[]	= { -0.835, 0.344, 0.975, 0.634, 0.312, -0.659, -0.624, -0.175 };
    double B[]	= { -0.707, -0.846, 0.825, -0.661 };
    double C[]	= { 0.352, -0.499, 0.267, 0.548 };
    double R[]	= { -2.160518, -0.156877, 0.648536, 0.867299 };
    test_complex_symm<double>(101,121,142,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsymm(case 1544)");
  };


  {
   gslmm::complex<double> alpha(-1., 0.);
   gslmm::complex<double> beta(-0.3, 0.1);
    double A[]	= { -0.409, 0.013, -0.308, -0.317, -0.535, -0.697, -0.385, 0.119 };
    double B[]	= { 0.299, -0.233, 0.093, 0.664 };
    double C[]	= { 0.699, 0.47, -0.347, -0.182 };
    double R[]	= { -0.550491, 0.249777, 0.559487, 0.348221 };
    test_complex_symm<double>(102,121,142,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsymm(case 1545)");
  };


  {
   gslmm::complex<double> alpha(1., 0.);
   gslmm::complex<double> beta(0., 1.);
    double A[]	= { -0.151, 0.635 };
    double B[]	= { 0.711, -0.869, 0.153, 0.647 };
    double C[]	= { -0.299, 0.43, -0.307, 0.133 };
    double R[]	= { 0.014454, 0.283704, -0.566948, -0.307542 };
    test_complex_symm<double>(101,122,141,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsymm(case 1546)");
  };


  {
   gslmm::complex<double> alpha(1., 0.);
   gslmm::complex<double> beta(0., 1.);
    double A[]	= { 0.793, -0.543 };
    double B[]	= { 0.054, -0.045, 0.989, 0.453 };
    double C[]	= { 0.443, -0.641, -0.809, -0.83 };
    double R[]	= { 0.659387, 0.377993, 1.860256, -0.986798 };
    test_complex_symm<double>(102,122,141,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsymm(case 1547)");
  };


  {
   gslmm::complex<double> alpha(1., 0.);
   gslmm::complex<double> beta(-1., 0.);
    double A[]	= { -0.432, -0.293, -0.819, 0.44, -0.818, -0.258, -0.836, 0.683 };
    double B[]	= { -0.259, -0.878, 0.161, 0.744 };
    double C[]	= { 0.436, -0.655, -0.61, -0.875 };
    double R[]	= { -0.521112, 0.460053, -0.04741, 1.148005 };
    test_complex_symm<double>(101,122,142,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsymm(case 1548)");
  };


  {
   gslmm::complex<double> alpha(1., 0.);
   gslmm::complex<double> beta(-1., 0.);
    double A[]	= { -0.656, 0.378, -0.688, 0.676, 0.967, -0.804, 0.455, -0.425 };
    double B[]	= { 0.791, -0.947, -0.945, -0.444 };
    double C[]	= { 0.014, -0.814, -0.091, -0.417 };
    double R[]	= { 0.775374, 1.400882, -0.431711, 1.802857 };
    test_complex_symm<double>(102,122,142,1,2,alpha,beta,A,B,C,R,ts,dbleps,"zsymm(case 1549)");
  };


}
/*
   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int side\\s-*=\\s-*\\(.*\\);
   int uplo\\s-*=\\s-*\\(.*\\);
   int M\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);"
  "test_symm(\\1,\\3,\\2,\\4,\\5);")

   (query-replace-regexp 
  "test_symm(\\(.*\\));
   \\(float\\|double\\) alpha\\s-*=\\s-*\\(.*\\);
   \\2 beta\\s-*=\\s-*\\(.*\\);"
  "test_symm<\\2>(\\1,\\3,\\4);")

   (query-replace-regexp 
  "test_symm(\\(.*\\));
   \\(float\\|double\\) alpha\\[2\\]\\s-*=\\s-*{\\(.*\\)};
   \\2 beta\\[2\\]\\s-*=\\s-*{\\(.*\\)};"
  "gslmm::complex<\\2> alpha(\\3);
   gslmm::complex<\\2> beta(\\4);
   test_complex_symm<\\2>(\\1,alpha,beta);")

   (query-replace-regexp 
   "test_\\(complex_\\|\\)symm<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\2 A\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int lda.*;
\\s-*\\2 B\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldb.*;
\\s-*\\2 C\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldc.*;
\\s-*\\2 C_expected\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*cblas_.symm(.*);
\\s-*{
\\s-*int i;
\\s-*for.*{
\\s-*gsl_test_rel(C\\[i\\], C_expected\\[i\\], \\(flt\\|dbl\\)eps, \"\\(.*\\)\");
\\s-*}
\\s-*};"
    " \\2 A[]	= {\\4};
    \\2 B[]	= {\\5};
    \\2 C[]	= {\\6};
    \\2 R[]	= {\\7};
    test_\\1symm<\\2>(\\3,A,B,C,R,\\8eps,\"\\9\");")


   (query-replace-regexp 
   "test_\\(complex_\\|\\)symm<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\2 A\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int lda.*;
\\s-*\\2 B\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldb.*;
\\s-*\\2 C\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldc.*;
\\s-*\\2 C_expected\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*cblas_.symm(.*);
\\s-*{
\\s-*int i;
\\s-*for.*{
\\s-*gsl_test_rel(C\\[2\\*i\\], C_expected\\[2\\*i\\], \\(flt\\|dbl\\)eps, \"\\(.*\\) real\");
\\s-*gsl_test_rel(C\\[2\\*i\\+1\\], C_expected\\[2\\*i\\+1\\], \\8eps, \"\\9 imag\");
\\s-*};?
\\s-*};"
    " \\2 A[]	= {\\4};
    \\2 B[]	= {\\5};
    \\2 C[]	= {\\6};
    \\2 R[]	= {\\7};
    test_\\1gemm<\\2>(\\3,A,B,C,R,\\8eps,\"\\9\");")
    
*/
