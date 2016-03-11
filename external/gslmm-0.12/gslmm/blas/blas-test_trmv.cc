#ifndef GSLMM_blas_test
# include <gslmm/blas/blas-test.hh>
#endif

//____________________________________________________________________
template <typename T> 
void
test_trmv(int order, int trans, int uplo, int diag,
	  size_t n, T* a, T* x, T* r, 
	  gslmm::test_suite& ts, const T& tol, const char* msg) 
{
  using namespace gslmm;
  
  size_t ma = n;
  size_t na = n;

  if (order == 102) {
    // Input is in column order mode - restructure 
    make_row_major(ma, na, a);
  }
  
  matrix<T> A(a, ma, na);
  vector<T> X(x, ma);
  vector<T> Y(ma);
  vector<T> R(r, ma);

  matrix_vector_product(T(1.0), 
			matrix_manip<T>(A, triangular_type, 
					transform(trans), 
					location(uplo),
					diagonal(diag)),
			X, T(1.0), Y);
  test_result(Y, R, ts, tol, msg);
}

//____________________________________________________________________
template <typename T> 
void
test_complex_trmv(int order, int trans, int uplo, int diag,
		  size_t n, T* a, T* x, T* r, 
		  gslmm::test_suite& ts, const T& tol, const char* msg) 
{
  using namespace gslmm;
  
  size_t ma = n;
  size_t na = n;

  if (order == 102) {
    // Input is in column order mode - restructure 
    make_complex_row_major(ma, na, a);
  }
  
  matrix<complex<T> > A(a, ma, na);
  vector<complex<T> > X(x, ma);
  vector<complex<T> > Y(ma);
  vector<complex<T> > R(r, ma);

  matrix_vector_product(complex<T>(1.,0), 
			matrix_manip<complex<T> >(A, 
						  triangular_type, 
						  transform(trans), 
						  location(uplo),
						  diagonal(diag)),
			X, complex<T>(1.,0), Y);
  test_complex_result(Y, R, ts, tol, msg);
}
//____________________________________________________________________
void
do_test_trmv (gslmm::test_suite& ts) 
{
  const double flteps = 1e-4, dbleps = 1e-6;
  {
    float A[] = { 0.987f };
    float X[] = { -0.138f };
    float R[] = { -0.136206f };
    test_trmv<float>(101,111,121,131,1,A,X,R,ts,flteps,"strmv(case 814)");
  };


  {
    float A[] = { 0.987f };
    float X[] = { -0.138f };
    float R[] = { -0.138f };
    test_trmv<float>(101,111,121,132,1,A,X,R,ts,flteps,"strmv(case 815)");
  };


  {
    float A[] = { 0.987f };
    float X[] = { -0.138f };
    float R[] = { -0.136206f };
    test_trmv<float>(101,111,122,131,1,A,X,R,ts,flteps,"strmv(case 816)");
  };


  {
    float A[] = { 0.987f };
    float X[] = { -0.138f };
    float R[] = { -0.138f };
    test_trmv<float>(101,111,122,132,1,A,X,R,ts,flteps,"strmv(case 817)");
  };


  {
    float A[] = { 0.987f };
    float X[] = { -0.138f };
    float R[] = { -0.136206f };
    test_trmv<float>(102,111,121,131,1,A,X,R,ts,flteps,"strmv(case 818)");
  };


  {
    float A[] = { 0.987f };
    float X[] = { -0.138f };
    float R[] = { -0.138f };
    test_trmv<float>(102,111,121,132,1,A,X,R,ts,flteps,"strmv(case 819)");
  };


  {
    float A[] = { 0.987f };
    float X[] = { -0.138f };
    float R[] = { -0.136206f };
    test_trmv<float>(102,111,122,131,1,A,X,R,ts,flteps,"strmv(case 820)");
  };


  {
    float A[] = { 0.987f };
    float X[] = { -0.138f };
    float R[] = { -0.138f };
    test_trmv<float>(102,111,122,132,1,A,X,R,ts,flteps,"strmv(case 821)");
  };


  {
    float A[] = { -0.329f };
    float X[] = { 0.463f };
    float R[] = { -0.152327f };
    test_trmv<float>(101,112,121,131,1,A,X,R,ts,flteps,"strmv(case 822)");
  };


  {
    float A[] = { -0.329f };
    float X[] = { 0.463f };
    float R[] = { 0.463f };
    test_trmv<float>(101,112,121,132,1,A,X,R,ts,flteps,"strmv(case 823)");
  };


  {
    float A[] = { -0.329f };
    float X[] = { 0.463f };
    float R[] = { -0.152327f };
    test_trmv<float>(101,112,122,131,1,A,X,R,ts,flteps,"strmv(case 824)");
  };


  {
    float A[] = { -0.329f };
    float X[] = { 0.463f };
    float R[] = { 0.463f };
    test_trmv<float>(101,112,122,132,1,A,X,R,ts,flteps,"strmv(case 825)");
  };


  {
    float A[] = { -0.329f };
    float X[] = { 0.463f };
    float R[] = { -0.152327f };
    test_trmv<float>(102,112,121,131,1,A,X,R,ts,flteps,"strmv(case 826)");
  };


  {
    float A[] = { -0.329f };
    float X[] = { 0.463f };
    float R[] = { 0.463f };
    test_trmv<float>(102,112,121,132,1,A,X,R,ts,flteps,"strmv(case 827)");
  };


  {
    float A[] = { -0.329f };
    float X[] = { 0.463f };
    float R[] = { -0.152327f };
    test_trmv<float>(102,112,122,131,1,A,X,R,ts,flteps,"strmv(case 828)");
  };


  {
    float A[] = { -0.329f };
    float X[] = { 0.463f };
    float R[] = { 0.463f };
    test_trmv<float>(102,112,122,132,1,A,X,R,ts,flteps,"strmv(case 829)");
  };


  {
    double A[] = { -0.429 };
    double X[] = { -0.899 };
    double R[] = { 0.385671 };
    test_trmv<double>(101,111,121,131,1,A,X,R,ts,dbleps,"dtrmv(case 830)");
  };


  {
    double A[] = { -0.429 };
    double X[] = { -0.899 };
    double R[] = { -0.899 };
    test_trmv<double>(101,111,121,132,1,A,X,R,ts,dbleps,"dtrmv(case 831)");
  };


  {
    double A[] = { -0.429 };
    double X[] = { -0.899 };
    double R[] = { 0.385671 };
    test_trmv<double>(101,111,122,131,1,A,X,R,ts,dbleps,"dtrmv(case 832)");
  };


  {
    double A[] = { -0.429 };
    double X[] = { -0.899 };
    double R[] = { -0.899 };
    test_trmv<double>(101,111,122,132,1,A,X,R,ts,dbleps,"dtrmv(case 833)");
  };


  {
    double A[] = { -0.429 };
    double X[] = { -0.899 };
    double R[] = { 0.385671 };
    test_trmv<double>(102,111,121,131,1,A,X,R,ts,dbleps,"dtrmv(case 834)");
  };


  {
    double A[] = { -0.429 };
    double X[] = { -0.899 };
    double R[] = { -0.899 };
    test_trmv<double>(102,111,121,132,1,A,X,R,ts,dbleps,"dtrmv(case 835)");
  };


  {
    double A[] = { -0.429 };
    double X[] = { -0.899 };
    double R[] = { 0.385671 };
    test_trmv<double>(102,111,122,131,1,A,X,R,ts,dbleps,"dtrmv(case 836)");
  };


  {
    double A[] = { -0.429 };
    double X[] = { -0.899 };
    double R[] = { -0.899 };
    test_trmv<double>(102,111,122,132,1,A,X,R,ts,dbleps,"dtrmv(case 837)");
  };


  {
    double A[] = { 0.842 };
    double X[] = { 0.192 };
    double R[] = { 0.161664 };
    test_trmv<double>(101,112,121,131,1,A,X,R,ts,dbleps,"dtrmv(case 838)");
  };


  {
    double A[] = { 0.842 };
    double X[] = { 0.192 };
    double R[] = { 0.192 };
    test_trmv<double>(101,112,121,132,1,A,X,R,ts,dbleps,"dtrmv(case 839)");
  };


  {
    double A[] = { 0.842 };
    double X[] = { 0.192 };
    double R[] = { 0.161664 };
    test_trmv<double>(101,112,122,131,1,A,X,R,ts,dbleps,"dtrmv(case 840)");
  };


  {
    double A[] = { 0.842 };
    double X[] = { 0.192 };
    double R[] = { 0.192 };
    test_trmv<double>(101,112,122,132,1,A,X,R,ts,dbleps,"dtrmv(case 841)");
  };


  {
    double A[] = { 0.842 };
    double X[] = { 0.192 };
    double R[] = { 0.161664 };
    test_trmv<double>(102,112,121,131,1,A,X,R,ts,dbleps,"dtrmv(case 842)");
  };


  {
    double A[] = { 0.842 };
    double X[] = { 0.192 };
    double R[] = { 0.192 };
    test_trmv<double>(102,112,121,132,1,A,X,R,ts,dbleps,"dtrmv(case 843)");
  };


  {
    double A[] = { 0.842 };
    double X[] = { 0.192 };
    double R[] = { 0.161664 };
    test_trmv<double>(102,112,122,131,1,A,X,R,ts,dbleps,"dtrmv(case 844)");
  };


  {
    double A[] = { 0.842 };
    double X[] = { 0.192 };
    double R[] = { 0.192 };
    test_trmv<double>(102,112,122,132,1,A,X,R,ts,dbleps,"dtrmv(case 845)");
  };

#if 0
  {
    float A[] = { -0.162f, -0.108f };
    float X[] = { 0.542f, 0.461f };
    float R[] = { -0.038016f, -0.133218f };
    test_complex_trmv<float>(101,111,121,131,1,A,X,R,ts,flteps,"ctrmv(case 846)");
  };


  {
    float A[] = { -0.162f, -0.108f };
    float X[] = { 0.542f, 0.461f };
    float R[] = { 0.542f, 0.461f };
    test_complex_trmv<float>(101,111,121,132,1,A,X,R,ts,flteps,"ctrmv(case 847)");
  };


  {
    float A[] = { -0.162f, -0.108f };
    float X[] = { 0.542f, 0.461f };
    float R[] = { -0.038016f, -0.133218f };
    test_complex_trmv<float>(101,111,122,131,1,A,X,R,ts,flteps,"ctrmv(case 848)");
  };


  {
    float A[] = { -0.162f, -0.108f };
    float X[] = { 0.542f, 0.461f };
    float R[] = { 0.542f, 0.461f };
    test_complex_trmv<float>(101,111,122,132,1,A,X,R,ts,flteps,"ctrmv(case 849)");
  };


  {
    float A[] = { -0.162f, -0.108f };
    float X[] = { 0.542f, 0.461f };
    float R[] = { -0.038016f, -0.133218f };
    test_complex_trmv<float>(102,111,121,131,1,A,X,R,ts,flteps,"ctrmv(case 850)");
  };


  {
    float A[] = { -0.162f, -0.108f };
    float X[] = { 0.542f, 0.461f };
    float R[] = { 0.542f, 0.461f };
    test_complex_trmv<float>(102,111,121,132,1,A,X,R,ts,flteps,"ctrmv(case 851)");
  };


  {
    float A[] = { -0.162f, -0.108f };
    float X[] = { 0.542f, 0.461f };
    float R[] = { -0.038016f, -0.133218f };
    test_complex_trmv<float>(102,111,122,131,1,A,X,R,ts,flteps,"ctrmv(case 852)");
  };


  {
    float A[] = { -0.162f, -0.108f };
    float X[] = { 0.542f, 0.461f };
    float R[] = { 0.542f, 0.461f };
    test_complex_trmv<float>(102,111,122,132,1,A,X,R,ts,flteps,"ctrmv(case 853)");
  };


  {
    float A[] = { 0.547f, 0.583f };
    float X[] = { -0.302f, 0.434f };
    float R[] = { -0.418216f, 0.061332f };
    test_complex_trmv<float>(101,112,121,131,1,A,X,R,ts,flteps,"ctrmv(case 854)");
  };


  {
    float A[] = { 0.547f, 0.583f };
    float X[] = { -0.302f, 0.434f };
    float R[] = { -0.302f, 0.434f };
    test_complex_trmv<float>(101,112,121,132,1,A,X,R,ts,flteps,"ctrmv(case 855)");
  };


  {
    float A[] = { 0.547f, 0.583f };
    float X[] = { -0.302f, 0.434f };
    float R[] = { -0.418216f, 0.061332f };
    test_complex_trmv<float>(101,112,122,131,1,A,X,R,ts,flteps,"ctrmv(case 856)");
  };


  {
    float A[] = { 0.547f, 0.583f };
    float X[] = { -0.302f, 0.434f };
    float R[] = { -0.302f, 0.434f };
    test_complex_trmv<float>(101,112,122,132,1,A,X,R,ts,flteps,"ctrmv(case 857)");
  };


  {
    float A[] = { 0.547f, 0.583f };
    float X[] = { -0.302f, 0.434f };
    float R[] = { -0.418216f, 0.061332f };
    test_complex_trmv<float>(102,112,121,131,1,A,X,R,ts,flteps,"ctrmv(case 858)");
  };


  {
    float A[] = { 0.547f, 0.583f };
    float X[] = { -0.302f, 0.434f };
    float R[] = { -0.302f, 0.434f };
    test_complex_trmv<float>(102,112,121,132,1,A,X,R,ts,flteps,"ctrmv(case 859)");
  };


  {
    float A[] = { 0.547f, 0.583f };
    float X[] = { -0.302f, 0.434f };
    float R[] = { -0.418216f, 0.061332f };
    test_complex_trmv<float>(102,112,122,131,1,A,X,R,ts,flteps,"ctrmv(case 860)");
  };


  {
    float A[] = { 0.547f, 0.583f };
    float X[] = { -0.302f, 0.434f };
    float R[] = { -0.302f, 0.434f };
    test_complex_trmv<float>(102,112,122,132,1,A,X,R,ts,flteps,"ctrmv(case 861)");
  };


  {
    float A[] = { 0.216f, 0.192f };
    float X[] = { -0.564f, -0.297f };
    float R[] = { -0.178848f, 0.044136f };
    test_complex_trmv<float>(101,113,121,131,1,A,X,R,ts,flteps,"ctrmv(case 862)");
  };


  {
    float A[] = { 0.216f, 0.192f };
    float X[] = { -0.564f, -0.297f };
    float R[] = { -0.564f, -0.297f };
    test_complex_trmv<float>(101,113,121,132,1,A,X,R,ts,flteps,"ctrmv(case 863)");
  };


  {
    float A[] = { 0.216f, 0.192f };
    float X[] = { -0.564f, -0.297f };
    float R[] = { -0.178848f, 0.044136f };
    test_complex_trmv<float>(101,113,122,131,1,A,X,R,ts,flteps,"ctrmv(case 864)");
  };


  {
    float A[] = { 0.216f, 0.192f };
    float X[] = { -0.564f, -0.297f };
    float R[] = { -0.564f, -0.297f };
    test_complex_trmv<float>(101,113,122,132,1,A,X,R,ts,flteps,"ctrmv(case 865)");
  };


  {
    float A[] = { 0.216f, 0.192f };
    float X[] = { -0.564f, -0.297f };
    float R[] = { -0.178848f, 0.044136f };
    test_complex_trmv<float>(102,113,121,131,1,A,X,R,ts,flteps,"ctrmv(case 866)");
  };


  {
    float A[] = { 0.216f, 0.192f };
    float X[] = { -0.564f, -0.297f };
    float R[] = { -0.564f, -0.297f };
    test_complex_trmv<float>(102,113,121,132,1,A,X,R,ts,flteps,"ctrmv(case 867)");
  };


  {
    float A[] = { 0.216f, 0.192f };
    float X[] = { -0.564f, -0.297f };
    float R[] = { -0.178848f, 0.044136f };
    test_complex_trmv<float>(102,113,122,131,1,A,X,R,ts,flteps,"ctrmv(case 868)");
  };


  {
    float A[] = { 0.216f, 0.192f };
    float X[] = { -0.564f, -0.297f };
    float R[] = { -0.564f, -0.297f };
    test_complex_trmv<float>(102,113,122,132,1,A,X,R,ts,flteps,"ctrmv(case 869)");
  };
#endif

  {
    double A[] = { 0.693, -0.22 };
    double X[] = { -0.101, 0.889 };
    double R[] = { 0.125587, 0.638297 };
    test_complex_trmv<double>(101,111,121,131,1,A,X,R,ts,dbleps,"ztrmv(case 870)");
  };


  {
    double A[] = { 0.693, -0.22 };
    double X[] = { -0.101, 0.889 };
    double R[] = { -0.101, 0.889 };
    test_complex_trmv<double>(101,111,121,132,1,A,X,R,ts,dbleps,"ztrmv(case 871)");
  };


  {
    double A[] = { 0.693, -0.22 };
    double X[] = { -0.101, 0.889 };
    double R[] = { 0.125587, 0.638297 };
    test_complex_trmv<double>(101,111,122,131,1,A,X,R,ts,dbleps,"ztrmv(case 872)");
  };


  {
    double A[] = { 0.693, -0.22 };
    double X[] = { -0.101, 0.889 };
    double R[] = { -0.101, 0.889 };
    test_complex_trmv<double>(101,111,122,132,1,A,X,R,ts,dbleps,"ztrmv(case 873)");
  };


  {
    double A[] = { 0.693, -0.22 };
    double X[] = { -0.101, 0.889 };
    double R[] = { 0.125587, 0.638297 };
    test_complex_trmv<double>(102,111,121,131,1,A,X,R,ts,dbleps,"ztrmv(case 874)");
  };


  {
    double A[] = { 0.693, -0.22 };
    double X[] = { -0.101, 0.889 };
    double R[] = { -0.101, 0.889 };
    test_complex_trmv<double>(102,111,121,132,1,A,X,R,ts,dbleps,"ztrmv(case 875)");
  };


  {
    double A[] = { 0.693, -0.22 };
    double X[] = { -0.101, 0.889 };
    double R[] = { 0.125587, 0.638297 };
    test_complex_trmv<double>(102,111,122,131,1,A,X,R,ts,dbleps,"ztrmv(case 876)");
  };


  {
    double A[] = { 0.693, -0.22 };
    double X[] = { -0.101, 0.889 };
    double R[] = { -0.101, 0.889 };
    test_complex_trmv<double>(102,111,122,132,1,A,X,R,ts,dbleps,"ztrmv(case 877)");
  };


  {
    double A[] = { -0.216, -0.623 };
    double X[] = { 0.048, 0.293 };
    double R[] = { 0.172171, -0.093192 };
    test_complex_trmv<double>(101,112,121,131,1,A,X,R,ts,dbleps,"ztrmv(case 878)");
  };


  {
    double A[] = { -0.216, -0.623 };
    double X[] = { 0.048, 0.293 };
    double R[] = { 0.048, 0.293 };
    test_complex_trmv<double>(101,112,121,132,1,A,X,R,ts,dbleps,"ztrmv(case 879)");
  };


  {
    double A[] = { -0.216, -0.623 };
    double X[] = { 0.048, 0.293 };
    double R[] = { 0.172171, -0.093192 };
    test_complex_trmv<double>(101,112,122,131,1,A,X,R,ts,dbleps,"ztrmv(case 880)");
  };


  {
    double A[] = { -0.216, -0.623 };
    double X[] = { 0.048, 0.293 };
    double R[] = { 0.048, 0.293 };
    test_complex_trmv<double>(101,112,122,132,1,A,X,R,ts,dbleps,"ztrmv(case 881)");
  };


  {
    double A[] = { -0.216, -0.623 };
    double X[] = { 0.048, 0.293 };
    double R[] = { 0.172171, -0.093192 };
    test_complex_trmv<double>(102,112,121,131,1,A,X,R,ts,dbleps,"ztrmv(case 882)");
  };


  {
    double A[] = { -0.216, -0.623 };
    double X[] = { 0.048, 0.293 };
    double R[] = { 0.048, 0.293 };
    test_complex_trmv<double>(102,112,121,132,1,A,X,R,ts,dbleps,"ztrmv(case 883)");
  };


  {
    double A[] = { -0.216, -0.623 };
    double X[] = { 0.048, 0.293 };
    double R[] = { 0.172171, -0.093192 };
    test_complex_trmv<double>(102,112,122,131,1,A,X,R,ts,dbleps,"ztrmv(case 884)");
  };


  {
    double A[] = { -0.216, -0.623 };
    double X[] = { 0.048, 0.293 };
    double R[] = { 0.048, 0.293 };
    test_complex_trmv<double>(102,112,122,132,1,A,X,R,ts,dbleps,"ztrmv(case 885)");
  };


  {
    double A[] = { -0.345, -0.851 };
    double X[] = { -0.708, 0.298 };
    double R[] = { -0.009338, -0.705318 };
    test_complex_trmv<double>(101,113,121,131,1,A,X,R,ts,dbleps,"ztrmv(case 886)");
  };


  {
    double A[] = { -0.345, -0.851 };
    double X[] = { -0.708, 0.298 };
    double R[] = { -0.708, 0.298 };
    test_complex_trmv<double>(101,113,121,132,1,A,X,R,ts,dbleps,"ztrmv(case 887)");
  };


  {
    double A[] = { -0.345, -0.851 };
    double X[] = { -0.708, 0.298 };
    double R[] = { -0.009338, -0.705318 };
    test_complex_trmv<double>(101,113,122,131,1,A,X,R,ts,dbleps,"ztrmv(case 888)");
  };


  {
    double A[] = { -0.345, -0.851 };
    double X[] = { -0.708, 0.298 };
    double R[] = { -0.708, 0.298 };
    test_complex_trmv<double>(101,113,122,132,1,A,X,R,ts,dbleps,"ztrmv(case 889)");
  };


  {
    double A[] = { -0.345, -0.851 };
    double X[] = { -0.708, 0.298 };
    double R[] = { -0.009338, -0.705318 };
    test_complex_trmv<double>(102,113,121,131,1,A,X,R,ts,dbleps,"ztrmv(case 890)");
  };


  {
    double A[] = { -0.345, -0.851 };
    double X[] = { -0.708, 0.298 };
    double R[] = { -0.708, 0.298 };
    test_complex_trmv<double>(102,113,121,132,1,A,X,R,ts,dbleps,"ztrmv(case 891)");
  };


  {
    double A[] = { -0.345, -0.851 };
    double X[] = { -0.708, 0.298 };
    double R[] = { -0.009338, -0.705318 };
    test_complex_trmv<double>(102,113,122,131,1,A,X,R,ts,dbleps,"ztrmv(case 892)");
  };


  {
    double A[] = { -0.345, -0.851 };
    double X[] = { -0.708, 0.298 };
    double R[] = { -0.708, 0.298 };
    test_complex_trmv<double>(102,113,122,132,1,A,X,R,ts,dbleps,"ztrmv(case 893)");
  };


}
/*
   (query-replace-regexp 
  "int order\\s-*=\\s-*\\(.*\\);
   int trans\\s-*=\\s-*\\(.*\\);
   int uplo\\s-*=\\s-*\\(.*\\);
   int diag\\s-*=\\s-*\\(.*\\);
   int N\\s-*=\\s-*\\(.*\\);
   int lda\\s-*.*;
   \\(float\\|double\\) A\\[\\]\\s-*=\\s-*\\(.*\\);
   \\6 X\\[\\]\\s-*=\\s-*\\(.*\\);
   int incX.*
   \\6 x_expected\\[\\]\\s-*=\\s-*\\(.*\\);"
  " \\6 A[] = \\7;
    \\6 X[] = \\8;
    \\6 R[] = \\9
    test_trmv<\\6>(\\1,\\2,\\3,\\4,\\5,A,X,R);")

   (query-replace-regexp 
   "test_trmv<\\(float\\|double\\)>(\\(.*\\));
\\s-*cblas_\\([sdcz]\\)trmv.*
\\s-*{
\\s-*int i;
\\s-*for.*{
\\s-*gsl_test_rel(X\\[.*\\],\\s-*x_expected\\[.*\\],\\s-*\\(flt\\|dbl\\)eps,\\s-*\"\\(.*)\\).*\");[^}]*
\\s-*};?
\\s-*};"
    "test_\\3_trmv<\\1>(\\2,ts,\\4eps,\"\\5\");")

*/

