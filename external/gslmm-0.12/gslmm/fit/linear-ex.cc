#include <gslmm/fit/linear.hh>
#include <iostream>
#include <sstream>
#include <gslmm/random/gaussian.hh>
#include <iomanip>

std::pair<double, double>
create_data(double_t x, gslmm::generator& g)
{
  std::pair<double, double> ret;
  double y0    = exp(x);
  ret.second   = 0.1 * y0;
  gslmm::gaussian r(ret.second, g);
  double dy    = r.sample();
  ret.first    = y0 + dy;
  return ret;
}

  

int main(int argc, char** arg) 
{
  size_t n = 19;
#if 0
  if (argc < 1) {
    std::cerr << "must supply a data set size" << std::endl;
    return 1;
  }
  std::stringstream s(argv[1]);
  s >> n;
#endif
  gslmm::generator::env_setup();
  gslmm::generator g;
  
  gslmm::matrix<double> x(n, 3);
  gslmm::vector<double> y(n);
  gslmm::vector<double> w(n);
  
  for (size_t i = 0; i < n; i++) {
    double ix = 0.1 + i * 0.1;
    std::pair<double,double> data = create_data(ix, g);
    x(i, 0) = 1;
    x(i, 1) = ix;
    x(i, 2) = ix*ix;
    y[i]    = data.first;
    w[i]    = 1. / (data.second * data.second);
    std::cout << std::setw(3) << i << "\t" 
	      << ix << "\t" << y[i] << "\t" << data.second << std::endl;
  }
  gslmm::linear<3> fitter;
  fitter.fit(x, w, y);
  
  const gslmm::matrix<double>& covar = fitter.covarience();
  const gslmm::vector<double>& param = fitter.parameters();
  double                       chisq = fitter.chi_square();
  size_t                       ndf   = fitter.degrees_of_freedom();

  std::cout << "Fit gave\n" << std::flush;
  for (size_t i = 0; i < 3; i++)
    std::cout << " Parameter # " << i << ":\t" 
	      << std::setw(10) << param[0] << " +/- " 
	      << std::setw(10) << sqrt(covar(0,0)) << std::endl;
  std::cout << " chi^2/ndf:\t" << std::setw(10) << chisq << "/" << ndf << "\n"
	    << " Covariance matrix:" << std::endl;
  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) 
      std::cout << "\t" << std::setw(10) << covar(i, j) << std::flush;
    std::cout << std::endl;
  }
  return 0;
}

  
