#ifndef GSLMM_vector
# include <gslmm/vectormatrix/vector.hh>
#endif
#ifndef GSLMM_vector_double
# include <gslmm/vectormatrix/vector_double.hh>
#endif
#ifndef GSLMM_vector_float
# include <gslmm/vectormatrix/vector_float.hh>
#endif
// #ifndef GSLMM_vector_complex_float
// # include <gslmm/vectormatrix/vector_complex_float.hh>
// #endif
#ifndef GSLMM_vector_complex_double
# include <gslmm/vectormatrix/vector_complex_double.hh>
#endif
#ifndef GSLMM_blas
# include <gslmm/blas/blas.hh>
#endif
#ifndef GSLMM_test_suite
# define NO_TESTSUITE_INSTANCE
# include <gslmm/test/test_suite.hh>
#endif

void
do_test_asum (gslmm::test_suite& ts) 
{
  const double flteps = 1e-4, dbleps = 1e-6;
  {
    float X[]	= { 0.239f };
    gslmm::vector<float> v(X,1);
    float f = gslmm::absolute_sum(v);
    ts.relative(f, 0.239f, flteps, "sasum(case 40)");
  };


  {
    double X[]	= { -0.413 };
    gslmm::vector<double> v(X,1);
    double f = gslmm::absolute_sum(v);
    ts.relative(f, 0.413f, dbleps, "dasum(case 41)");
  };

#if 0
  {
    float X[]	= { 0.1f, 0.017f };
    gslmm::vector<gslmm::complex<float> > v(X,1);
    float f = gslmm::absolute_sum(v);
    ts.relative(f, 0.0f, flteps, "scasum(case 42)");
  };
#endif

  {
    double X[]	= { -0.651, 0.079 };
    gslmm::vector<gslmm::complex<double> > v(X,1);
    double f = gslmm::absolute_sum(v);
    ts.relative(f, 0.730f, dbleps, "dzasum(case 43)");
  };


  {
    float X[]	= { 0.899f, -0.72f };
    gslmm::vector<float> v(X,2);
    float f = gslmm::absolute_sum(v);
    ts.relative(f, 1.619f, flteps, "sasum(case 44)");
  };


  {
    double X[]	= { 0.271, -0.012 };
    gslmm::vector<double> v(X,2);
    double f = gslmm::absolute_sum(v);
    ts.relative(f, 0.283, dbleps, "dasum(case 45)");
  };

#if 0
  {
    float X[]	= { -0.567f, -0.645f, 0.098f, 0.256f };
    gslmm::vector<gslmm::complex<float> > v(X,2);
    float f = gslmm::absolute_sum(v);
    ts.relative(f, 1.566f, flteps, "scasum(case 46)");
  };
#endif

  {
    double X[]	= { -0.046, -0.671, -0.323, 0.785 };
    gslmm::vector<gslmm::complex<double> > v(X,2);
    double f = gslmm::absolute_sum(v);
    ts.relative(f, 1.825, dbleps, "dzasum(case 47)");
  };


  {
    float X[]	= { 0.169f, 0.833f };
    gslmm::vector<float> v(X,2);
    float f = gslmm::absolute_sum(v);
    ts.relative(f, 1.002f, flteps, "sasum(case 48)");
  };


  {
    double X[]	= { -0.586, -0.486 };
    gslmm::vector<double> v(X,2);
    double f = gslmm::absolute_sum(v);
    ts.relative(f, 1.072f, dbleps, "dasum(case 49)");
  };

#if 0
  {
    float X[]	= { -0.314f, -0.318f, -0.835f, -0.807f };
    gslmm::vector<gslmm::complex<float> > v(X,2);
    float f = gslmm::absolute_sum(v);
    ts.relative(f, 0.0f, flteps, "scasum(case 50)");
  };
#endif

  {

    double X[]	= { -0.927, 0.152, -0.554, -0.844 };
    gslmm::vector<gslmm::complex<double> > v(X,2);
    double f = gslmm::absolute_sum(v);
    ts.relative(f, 2.477f, dbleps, "dzasum(case 51)");
  };


}
/*
 (query-replace-regexp 
   "int N = \\(.*\\);
   \\(float\\|double\\) X\\[\\] = {\\(.*\\)};
   int incX.*
   \\2 expected = \\(.*\\);
   \\2 f;
   f = cblas_.asum(N, X, incX);
   gsl_test_rel(f, expected, \\(flt\\|dbl\\)eps, \"\\(.*\\)\");"
   " \\2 X[]	= {\\3};
     gslmm::vector<\\2> v(X,\\1);
     \\2 f = gslmm::absolute_sum(v);
     ts.relative(f, \\4, \\5eps, \"\\6\");")

 (query-replace-regexp 
   "int N = \\(.*\\);
   \\(float\\|double\\) X\\[\\] = {\\(.*\\)};
   int incX.*
   \\2 expected = \\(.*\\);
   \\2 f;
   f = cblas_.[cz]asum(N, X, incX);
   gsl_test_rel(f, expected, \\(flt\\|dbl\\)eps, \"\\(.*\\)\");"
   " \\2 X[]	= {\\3};
     gslmm::vector<gslmm::complex<\\2> > v(X,\\1);
     \\2 f = gslmm::absolute_sum(v);
     ts.relative(f, \\4, \\5eps, \"\\6\");")
*/
