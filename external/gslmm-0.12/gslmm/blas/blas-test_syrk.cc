#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

    
//____________________________________________________________________
template <typename T> 
void
test_syrk(int order, int trans, int uplo,
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
    make_row_major(ma, na, a);
    make_row_major(m, m, b);
    make_row_major(m, m, r);
  }
  matrix<T> A(a, ma, na);
  matrix<T> B(b, m,  m);
  matrix<T> R(r, m,  m);

  rank_k_update(alpha, matrix_manip<T>(A, general_type, transform(trans)),
		beta, matrix_manip<T>(B, symmetric_type, no_transform, 
				      location(uplo)));
  test_result(B, R, ts, tol, msg);
}

//____________________________________________________________________
template <typename T> 
void
test_complex_syrk(int order, int trans, int uplo,
		  size_t m, size_t n, 
		  const gslmm::complex<T>& alpha, 
		  const gslmm::complex<T>& beta, 
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

  rank_k_update(alpha, 
		matrix_manip<complex<T> >(A, general_type, transform(trans)),
		beta, 
		matrix_manip<complex<T> >(B, symmetric_type, no_transform,
					  location(uplo)));
  test_complex_result(B, R, ts, tol, msg);
}

//____________________________________________________________________
void
do_test_syrk (gslmm::test_suite& ts) 
{
  const double flteps = 1e-4, dbleps = 1e-6;
  {
    float A[]	= { 0.412f, -0.229f };
    float C[]	= { 0.628f, -0.664f, -0.268f, 0.096f };
    float R[]	= { -0.106944f, 0.027948f, -0.268f, -0.042841f };
    test_syrk<float>(101,111,121,2,1,-1.0f,0.1f,A,C,R,ts,flteps,"ssyrk(case 1566)");
  };


  {
    float A[]	= { 0.101f, -0.653f };
    float C[]	= { 0.432f, 0.107f, -0.952f, -0.532f };
    float R[]	= { 0.032999f, 0.107f, -0.029247f, -0.479609f };
    test_syrk<float>(102,111,121,2,1,-1.0f,0.1f,A,C,R,ts,flteps,"ssyrk(case 1567)");
  };


  {
    float A[]	= { 0.79f, 0.595f };
    float C[]	= { 0.257f, 0.183f, -0.021f, -0.053f };
    float R[]	= { 0.6498f, 0.48835f, -0.021f, 0.348725f };
    test_syrk<float>(101,112,121,2,1,1.0f,0.1f,A,C,R,ts,flteps,"ssyrk(case 1568)");
  };


  {
    float A[]	= { -0.181f, -0.654f };
    float C[]	= { -0.4f, 0.615f, 0.147f, -0.163f };
    float R[]	= { -0.007239f, 0.615f, 0.133074f, 0.411416f };
    test_syrk<float>(102,112,121,2,1,1.0f,0.1f,A,C,R,ts,flteps,"ssyrk(case 1569)");
  };


  {
    float A[]	= { -0.191f, 0.584f };
    float C[]	= { -0.719f, -0.681f, -0.003f, 0.544f };
    float R[]	= { 0.719f, -0.681f, 0.003f, -0.544f };
    test_syrk<float>(101,111,122,2,1,0.0f,-1.0f,A,C,R,ts,flteps,"ssyrk(case 1570)");
  };


  {
    float A[]	= { 0.788f, 0.041f };
    float C[]	= { 0.029f, 0.365f, 0.739f, -0.769f };
    float R[]	= { -0.029f, -0.365f, 0.739f, 0.769f };
    test_syrk<float>(102,111,122,2,1,0.0f,-1.0f,A,C,R,ts,flteps,"ssyrk(case 1571)");
  };


  {
    float A[]	= { 0.733f, 0.678f };
    float C[]	= { -0.941f, 0.96f, 0.07f, -0.295f };
    float R[]	= { 0.779813f, 0.96f, -0.219092f, 0.157095f };
    test_syrk<float>(101,112,122,2,1,-0.3f,-1.0f,A,C,R,ts,flteps,"ssyrk(case 1572)");
  };


  {
    float A[]	= { -0.87f, 0.675f };
    float C[]	= { -0.602f, -0.432f, -0.984f, 0.384f };
    float R[]	= { 0.37493f, 0.608175f, -0.984f, -0.520687f };
    test_syrk<float>(102,112,122,2,1,-0.3f,-1.0f,A,C,R,ts,flteps,"ssyrk(case 1573)");
  };


  {
    double A[]	= { 0.169, -0.875 };
    double C[]	= { 0.159, 0.277, 0.865, 0.346 };
    double R[]	= { -0.0448439, -0.0978875, 0.865, -0.0272375 };
    test_syrk<double>(101,111,121,2,1,0.1,-0.3,A,C,R,ts,dbleps,"dsyrk(case 1574)");
  };


  {
    double A[]	= { 0.536, -0.725 };
    double C[]	= { 0.154, -0.445, -0.841, -0.91 };
    double R[]	= { -0.0174704, -0.445, 0.21344, 0.3255625 };
    test_syrk<double>(102,111,121,2,1,0.1,-0.3,A,C,R,ts,dbleps,"dsyrk(case 1575)");
  };


  {
    double A[]	= { -0.07, 0.8 };
    double C[]	= { 0.823, -0.88, -0.136, 0.793 };
    double R[]	= { -0.823, 0.88, -0.136, -0.793 };
    test_syrk<double>(101,112,121,2,1,0,-1,A,C,R,ts,dbleps,"dsyrk(case 1576)");
  };


  {
    double A[]	= { -0.058, 0.649 };
    double C[]	= { -0.187, 0.294, -0.004, -0.933 };
    double R[]	= { 0.187, 0.294, 0.004, 0.933 };
    test_syrk<double>(102,112,121,2,1,0,-1,A,C,R,ts,dbleps,"dsyrk(case 1577)");
  };


  {
    double A[]	= { 0.263, -0.289 };
    double C[]	= { 0.554, -0.679, 0.993, 0.758 };
    double R[]	= { -0.484831, -0.679, -1.069007, -0.674479 };
    test_syrk<double>(101,111,122,2,1,1,-1,A,C,R,ts,dbleps,"dsyrk(case 1578)");
  };


  {
    double A[]	= { -0.265, -0.837 };
    double C[]	= { -0.994, 0.967, -0.34, -0.069 };
    double R[]	= { 1.064225, -0.745195, -0.34, 0.769569 };
    test_syrk<double>(102,111,122,2,1,1,-1,A,C,R,ts,dbleps,"dsyrk(case 1579)");
  };


  {
    double A[]	= { -0.464, 0.394 };
    double C[]	= { -0.45, -0.447, 0.649, 0.055 };
    double R[]	= { -0.5145888, -0.447, 0.7038448, 0.0084292 };
    test_syrk<double>(101,112,122,2,1,-0.3,1,A,C,R,ts,dbleps,"dsyrk(case 1580)");
  };


  {
    double A[]	= { 0.815, 0.168 };
    double C[]	= { 0.817, -0.957, -0.395, -0.382 };
    double R[]	= { 0.6177325, -0.998076, -0.395, -0.3904672 };
    test_syrk<double>(102,112,122,2,1,-0.3,1,A,C,R,ts,dbleps,"dsyrk(case 1581)");
  };

#if 0
  {
    gslmm::complex<float> alpha(0.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[]	= { 0.447f, -0.507f, -0.425f, 0.701f };
    float C[]	= { 0.16f, -0.245f, 0.922f, -0.437f, 0.24f, 0.008f, -0.095f, 0.749f };
    float R[]	= { -0.0235f, 0.0895f, -0.2329f, 0.2233f, 0.24f, 0.008f, -0.0464f, -0.2342f };
    test_complex_syrk<float>(101,111,121,2,1,alpha,beta,A,C,R,ts,flteps,"csyrk(case 1582)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[]	= { -0.421f, -0.435f, -0.914f, -0.493f };
    float C[]	= { -0.761f, -0.38f, 0.043f, -0.999f, 0.779f, 0.238f, 0.082f, 0.394f };
    float R[]	= { 0.2663f, 0.0379f, 0.043f, -0.999f, -0.2575f, 0.0065f, -0.064f, -0.11f };
    test_complex_syrk<float>(102,111,121,2,1,alpha,beta,A,C,R,ts,flteps,"csyrk(case 1583)");
  };


  {
    gslmm::complex<float> alpha(-1.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[]	= { 0.827f, -0.896f, 0.417f, 0.865f };
    float C[]	= { -0.349f, -0.31f, 0.972f, 0.794f, -0.906f, -0.595f, -0.089f, -0.333f };
    float R[]	= { 0.254587f, 1.54008f, -1.4909f, -0.482723f, -0.906f, -0.595f, 0.634336f, -0.63041f };
    test_complex_syrk<float>(101,112,121,2,1,alpha,beta,A,C,R,ts,flteps,"csyrk(case 1584)");
  };


  {
    gslmm::complex<float> alpha(-1.0f, 0.0f);
    gslmm::complex<float> beta(-0.3f, 0.1f);
    float A[]	= { 0.607f, 0.747f, -0.889f, 0.333f };
    float C[]	= { 0.244f, 0.564f, 0.009f, 0.578f, -0.827f, 0.558f, -0.337f, 0.731f };
    float R[]	= { 0.05996f, -1.05166f, 0.009f, 0.578f, 0.980674f, 0.211852f, -0.651432f, 0.339074f };
    test_complex_syrk<float>(102,112,121,2,1,alpha,beta,A,C,R,ts,flteps,"csyrk(case 1585)");
  };


  {
    gslmm::complex<float> alpha(1.0f, 0.0f);
    gslmm::complex<float> beta(0.0f, 1.0f);
    float A[]	= { 0.784f, -0.281f, -0.88f, 0.479f };
    float C[]	= { 0.491f, 0.531f, 0.805f, -0.097f, 0.728f, 0.674f, -0.705f, -0.754f };
    float R[]	= { 0.004695f, 0.050392f, 0.805f, -0.097f, -1.22932f, 1.35082f, 1.29896f, -1.54804f };
    test_complex_syrk<float>(101,111,122,2,1,alpha,beta,A,C,R,ts,flteps,"csyrk(case 1586)");
  };


  {
    gslmm::complex<float> alpha(1.0f, 0.0f);
    gslmm::complex<float> beta(0.0f, 1.0f);
    float A[]	= { 0.272f, -0.146f, 0.155f, 0.038f };
    float C[]	= { 0.533f, -0.41f, -0.904f, 0.301f, -0.836f, 0.57f, -0.374f, -0.293f };
    float R[]	= { 0.462668f, 0.453576f, -0.253292f, -0.916294f, -0.836f, 0.57f, 0.315581f, -0.36222f };
    test_complex_syrk<float>(102,111,122,2,1,alpha,beta,A,C,R,ts,flteps,"csyrk(case 1587)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 1.0f);
    gslmm::complex<float> beta(-1.0f, 0.0f);
    float A[]	= { -0.055f, -0.127f, -0.896f, -0.625f };
    float C[]	= { -0.619f, 0.511f, -0.877f, 0.557f, -0.801f, -0.437f, -0.922f, 0.332f };
    float R[]	= { 0.60503f, -0.524104f, -0.877f, 0.557f, 0.652833f, 0.406905f, -0.198f, 0.080191f };
    test_complex_syrk<float>(101,112,122,2,1,alpha,beta,A,C,R,ts,flteps,"csyrk(case 1588)");
  };


  {
    gslmm::complex<float> alpha(0.0f, 1.0f);
    gslmm::complex<float> beta(-1.0f, 0.0f);
    float A[]	= { -0.528f, 0.759f, -0.079f, 0.952f };
    float C[]	= { 0.775f, 0.855f, 0.786f, 0.525f, 0.85f, 0.044f, 0.658f, 0.947f };
    float R[]	= { 0.026504f, -1.1523f, -0.223383f, -1.20586f, 0.85f, 0.044f, -0.507584f, -1.84706f };
    test_complex_syrk<float>(102,112,122,2,1,alpha,beta,A,C,R,ts,flteps,"csyrk(case 1589)");
  };
#endif

  {
    gslmm::complex<double> alpha(1., 0.);
    gslmm::complex<double> beta(1., 0.);
    double A[]	= { -0.049, -0.687, -0.434, 0.294 };
    double C[]	= { 0.937, -0.113, 0.796, 0.293, 0.876, -0.199, -0.757, -0.103 };
    double R[]	= { 0.467432, -0.045674, 1.019244, 0.576752, 0.876, -0.199, -0.65508, -0.358192 };
    test_complex_syrk<double>(101,111,121,2,1,alpha,beta,A,C,R,ts,dbleps,"zsyrk(case 1590)");
  };


  {
    gslmm::complex<double> alpha(1., 0.);
    gslmm::complex<double> beta(1., 0.);
    double A[]	= { 0.359, -0.364, 0.926, -0.69 };
    double C[]	= { 0.306, 0.249, 0.28, 0.229, 0.866, 0.092, 0.886, -0.283 };
    double R[]	= { 0.302385, -0.012352, 0.28, 0.229, 0.947274, -0.492774, 1.267376, -1.56088 };
    test_complex_syrk<double>(102,111,121,2,1,alpha,beta,A,C,R,ts,dbleps,"zsyrk(case 1591)");
  };


  {
    gslmm::complex<double> alpha(-0.3, 0.1);
    gslmm::complex<double> beta(0., 0.);
    double A[]	= { 0.607, 0.555, -0.85, 0.831 };
    double C[]	= { 0.069, 0.368, 0.551, -0.912, -0.243, -0.063, -0.924, 0.192 };
    double R[]	= { -0.0855042, -0.1960886, 0.2898798, -0.1075156, -0.243, -0.063, 0.1316883, 0.4270039 };
    test_complex_syrk<double>(101,112,121,2,1,alpha,beta,A,C,R,ts,dbleps,"zsyrk(case 1592)");
  };


  {
    gslmm::complex<double> alpha(-0.3, 0.1);
    gslmm::complex<double> beta(0., 0.);
    double A[]	= { 0.427, 0.86, -0.136, 0.002 };
    double C[]	= { 0.398, -0.47, 0.011, -0.547, -0.106, 0.016, 0.681, 0.246 };
    double R[]	= { 0.0937373, -0.2760591, 0.011, -0.547, 0.0295482, 0.0288526, -0.0054932, 0.0020124 };
    test_complex_syrk<double>(102,112,121,2,1,alpha,beta,A,C,R,ts,dbleps,"zsyrk(case 1593)");
  };


  {
    gslmm::complex<double> alpha(-0.3, 0.1);
    gslmm::complex<double> beta(1., 0.);
    double A[]	= { 0.718, 0.023, 0.355, -0.492 };
    double C[]	= { -0.637, -0.727, -0.475, -0.776, 0.802, -0.55, -0.837, 0.222 };
    double R[]	= { -0.7948013, -0.6854089, -0.475, -0.776, 0.7566473, -0.4198521, -0.7672563, 0.3151921 };
    test_complex_syrk<double>(101,111,122,2,1,alpha,beta,A,C,R,ts,dbleps,"zsyrk(case 1594)");
  };


  {
    gslmm::complex<double> alpha(-0.3, 0.1);
    gslmm::complex<double> beta(1., 0.);
    double A[]	= { 0.209, 0.139, -0.202, -0.223 };
    double C[]	= { -0.695, 0.524, 0.212, -0.88, -0.752, 0.291, 0.684, -0.124 };
    double R[]	= { -0.7081182, 0.5090054, 0.2228348, -0.8587166, -0.752, 0.291, 0.6776683, -0.1519201 };
    test_complex_syrk<double>(102,111,122,2,1,alpha,beta,A,C,R,ts,dbleps,"zsyrk(case 1595)");
  };


  {
    gslmm::complex<double> alpha(-0.3, 0.1);
    gslmm::complex<double> beta(1., 0.);
    double A[]	= { -0.365, -0.624, 0.632, 0.348 };
    double C[]	= { 0.877, 0.927, -0.377, 0.967, 0.008, 0.292, -0.779, 0.794 };
    double R[]	= { 0.9082933, 0.7647289, -0.377, 0.967, 0.0641972, 0.4470636, -0.9064832, 0.6898704 };
    test_complex_syrk<double>(101,112,122,2,1,alpha,beta,A,C,R,ts,dbleps,"zsyrk(case 1596)");
  };


  {
    gslmm::complex<double> alpha(-0.3, 0.1);
    gslmm::complex<double> beta(1., 0.);
    double A[]	= { -0.067, -0.586, 0.208, 0.331 };
    double C[]	= { 0.584, -0.454, 0.93, 0.782, 0.489, -0.278, 0.081, -0.919 };
    double R[]	= { 0.6778197, -0.5114479, 0.8903975, 0.8432225, 0.489, -0.278, 0.0871195, -0.9669385 };
    test_complex_syrk<double>(102,112,122,2,1,alpha,beta,A,C,R,ts,dbleps,"zsyrk(case 1597)");
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
  "test_syrk<\\6>(\\1,\\3,\\2,\\4,\\5,\\7,\\8);")

   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int uplo\\s-*=\\s-*\\(.*\\);
   int trans\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);
   int K\\s-*=\\s-*\\(.*\\);
   \\(float\\|double\\) alpha\\[2\\]\\s-*=\\s-*{\\(.*\\)};
   \\6 beta\\[2\\]\\s-*=\\s-*{\\(.*\\)};"
  " gslmm::complex<\\6> alpha(\\7);
    gslmm::complex<\\6> beta(\\8);
    test_complex_syrk<\\6>(\\1,\\3,\\2,\\4,\\5,alpha,beta);")

   (query-replace-regexp 
   "test_\\(complex_\\|\\)syrk<\\(float\\|double\\)>(\\(.*\\));
\\s-*\\2 A\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int lda.*;
\\s-*\\2 C\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*int ldc.*;
\\s-*\\2 C_expected\\[\\]\\s-*=\\s-*{\\(.*\\)};
\\s-*cblas_.syrk(.*);
\\s-*{
\\s-*int i;
\\s-*for.*{
\\s-*gsl_test_rel(C\\[.*\\], C_expected\\[.*], \\(flt\\|dbl\\)eps, \"\\(.*)\\).*\");[^}]*
\\s-*};?
\\s-*};"
    " \\2 A[]	= {\\4};
    \\2 C[]	= {\\5};
    \\2 R[]	= {\\6};
    test_\\1syrk<\\2>(\\3,A,C,R,ts,\\7eps,\"\\8\");")
    
*/
