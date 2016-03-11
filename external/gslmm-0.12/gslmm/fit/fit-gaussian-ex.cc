#include <gslmm/fit/fit.hh>
#include <gslmm/random/gaussian.hh>
#include <gslmm/error/error_handler.hh>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

struct myfit
{
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> e;
  
  int function(const gslmm::vector<double>& p, gslmm::vector<double>& f)
  {
    double A     = p[0];
    double mu    = p[1];
    double sigma = p[2];
    for (size_t i = 0; i < y.size(); i++) {
      double v  = A * exp(-(x[i] * x[i] - mu * mu) / (sigma * sigma));
      f[i]      = (v - y[i]) / e[i];
    }
    return GSL_SUCCESS;
  }
  int jacobian(const gslmm::vector<double>& p, gslmm::matrix<double>& j)
  {
    double A     = p[0];
    double mu    = p[1];
    double sigma = p[2];
    for (size_t i = 0; i < y.size(); i++) {
      double ee = exp(-(x[i] * x[i] - mu * mu) / (sigma * sigma));
      j(i, 0)   = ee;
      j(i, 1)   = 2 * A * mu * ee / (sigma * sigma);
      j(i, 2)   = - 2 * A * (mu * mu - x[i] * x[i]) * ee / (sigma * sigma);
    }
    return GSL_SUCCESS;
  }
  int iterate() { return GSL_SUCCESS; }
  myfit() 
    : x(0), y(0), e(0)
  {
    while (!std::cin.eof()) {
      double x1, x2, y1;
      std::cin >> x1 >> x2 >> y1;
      if (std::cin.bad()) return;
      if (y1 > 0) {
	x.push_back((x1 + x2) / 2);
	y.push_back(y1);
	e.push_back(sqrt(y1));
      }
    } 
    for (size_t i = 0; i < x.size(); i++) 
      std::cout << std::setw(10) << x[i] << " -> "
		<< std::setw(10) << y[i] << " +/- "
		<< std::setw(10) << e[i] << std::endl;
  }
  size_t size() const  
  {
    return x.size();
  }
};

template <typename T>
void 
print_status(size_t iter, const gslmm::fitter<T>& fit, bool cont) 
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
  myfit e;
  gslmm::vector<double> initial(3);
  initial[0] = 1;
  initial[2] = 1;
  gslmm::fitter<myfit> fit;
  fit.initialize(e, gslmm::fitter<myfit>::scaled_levenberg_marquardt, 
		 initial, e.size());
  print_status(0, fit, true);
  size_t iter = 1;
  int status  = 0;
  bool cont = true;
  do {
    status = fit.iterate();
    if (status) { 
      std::cout << "bad status " << status << ": " 
		<< gslmm::error_handler<>::code_to_string(status) << std::endl;
      break;
    }
    cont = !fit.test_delta(1e-4, 1e-4);
  } while (cont && ++iter < 500);
  gslmm::matrix<double> c(3,3);
  gslmm::vector<double> p(fit.position());
  fit.covariance(c, 0);
  
  for (size_t i = 0; i < 3; i++) {
    std::cout << (i == 0 ? "A" : (i == 1 ? "mu" : "sigma"))
	      << "\t\t= " << std::setw(10) << p[i] 
	      << " +/" << std::setw(10) << sqrt(c(i,i)) << std::endl;
  }
  gslmm::vector<double> f(fit.values());
  double chisq = f.length();
  std::cout << "chi^2/dof\t= " << std::setw(10) << (chisq*chisq) 
	    << " / 37 = " << (chisq*chisq)/37 << "\nstatus\t\t= " 
	    << gslmm::error_handler<>::code_to_string(status) << std::endl;
  return status;
}
