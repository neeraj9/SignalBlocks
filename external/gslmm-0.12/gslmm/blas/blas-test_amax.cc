#ifndef GSLMM_vectormamtrix_vector
# include <gslmm/vectormatrix/vector.hh>
#endif
// #ifndef GSLMM_vector_double
// # include <gslmm/vectormatrix/vector_double.hh>
// #endif
// #ifndef GSLMM_vector_float
// # include <gslmm/vectormatrix/vector_float.hh>
// #endif
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

template <typename Type>
void 
test_amax(size_t n, const Type* data, size_t expected, 
	  gslmm::test_suite& ts, const char* msg) 
{

  gslmm::vector<Type> v(n);
  for (size_t i = 0; i < n; i++) v[i] = data[i];
  size_t ret = gslmm::max(v);
  ts.test(ret, expected, msg);
}

void
do_test_amax (gslmm::test_suite& ts) 
{
  {
    float X[] = { -0.388f };
    test_amax(1, X, 0, ts, "samax(case 52)");
  };


  {
    double X[] = { 0.247 };
    test_amax(1, X, 0, ts, "damax(case 53)");
  };


#if 0
  {
    gslmm::complex<float> X(0.704f, 0.665f);
    test_amax(1, &X, 0, ts, "camax(case 54)");
  };
#endif

  {
    gslmm::complex<double> X(-0.599, -0.758);
    test_amax(1, &X, 0, ts, "zamax(case 55)");
  };


  {
    float X[] = { 0.909f, 0.037f };
    test_amax(2, X, 0, ts, "samax(case 56)");
  };


  {
    double X[] = { 0.271, -0.426 };
    test_amax(2, X, 1, ts, "damax(case 57)");
  };

#if 0
  {
    gslmm::complex<float> X[] = { 
      gslmm::complex<float>(-0.648f, 0.317f), 
      gslmm::complex<float>(0.62f, 0.392f) };
    test_amax(2, X, 1, ts, "camax(case 58)");
  };
#endif


  {
    gslmm::complex<double> X[] = { 
      gslmm::complex<double>(-0.789, 0.352), 
      gslmm::complex<double>(0.562, 0.697) };
    test_amax(2, X, 1, ts, "zamax(case 59)");
  };

  {
    float X[] = { 0.487f, 0.918f };
    test_amax(2, X, 1, ts, "samax(case 60)");
  };


  {
    double X[] = { 0.537, 0.826 };
    test_amax(2, X, 1, ts, "damax(case 61)");
  };


#if 0
  {
    int N = 2;
    gslmm::complex<float> X[] = { 
      gslmm::complex<float>(0.993f, 0.172f), 
      gslmm::complex<float>(-0.825f, 0.873f) };
    test_amax(2, X, 0, ts, "camax(case 62)");
  };
#endif


  {
    gslmm::complex<double> X[] = { 
      gslmm::complex<double>(0.913, -0.436), 
      gslmm::complex<double>(-0.134, 0.129) };
    test_amax(2, X, 0, ts, "zamax(case 63)");
  };
}
//
// EOF
//

