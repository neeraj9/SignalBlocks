//// $Id: randist.cc,v 1.5 2006-05-01 14:25:41 cholm Exp $ 
//  
//  random/distribution-test.cc
//  Copyright (C) 2002 Christian Holm Christensen <cholm@nbi.dk> 
//
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public License 
//  as published by the Free Software Foundation; either version 2.1 
//  of the License, or (at your option) any later version. 
//
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free 
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 
//  02111-1307 USA 
//
#ifndef __IOSTREAM__
# include <iostream>
#endif
#ifndef __IOMANIP__
# include <iomanip>
#endif
#ifndef __SSTREAM__
# include <sstream>
#endif
#ifndef __STRING__
# include <string>
#endif
#ifndef __MAP__
# include <map>
#endif
#ifndef __LIST__
# include <list>
#endif
#ifndef __STDEXCEPT__
# include <stdexcept>
#endif
#ifndef GSLMM_math_type_trait
#include <gslmm/math/type_trait.hh>
#endif
#ifndef GSLMM_random_bernoulli
# include <gslmm/random/bernoulli.hh>
#endif
#ifndef GSLMM_random_beta
# include <gslmm/random/beta.hh>
#endif
#ifndef GSLMM_random_binomial
# include <gslmm/random/binomial.hh>
#endif
#ifndef GSLMM_random_bivariate_gaussian
# include <gslmm/random/bivariate_gaussian.hh>
#endif
#ifndef GSLMM_random_cauchy
# include <gslmm/random/cauchy.hh>
#endif
#ifndef GSLMM_random_chisq
# include <gslmm/random/chisq.hh>
#endif
#ifndef GSLMM_random_dir
# include <gslmm/random/dir.hh>
#endif
#ifndef GSLMM_random_dirichlet
# include <gslmm/random/dirichlet.hh>
#endif
#ifndef GSLMM_random_discrete
# include <gslmm/random/discrete.hh>
#endif
#ifndef GSLMM_random_erlang
# include <gslmm/random/erlang.hh>
#endif
#ifndef GSLMM_random_exponential
# include <gslmm/random/exponential.hh>
#endif
#ifndef GSLMM_random_exponential_power
# include <gslmm/random/exponential_power.hh>
#endif
#ifndef GSLMM_random_fdist
# include <gslmm/random/fdist.hh>
#endif
#ifndef GSLMM_random_flat
# include <gslmm/random/flat.hh>
#endif
#ifndef GSLMM_random_gamma
# include <gslmm/random/gamma.hh>
#endif
#ifndef GSLMM_random_gaussian
# include <gslmm/random/gaussian.hh>
#endif
#ifndef GSLMM_random_gaussian_tail
# include <gslmm/random/gaussian_tail.hh>
#endif
#ifndef GSLMM_random_geometric
# include <gslmm/random/geometric.hh>
#endif
#ifndef GSLMM_random_gumbel
# include <gslmm/random/gumbel.hh>
#endif
#ifndef GSLMM_random_hypergeometric
# include <gslmm/random/hypergeometric.hh>
#endif
#ifndef GSLMM_random_landau
# include <gslmm/random/landau.hh>
#endif
#ifndef GSLMM_random_laplace
# include <gslmm/random/laplace.hh>
#endif
#ifndef GSLMM_random_levy
# include <gslmm/random/levy.hh>
#endif
#ifndef GSLMM_random_levy_skew
# include <gslmm/random/levy_skew.hh>
#endif
#ifndef GSLMM_random_logarithmic
# include <gslmm/random/logarithmic.hh>
#endif
#ifndef GSLMM_random_logistic
# include <gslmm/random/logistic.hh>
#endif
#ifndef GSLMM_random_lognormal
# include <gslmm/random/lognormal.hh>
#endif
#ifndef GSLMM_random_multinomial
# include <gslmm/random/multinomial.hh>
#endif
#ifndef GSLMM_random_negative_binomial
# include <gslmm/random/negative_binomial.hh>
#endif
#ifndef GSLMM_random_pareto
# include <gslmm/random/pareto.hh>
#endif
#ifndef GSLMM_random_pascal
# include <gslmm/random/pascal.hh>
#endif
#ifndef GSLMM_random_poisson
# include <gslmm/random/poisson.hh>
#endif
#ifndef GSLMM_random_rayleigh
# include <gslmm/random/rayleigh.hh>
#endif
#ifndef GSLMM_random_rayleigh_tail
# include <gslmm/random/rayleigh_tail.hh>
#endif
#ifndef GSLMM_random_tdist
# include <gslmm/random/tdist.hh>
#endif
#ifndef GSLMM_random_weibull
# include <gslmm/random/weibull.hh>
#endif
#ifndef GSLMM_test_suite
#include <gslmm/test/test_suite.hh>
#endif
#ifndef optionmm_command_line_hh
# include <optionmm/command_line.hh>
#endif
#ifdef HAVE_CONFIG_H
# include "config.hh"
#else 
# define VERSION "?.?"
#endif

/** @file   randist.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Sample a random distribution of choice.  

    Try for example: 
    @verbatim 
    ./gslmm-randist 1 | \
      ./gslmm-histogram -m -3 -M 3  -n 100 | \
      awk '{print $1, $3 ; print $2, $3}' | \
      graph -T X
    @endverbatim
    To draw a histogram of a guassian distribution with mean 0, and
    spread 1. 
*/

//____________________________________________________________________  
struct runner_base
{
  std::string _name;
  size_t      _needed;
  std::string _doc;
  
  runner_base() 
    : _name(""), _needed(0), _doc("")
  {}
  
  
  runner_base(const std::string& name, 
	      size_t needed,
	      const std::string& doc)
    : _name(name), _needed(needed), _doc(doc)
  {}
  virtual ~runner_base() {}
  
  virtual void operator()(gslmm::generator& rg, size_t nev, 
			  int& argc, char** argv)  
  {
    if (size_t(argc) != _needed) {
      std::string err("Error: arguments should be: ");
      err.append(_doc);
      throw std::runtime_error(err.c_str());
    }
    this->exec(rg, nev, argc, argv);
  }

  virtual void exec(gslmm::generator& rg, size_t nev, 
		    int& argc, char** argv)  = 0;
  
  template <typename Out>
  Out 
  get_parameter(int& argc, char** argv) 
  {
    if (!argc) 
      throw std::runtime_error("missing parameter");
    // ++argv;
    std::stringstream s((argv[0]));
    Out o;
    s >> o;
    argv++;
    argc--;
    return o;
  }
};

//____________________________________________________________________  
template <typename Return>
struct runner : public runner_base
{
  runner(const std::string& name, 
	  size_t needed,
	  const std::string& doc)
    : runner_base(name, needed, doc)
  {}
  void output(gslmm::distribution<Return>& d, size_t n) 
  {
    for (size_t i = 0; i < n; ++i) 
      std::cout << d.sample() << std::endl;
  }
};

template <typename T>
struct runner<std::pair<T,T> > : public runner_base
{
  runner(const std::string& name, 
	  size_t needed,
	  const std::string& doc)
    : runner_base(name, needed, doc)
  {}
  void output(gslmm::distribution<std::pair<T,T> >& d, size_t n) 
  {
    std::pair<T,T> x;
    for (size_t i = 0; i < n; ++i) {
      d.sample(x);
      std::cout << x.first << "\t" << x.second << std::endl;
    }
  }
};

template <typename T>
struct runner<std::vector<T> > : public runner_base
{
  runner(const std::string& name, 
	  size_t needed,
	  const std::string& doc)
    : runner_base(name, needed, doc)
  {}
  void output(gslmm::distribution<std::vector<T> >& d, size_t n) 
  {
    typedef std::vector<T> vector_type;
    vector_type x;
    for (size_t i = 0; i < n; ++i) {
      d.sample(x);
      for (size_t j = 0; j < x.size(); ++j)
	std::cout << x[j] << "\t";
      std::cout << std::endl;
    }
  }
};

//___________________________________________________________
struct run_bernoulli : public runner<gslmm::bernoulli::return_type>
{
  typedef runner<gslmm::bernoulli::return_type> base_type;
  run_bernoulli()
    : base_type("bernoulli", 1, "p = probability of success")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double p = get_parameter<double>(argc, argv);
    gslmm::bernoulli g(p, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_beta : public runner<gslmm::beta::return_type>
{
  typedef runner<gslmm::beta::return_type> base_type;
  run_beta()
    : base_type("beta", 2, "a,b = shape parameters")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv);
    double b = get_parameter<double>(argc, argv);
    gslmm::beta g(a, b, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_binomial : public runner<gslmm::binomial::return_type>
{
  typedef runner<gslmm::binomial::return_type> base_type;
  run_binomial()
    : base_type("binomial", 2, "p = probability, N = number of trials")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double p = get_parameter<double>(argc, argv);
    int N = get_parameter<int>(argc, argv);
    gslmm::binomial g(p, N, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_cauchy : public runner<gslmm::cauchy::return_type>
{
  typedef runner<gslmm::cauchy::return_type> base_type;
  run_cauchy()
    : base_type("cauchy", 1, "a = scale parameter")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv);
    gslmm::cauchy g(a, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_chisq : public runner<gslmm::chisq::return_type>
{
  typedef runner<gslmm::chisq::return_type> base_type;
  run_chisq()
    : base_type("chisq", 1, "nu = degrees of freedom")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double nu = get_parameter<double>(argc, argv);
    gslmm::chisq g(nu, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_erlang : public runner<gslmm::erlang::return_type>
{
  typedef runner<gslmm::erlang::return_type> base_type;
  run_erlang()
    : base_type("erlang", 2, "a = scale parameter, b = order")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv);
    double b = get_parameter<double>(argc, argv);
    gslmm::erlang g(a, b, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_exponential : public runner<gslmm::exponential::return_type>
{
  typedef runner<gslmm::exponential::return_type> base_type;
  run_exponential()
    : base_type("exponential", 1, "mu = mean value")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double mu = get_parameter<double>(argc, argv); ;
    gslmm::exponential g(mu, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_exppow : public runner<gslmm::exponential_power::return_type>
{
  typedef runner<gslmm::exponential_power::return_type> base_type;
  run_exppow()
    : base_type("exppow", 2, 
	  "a = scale parameter, b = power (1=exponential, 2=gaussian)")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    double b = get_parameter<double>(argc, argv); ;
    gslmm::exponential_power g(a, b, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_fdist : public runner<gslmm::fdist::return_type>
{
  typedef runner<gslmm::fdist::return_type> base_type;
  run_fdist()
    : base_type("fdist", 2, "nu1, nu2 = degrees of freedom parameters")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double nu1 = get_parameter<double>(argc, argv); ;
    double nu2 = get_parameter<double>(argc, argv); ;
    gslmm::fdist g(nu1, nu2, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_flat : public runner<gslmm::flat::return_type>
{
  typedef runner<gslmm::flat::return_type> base_type;
  run_flat()
    : base_type("flat", 2, "a = lower limit, b = upper limit")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    double b = get_parameter<double>(argc, argv); ;
    gslmm::flat g(a, b, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_gamma : public runner<gslmm::gamma::return_type>
{
  typedef runner<gslmm::gamma::return_type> base_type;
  run_gamma()
    : base_type("gamma", 2, "a = order, b = scale")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    double b = get_parameter<double>(argc, argv); ;
    gslmm::gamma g(a, b, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_gaussian : public runner<gslmm::gaussian::return_type>
{
  typedef runner<gslmm::gaussian::return_type> base_type;
  run_gaussian()
    : base_type("gaussian", 1, "sigma = standard deviation")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double sigma = get_parameter<double>(argc, argv); ;
    gslmm::gaussian g(sigma, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_gaussian_tail : public runner<gslmm::gaussian_tail::return_type>
{
  typedef runner<gslmm::gaussian_tail::return_type> base_type;
  run_gaussian_tail()
    : base_type("gaussian_tail", 2, "a = lower limit, sigma = standard deviation")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    double sigma = get_parameter<double>(argc, argv); ;
    gslmm::gaussian_tail g(a, sigma, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_ugaussian : public runner<gslmm::gaussian::return_type>
{
  typedef runner<gslmm::gaussian::return_type> base_type;
  run_ugaussian()
    : base_type("ugaussian", 0, "unit gaussian, no parameters required")
  {}
  void exec(gslmm::generator& rg, size_t nev, int&, char**)
  {
    gslmm::gaussian g(1, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_ugaussian_tail : public runner<gslmm::gaussian_tail::return_type>
{
  typedef runner<gslmm::gaussian_tail::return_type> base_type;
  run_ugaussian_tail()
    : base_type("ugaussian_tail", 1, "a = lower limit")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    gslmm::gaussian_tail g(1, a, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_bivariate_gaussian : public runner<gslmm::bivariate_gaussian::return_type>
{
  typedef runner<gslmm::bivariate_gaussian::return_type> base_type;
  run_bivariate_gaussian()
    : base_type("bivariate_gaussian", 3, 
		"sigmax = x std.dev., sigmay = y std.dev., rho = correlation")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double sigmax = get_parameter<double>(argc, argv); ;
    double sigmay = get_parameter<double>(argc, argv); ;
    double rho = get_parameter<double>(argc, argv); ;
    gslmm::bivariate_gaussian g(sigmax, sigmay, rho, rg);
    output(g, nev);
  }
};

//___________________________________________________________
struct run_dir_2d : public runner<gslmm::dir::return_type>
{
  typedef runner<gslmm::dir::return_type> base_type;
  run_dir_2d() 
    : base_type("dir_2d", 0, "no parameters")
  {}
  void exec(gslmm::generator& rg, size_t nev, int&, char**)
  {
    gslmm::dir g(2, rg);
    output(g, nev);
  }
};

//___________________________________________________________
struct run_dir_3d : public runner<gslmm::dir::return_type>
{
  typedef runner<gslmm::dir::return_type> base_type;
  run_dir_3d() 
    : base_type("dir_3d", 0, "no parameters")
  {}
  void exec(gslmm::generator& rg, size_t nev, int&, char**)
  {
    gslmm::dir g(3, rg);
    output(g, nev);
  }
};

//___________________________________________________________
struct run_dir_nd : public runner<gslmm::dir::return_type>
{
  typedef runner<gslmm::dir::return_type> base_type;
  run_dir_nd() 
    : base_type("dir_nd", 1, "n1 = number of dimensions of hypersphere")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    int n1 = get_parameter<int>(argc, argv); ;
    gslmm::dir g(n1, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_geometric : public runner<gslmm::geometric::return_type>
{
  typedef runner<gslmm::geometric::return_type> base_type;
  run_geometric()
    : base_type("geometric", 1, "p = bernoulli trial probability of success")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double p = get_parameter<double>(argc, argv); ;
    gslmm::geometric g(p, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_gumbel1 : public runner<gslmm::gumbel::return_type>
{
  typedef runner<gslmm::gumbel::return_type> base_type;
  run_gumbel1()
    : base_type("gumbel1", 2, "a = order, b = scale parameter")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    double b = get_parameter<double>(argc, argv); ;
    gslmm::gumbel g(1, a, b, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_gumbel2 : public runner<gslmm::gumbel::return_type>
{
  typedef runner<gslmm::gumbel::return_type> base_type;
  run_gumbel2()
    : base_type("gumbel2", 2, "a = order, b = scale parameter")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    double b = get_parameter<double>(argc, argv); ;
    gslmm::gumbel g(2, a, b, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_hypergeometric : public runner<gslmm::hypergeometric::return_type>
{
  typedef runner<gslmm::hypergeometric::return_type> base_type;
  run_hypergeometric()
    : base_type("hypergeometric", 3, 
		"n1 = tagged population, n2 = untagged population, "
		"t = number of trials")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    int n1 = get_parameter<int>(argc, argv); ;
    int n2 = get_parameter<int>(argc, argv); ;
    int t = get_parameter<int>(argc, argv); ;
    gslmm::hypergeometric g(n1, n2, t, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_laplace : public runner<gslmm::laplace::return_type>
{
  typedef runner<gslmm::laplace::return_type> base_type;
  run_laplace()
    : base_type("laplace", 1, "a = scale parameter")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    gslmm::laplace g(a, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_landau : public runner<gslmm::landau::return_type>
{
  typedef runner<gslmm::landau::return_type> base_type;
  run_landau()
    : base_type("landau", 0, "no arguments required")
  {}
  void exec(gslmm::generator& rg, size_t nev, int&, char**)
  {
    gslmm::landau g(rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_levy : public runner<gslmm::levy::return_type>
{
  typedef runner<gslmm::levy::return_type> base_type;
  run_levy()
    : base_type("levy", 2, "c = scale, a = power (1=cauchy, 2=gaussian)")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double c = get_parameter<double>(argc, argv); ;
    double a = get_parameter<double>(argc, argv); ;
    gslmm::levy g(c, a, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_levy_skew : public runner<gslmm::levy_skew::return_type>
{
  typedef runner<gslmm::levy_skew::return_type> base_type;
  run_levy_skew()
    : base_type("levy_skew", 3, 
		"c = scale, a = power (1=cauchy, 2=gaussian), b = skew")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double c = get_parameter<double>(argc, argv); ;
    double a = get_parameter<double>(argc, argv); ;
    double b = get_parameter<double>(argc, argv); ;
    gslmm::levy_skew g(c, a, b, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_logarithmic : public runner<gslmm::logarithmic::return_type>
{
  typedef runner<gslmm::logarithmic::return_type> base_type;
  run_logarithmic()
    : base_type("logarithmic", 1, "p = probability")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double p = get_parameter<double>(argc, argv); ;
    gslmm::logarithmic g(p, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_logistic : public runner<gslmm::logistic::return_type>
{
  typedef runner<gslmm::logistic::return_type> base_type;
  run_logistic()
    : base_type("logistic", 1, "a = scale parameter")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    gslmm::logistic g(a, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_lognormal : public runner<gslmm::lognormal::return_type>
{
  typedef runner<gslmm::lognormal::return_type> base_type;
  run_lognormal()
    : base_type("lognormal", 2, "zeta = location parameter, sigma = scale parameter")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double zeta = get_parameter<double>(argc, argv); ;
    double sigma = get_parameter<double>(argc, argv); ;
    gslmm::lognormal g(zeta, sigma, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_negative_binomial : public runner<gslmm::negative_binomial::return_type>
{
  typedef runner<gslmm::negative_binomial::return_type> base_type;
  run_negative_binomial()
    : base_type("negative_binomial", 2, "p = probability, a = order")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double p = get_parameter<double>(argc, argv); ;
    double a = get_parameter<double>(argc, argv); ;
    gslmm::negative_binomial g(p, a, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_pareto : public runner<gslmm::pareto::return_type>
{
  typedef runner<gslmm::pareto::return_type> base_type;
  run_pareto()
    : base_type("pareto", 2, "a = power, b = scale parameter")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    double b = get_parameter<double>(argc, argv); ;
    gslmm::pareto g(a, b, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_pascal : public runner<gslmm::pascal::return_type>
{
  typedef runner<gslmm::pascal::return_type> base_type;
  run_pascal()
    : base_type("pascal", 2, "p = probability, n = order (integer)")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double p = get_parameter<double>(argc, argv); ;
    int N = get_parameter<int>(argc, argv); ;
    gslmm::pascal g(p, N, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_poisson : public runner<gslmm::poisson::return_type>
{
  typedef runner<gslmm::poisson::return_type> base_type;
  run_poisson()
    : base_type("poisson", 1, "mu = scale parameter")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double mu = get_parameter<double>(argc, argv); ;
    gslmm::poisson g(mu, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_rayleigh : public runner<gslmm::rayleigh::return_type>
{
  typedef runner<gslmm::rayleigh::return_type> base_type;
  run_rayleigh()
    : base_type("rayleigh", 1, "sigma = scale parameter")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double sigma = get_parameter<double>(argc, argv); ;
    gslmm::rayleigh g(sigma, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_rayleigh_tail : public runner<gslmm::rayleigh_tail::return_type>
{
  typedef runner<gslmm::rayleigh_tail::return_type> base_type;
  run_rayleigh_tail()
    : base_type("rayleigh_tail", 2, "a = lower limit, sigma = scale parameter")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    double sigma = get_parameter<double>(argc, argv); ;
    gslmm::rayleigh_tail g(a, sigma, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_tdist : public runner<gslmm::tdist::return_type>
{
  typedef runner<gslmm::tdist::return_type> base_type;
  run_tdist()
    : base_type("tdist", 1, "nu = degrees of freedom")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double nu = get_parameter<double>(argc, argv); ;
    gslmm::tdist g(nu, rg);
    output(g, nev);
  }
};
//___________________________________________________________
struct run_weibull : public runner<gslmm::weibull::return_type>
{
  typedef runner<gslmm::weibull::return_type> base_type;
  run_weibull()
    : base_type("weibull", 2, "a = scale parameter, b = exponent")
  {}
  void exec(gslmm::generator& rg, size_t nev, int& argc, char** argv)
  {
    double a = get_parameter<double>(argc, argv); ;
    double b = get_parameter<double>(argc, argv); ;
    gslmm::weibull g(a, b, rg);
    output(g, nev);
  }
};

//____________________________________________________________________  
typedef std::list<runner_base*> dist_list;
static dist_list dists;

void init() 
{
  dists.push_back(new run_bernoulli);
  dists.push_back(new run_beta);
  dists.push_back(new run_binomial);
  dists.push_back(new run_cauchy);
  dists.push_back(new run_chisq);
  dists.push_back(new run_erlang);
  dists.push_back(new run_exponential);
  dists.push_back(new run_exppow);
  dists.push_back(new run_fdist);
  dists.push_back(new run_flat);
  dists.push_back(new run_gamma);
  dists.push_back(new run_gaussian);
  dists.push_back(new run_gaussian_tail);
  dists.push_back(new run_ugaussian);
  dists.push_back(new run_ugaussian_tail);
  dists.push_back(new run_bivariate_gaussian);
  dists.push_back(new run_dir_2d);
  dists.push_back(new run_dir_3d);
  dists.push_back(new run_dir_nd);  
  dists.push_back(new run_geometric);
  dists.push_back(new run_gumbel1);
  dists.push_back(new run_gumbel2);
  dists.push_back(new run_hypergeometric);
  dists.push_back(new run_laplace);
  dists.push_back(new run_landau);
  dists.push_back(new run_levy);
  dists.push_back(new run_levy_skew);
  dists.push_back(new run_logarithmic);
  dists.push_back(new run_logistic);
  dists.push_back(new run_lognormal);
  dists.push_back(new run_negative_binomial);
  dists.push_back(new run_pareto);
  dists.push_back(new run_pascal);
  dists.push_back(new run_poisson);
  dists.push_back(new run_rayleigh);
  dists.push_back(new run_rayleigh_tail);
  dists.push_back(new run_tdist);
  dists.push_back(new run_weibull);
}

runner_base* find_dist(const std::string& name) 
{
  runner_base* d = 0;
  for (dist_list::iterator i = dists.begin();  i != dists.end(); ++i) 
    if (name == (*i)->_name) d = (*i); 
  if (!d) {
    std::string err("unknown distribution '");
    err.append(name);
    err.append("'");
    throw std::runtime_error(err.c_str());
  }
  return d;
}

namespace optionmm 
{
  typedef basic_option<double,true,false> double_option;
  typedef basic_option<size_t,true,false> size_option;
  typedef basic_option<unsigned long int,true,false> ulong_option;
}


//____________________________________________________________________
int
main (int argc, char *argv[])
{
  try {
    // Initialize list of random number distributions 
    init();

    optionmm::command_line cl("gslmm-randist",  VERSION, 
			      "(C) 2004 Christian Holm <cholm@nbi.dk>", 
			      " -s SEED -n SIZE -d DIST [PARAM ...] ",
			      argc, argv);
    optionmm::ulong_option  seed_option('s', "seed", "sets SEED", 0);
    optionmm::size_option   size_option('n', "size", "sets SIZE", 10000);
    optionmm::string_option dist_option('d', "distribution", "sets DIST", 
					"gaussian");
    optionmm::string_option g_option('g', "generator", "sets GENERATOR", 
				     "");
    optionmm::bool_option   list_option('\0', "list-dists", 
					"Show distributions known", false);
    optionmm::bool_option   param_option('\0', "list-params", 
					 "Show distribution parameters", false);
    optionmm::bool_option   gen_option('\0', "list-generators", 
					"Show generators known", false);
    cl.add(seed_option);
    cl.add(size_option);
    cl.add(dist_option);
    cl.add(g_option);
    cl.add(list_option);
    cl.add(param_option);
    cl.add(gen_option);

    if (!cl.process()) throw std::runtime_error("bad command line");
    if (cl.help()) {
      std::cout << "Generates SIZE samples from the distrubution DIST\n"
		<< "with (optional) parameters PARAMS"
		<< std::endl;
      return 0;
    }
    if (list_option.value()) {
      std::cout << "Valid distributions are: " << std::endl;
      for (dist_list::iterator i = dists.begin(); i != dists.end(); ++i) 
	std::cout << "\t" << (*i)->_name << std::endl;
      return 0;
    }
    if (param_option.value()) {
      runner_base* d = find_dist(dist_option.value());
      std::cout << "Parameters for distribution '"  
		<< dist_option.value() << "' are " << std::endl
		<< "\t" << d->_doc << std::endl;
      return 0;
    }
    if (gen_option.value()) {
      gslmm::generator::backend_type_list& g = gslmm::generator::types();
      std::cout << "Valid generators are: " << std::endl;
      for (gslmm::generator::backend_type_list::const_iterator i = g.begin();
	   i != g.end(); ++i) 
	std::cout << "\t" << (*i)->name << std::endl;
      return 0;
    }
    cl.version();

    argv++;
    argc--;

    //__________________________________________________________________
    gslmm::generator::env_setup();
    if (seed_option.value() != 0 && 
	gslmm::generator::default_seed() != 0)
      std::cout << "overriding GSL_RNG_SEED with command line value, seed = "
		<< seed_option.value() << std::endl;
    
    if (g_option.value().size() != 0 &&
	gslmm::generator::default_backend() != gslmm::generator::mt19937)
      std::cout << "overriding GSL_RNG_TYPE with command line value, type = "
		<< g_option.value() << std::endl;

    gslmm::generator g(g_option.value());
    g.set(seed_option.value());

    cl.version();
    
    //__________________________________________________________________
    runner_base* d = find_dist(dist_option.value());
    d->operator()(g, size_option.value(), argc, argv);
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0 ;
}

//__________________________________________________________________
//
// EOF
//
