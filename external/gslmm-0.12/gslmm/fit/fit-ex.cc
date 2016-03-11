#include <gslmm/fit/fit.hh>
#include <iostream>
#include <sstream>
#include <gslmm/random/gaussian.hh>
#include <gslmm/error/error_handler.hh>
#include <iomanip>
#include <vector>

struct expo 
{
  std::vector<double> y;
  std::vector<double> sigma;
  
  int function(const gslmm::vector<double>& x, gslmm::vector<double>& f)
  {
    double A      = x[0];
    double lambda = x[1];
    double b      = x[2];
    for (size_t i = 0; i < y.size(); i++) {
      double t  = i;
      double yi = A * exp(-lambda * t) + b;
      f[i]      = (yi - y[i]) / sigma[i];
    }
    return GSL_SUCCESS;
  }
  int jacobian(const gslmm::vector<double>& x, gslmm::matrix<double>& j)
  {
    double A      = x[0];
    double lambda = x[1];
    double b      = x[2];
    for (size_t i = 0; i < y.size(); i++) {
      double t   = i;
      double s   = sigma[i];
      double e   = exp(-lambda * t);
      j(i, 0) = e / s;
      j(i, 1) = -t * A * e / s;
      j(i, 2) = 1 / s;
    }
    return GSL_SUCCESS;
  }
  int iterate() { return GSL_SUCCESS; }
  expo(size_t n) 
    : y(n), sigma(n)
  {
    gslmm::generator::env_setup();
    gslmm::generator g;
    gslmm::gaussian  r(0.1, g);
    
    for (size_t i = 0; i < n; i++) {
      double t = i;
      y[i]     = 1. + 5 * exp(-0.1 * t) + r.sample();
      sigma[i] = .1;
    }
  }
};

void 
print_status(size_t iter, const gslmm::fitter<expo>& fit, bool cont) 
{
  gslmm::vector<double> p(fit.position());
  gslmm::vector<double> f(fit.values());
  std::cout << std::setw(4) << iter << " x=("
	    << std::setw(10) << p[0] << ","
	    << std::setw(10) << p[1] << ","
	    << std::setw(10) << p[2] << ") |f(x)|="
	    << std::setw(10) << f.length() << "\t" 
	    << (cont ? "continue" : "done") << std::endl;
}

int main()
{
  expo e(40);
  gslmm::vector<double> initial(3);
  initial[0] = 1;
  gslmm::fitter<expo> fit;
  fit.initialize(e, gslmm::fitter<expo>::scaled_levenberg_marquardt, 
	       initial, 40);
  
  print_status(0, fit, true);
  size_t iter = 1;
  int status  = 0;
  bool cont = true;
  do {
    status = fit.iterate();
    if (status) { 
      std::cout << "bad status " << status << std::endl;
      break;
    }
    cont = !fit.test_delta(1e-4, 1e-4);
    print_status(iter, fit, cont);
  } while (cont && ++iter < 500);
  gslmm::matrix<double> c(3,3);
  gslmm::vector<double> p(fit.position());
  fit.covariance(c, 0);
  
  for (size_t i = 0; i < 3; i++) {
    std::cout << (i == 0 ? "A" : (i == 1 ? "lambda" : "b"))
	      << "\t\t= " << std::setw(10) << p[i] 
	      << " +/" << std::setw(10) << sqrt(c(i,i)) << std::endl;
  }
  gslmm::vector<double> f(fit.values());
  double chisq = f.length();
  std::cout << "chi^2/dof\t= " << std::setw(10) << (chisq*chisq) 
	    << " / 37 = " << (chisq*chisq)/37 << "\nstatus=" 
	    << gslmm::error_handler<>::code_to_string(status) << std::endl;
  return status;
}

  

  
      
