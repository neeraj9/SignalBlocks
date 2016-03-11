//
// $Id: annealing.hh,v 1.3 2006-04-28 23:23:48 cholm Exp $ 
//  
//  gslmm::annealing
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
#ifndef GSLMM_annealing_annealing
#define GSLMM_annealing_annealing
#ifndef GSLMM_random_generator
# include <gslmm/random/generator.hh>
#endif
#ifndef __IOSTREAM__
# include <iostream>
#endif
#ifndef __GSL_SIMAN
# include <gsl/gsl_siman.h>
#endif

/** @file   annealing/annealing.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  annealing class */

/** @defgroup annealing Simulated Annealing */
namespace gslmm
{    
  /** Default printer for annealing 
      @ingroup annealing
   */
  template <typename T>
  struct annealing_printer 
  {
    void 
    operator()(const T& v) 
    {
      std::cout << v << std::endl;
    }
  };
  /** @brief Perform a simulated annealing. 
      
      Stochastic search techniques are used when the structure of a
      space is not well understood or is not smooth, so that
      techniques like Newton's method (which requires calculating
      Jacobian derivative matrices) cannot be used. In particular,
      these techniques are frequently used to solve combinatorial
      optimization problems, such as the traveling salesman problem.

      The goal is to find a point in the space at which a real valued
      @e energy @e function (or @e cost @e function) is minimized.
      Simulated annealing is a minimization technique which has given
      good results in avoiding local minima; it is based on the idea
      of taking a random walk through the space at successively lower
      temperatures, where the probability of taking a step is given by
      a Boltzmann distribution. 

      The simulated annealing algorithm takes random walks through the
      problem space, looking for points with low energies; in these
      random walks, the probability of taking a step is determined by
      the Boltzmann distribution,
      @f[
      p = e^{-(E_{i+1} - E_i)/(kT)}
      @f] 
      if @f$ E_{i+1} > E_i@f$, and @f$ p = 1@f$ when @f$ E_{i+1} \leq
      E_i@f$. 

      In other words, a step will occur if the new energy is lower.
      If the new energy is higher, the transition can still occur, and
      its likelihood is proportional to the temperature @f$ T@f$ and
      inversely proportional to the energy difference @f$ E_{i+1} -
      E_i@f$. 

      The temperature @f$ T@f$ is initially set to a high value, and a
      random walk is carried out at that temperature.  Then the
      temperature is lowered very slightly according to a @e cooling
      @e schedule, for example: @f$ T \rightarrow T/\mu_T@f$ where @f$
      \mu_T@f$ is slightly greater than 1.

      The slight probability of taking a step that gives higher energy
      is what allows simulated annealing to frequently get out of
      local minima. 

      Below follows a simple example.  In this example, in one
      dimensional cartesian space, we set up an energy function which
      is a damped sine wave; this has many local minima, but only one
      global minimum, somewhere between 1 and 1.5.  The initial
      guess given is 15.5, which is several local minima away from the
      global minimum.

      Printer of state 
      @dontinclude annealing-ex.cc 
      @skip struct 
      @until };

      Derived class that sets up the solver
      @skip struct 
      @until };
      
      Now the program
      @skip main
      @until }

      Below are two plots from this run 
      @image html annealing-ex-t.png 
      @image html annealing-ex-e.png 

      These where made like 
      @verbatim 
      ./annealing-ex | grep -v "#" | \
         awk 'BEGIN {FS=" "} {printf "%g %g\n", $1, $3 }'| \
         graph -T X -X "Generation" -Y "T" -C 
      ./annealing-ex | grep -v "#" | \
         awk 'BEGIN {FS=" "} {printf "%g %g\n", $1, $5}'| \
         graph -T X -X "Generation" -l X -Y "Energy" -C 
      @endverbatim
 
      Here's a more complicated example.  It's the TSP (@e Traveling
      @e Salesman @e Problem) is the classic combinatorial
      optimization problem.  I have provided a very simple version of
      it, based on the coordinates of twelve cities in the
      southwestern United States.  This should maybe be called the
      @e Flying @e Salesman @e Problem, since I am using the
      great-circle distance between cities, rather than the driving
      distance.  Also: I assume the earth is a sphere, so I don't use
      geoid distances.  The class routine finds a route which is
      3490.62 Kilometers long; this is confirmed by an exhaustive
      search of all possible routes with the same initial city.

      A structure that contains the city information, with a function
      to calculate the distance between two cities 
      @dontinclude annealing-tsm.cc 
      @skip struct 
      @until };

      A printer of a route.
      @skip struct 
      @until };

      Derived class that sets up the solver
      @skip struct 
      @until };
      
      Now the program
      @skip main
      @until }

      Below are some plots.

      Initial configuration 
      @image html annealing-tsm-init.png 

      Distance, as a function of the generation 
      @image html annealing-tsm-dist.png 

      Final configuration 
      @image html annealing-tsm-final.png 

      These where made doing
      @verbatim 
      ./annealing-tsm | grep -v "#" > tsm.steps
      ./annealing-tsm | grep "###" > tsm.final
      ./annealing-tsm | grep "###" > tsm.initial
      cat tsm.final | awk 'BEGIN {FS=" "} {printf "%g %g\n", $2, $3}'| \
        graph -T X -X "Longtitude"  -Y "Latitude" -C 
      cat tsm.inital | awk 'BEGIN {FS=" "} {printf "%g %g\n", $2, $3}'| \
        graph -T X -X "Longtitude"  -Y "Latitude" -C 
      cat tsm.steps | awk 'BEGIN {FS=" "} {printf "%g %g\n", $1, $17}' | \
        graph -X "Generation" -Y "Distance" -T X  -C
      @endverbatim
      @ingroup annealing
  */
  template <typename T, class Printer=annealing_printer<T>,
	    class Allocator=std::allocator<T> >
  struct annealing 
  {
  public:
    /** Value type */
    typedef T value_type;
    /** Constructor 
	@param g Random number generator */
    /** Constructor
	@param g       Random number generator 
	@param n_iter  Number of iterations per temperature
	@param minT    Minimum temperature 
	@param initT   Initial temperature 
	@param dstep   Step size 
	@param mu      Temperature damping factor 
	@param k       Boltzmanns constant */
    annealing(generator& g, unsigned int n_iter=10, 
	      double minT=1e-6, double initT=1e6, 
	      double dstep=1, double mu=1.005,
	      double k=1) 
      : _generator(g) 
    {
      _params.iters_fixed_T   = n_iter;
      _params.t_min           = minT;
      _params.t_initial       = initT;
      _params.step_size       = dstep;
      _params.mu_t            = mu;
      _params.k               = k;
      _params.n_tries         = 1;
    }
    /** @param k Boltzmanns constant */
    void set_k(double k=1)                   { _params.k             = k; }
    /** @param mu Temperature dampening factor @f$ \mu_T@f$ */
    void set_mu(double mu=1.005)             { _params.mu_t          = mu; }
    /** @param min Minimal temperature.   Algorithm stops when the
	temperature falls below this value */
    void set_minimum_temperature(double min) { _params.t_min         = min; }
    /** @param t Initial temperature of the system */
    void set_initial_temperature(double t)   { _params.t_initial     = t; }
    /** @param n Number of iterations done for each temperature */
    void set_iterations(unsigned int n)      { _params.iters_fixed_T = n; }
    /** @param size the maximum step size */
    void set_step_size(double size)          { _params.step_size     = size; }
    /** Solve the problem
	@param v     The initial value of the configuration. 
	@param debug If @c true, print out each state change (when the
	temperature change. */
    virtual void solve(T& v, bool debug=false);
  protected:
    /** Allocator type */
    typedef Allocator alloc_t;
    /** Allocator */
    alloc_t _alloc;
    /** Printer type */
    typedef Printer printer_t;
    /** Printer */
    printer_t _printer;
    /** Internal status structure */
    struct status
    {
      /** Current configuration */
      T*            _data;
      /** Pointer to master */
      annealing<T,Printer,Allocator>* _master;
    };
    /** This function type should return the energy of the
	configuration @a v 
	@param v Current configuration. 
	@return Energy of the configuration @a v */
    virtual double energy(const T& v) = 0;
    /** This function type should modify the configuration @a v using
	a random step taken from the internal generator, up to a
	maximum distance of @a step.
	@param v The configuration to update.  On entry, it's the
	current configuration.  On exit, it should be the updated
	configuration. 
	@param size Maximum step size */ 
    virtual void   step(T& v, double size) = 0;
    /** This function type should return the distance between two 
	configurations @a x and @a y.
	@param x configuration. 
	@param y configuration 
	@return distance between @a x and @a y */
    virtual double metric(const T& x, const T& y) = 0;
    /** This function type should copy the configuration @a src into
	@a dest.  The default implementation uses the normal assignment
	operator. 
	@param src Source 
	@param dest Destination. */
    virtual void copy(const T& src, T& dest);
    /** Parameters */
    gsl_siman_params_t _params;
    /** Generator */
    generator _generator;
    
    /** @{ 
	@name Interface */
    /** Evaluate the energy function at a point stored in passed
	pointer to object.  The pointer is interpreted as a pointer to
	an object of type gslmm::annealing<T>. 
	@return o->energy(o->current()); */ 
    static double annealing_energy(void* o) 
    {
      typedef typename annealing<T,Printer,Allocator>::status status_t;
      status_t* d = static_cast<status_t*>(o);
      double val = d->_master->energy(*(d->_data));
      return val;
    }
    /** Evaluate the step function at a point stored in passed pointer
	to object. The pointer is interpreted as a pointer to
	an object of type gslmm::annealing<T>. 
	@return o->energy(o->current()); */ 
    static void annealing_step(const gsl_rng* r, void* o, double step) 
    {
      typedef typename annealing<T,Printer,Allocator>::status status_t;
      status_t* d = static_cast<status_t*>(o);
      d->_master->step(*(d->_data), step);
    }
    /** Evaluate the metric function at a point stored in passed pointer
	to object. The pointer is interpreted as a pointer to
	an object of type gslmm::annealing<T>.  The second pointer is
	interpreted as a pointer to an object of type T.
	@return o->metric(o->current(), *y); */ 
    static double annealing_metric(void* x, void* y) 
    {
      typedef typename annealing<T,Printer,Allocator>::status status_t;
      status_t* xp  = static_cast<status_t*>(x);
      status_t* yp  = static_cast<status_t*>(y);
      return xp->_master->metric(*(xp->_data), *(yp->_data));
    }
    /** Print an object. */
    static void annealing_print(void* v)
    {
      typedef typename annealing<T,Printer,Allocator>::status status_t;
      status_t* d  = static_cast<status_t*>(v);
      d->_master->_printer(*(d->_data));
    }
    /** Copy an object to another. */
    static void annealing_copy_to(void* src, void* dest) 
    {
      typedef typename annealing<T,Printer,Allocator>::status status_t;
      status_t* s  = static_cast<status_t*>(src);
      status_t* d  = static_cast<status_t*>(dest);
      s->_master->copy(*(s->_data),*(d->_data));
    }
    /** Create a new copy of an object. */
    static void* annealing_copy_from(void* src) 
    {
      typedef typename annealing<T,Printer,Allocator>::status status_t;
      status_t* s  = static_cast<status_t*>(src);
      status_t* d  = new status_t;
      d->_master   = s->_master;
      d->_data     = s->_master->_alloc.allocate(1,0);
      s->_master->_alloc.construct(d->_data, *(s->_data));
      return d;
    }
    /** Copy an object to another, using the assignment operator of
	the type T */
    static void annealing_destroy(void* src) 
    {
      typedef typename annealing<T,Printer,Allocator>::status status_t;
      status_t* s  = static_cast<status_t*>(src);
      s->_master->_alloc.destroy(s->_data);
      s->_master->_alloc.deallocate(s->_data, 0);
      delete s;
    }
    /** @} */
  };
  //__________________________________________________________________
  template <typename T, class Printer, class Allocator> 
  inline void
  annealing<T,Printer,Allocator>::solve(T& v, bool debug) 
  {
    status*                    d        = new status;
    d->_master                          = this;
    d->_data                            = new T(v);
    gsl_siman_Efunc_t          energyf  = &annealing_energy;
    gsl_siman_step_t           stepf    = &annealing_step;
    gsl_siman_metric_t         metricf  = &annealing_metric;
    gsl_siman_print_t          printf   = &annealing_print;
    gsl_siman_copy_t           copyf    = &annealing_copy_to;
    gsl_siman_copy_construct_t cctorf   = &annealing_copy_from;
    gsl_siman_destroy_t        destroyf = &annealing_destroy;
    gsl_siman_solve(_generator._rng, d, energyf, stepf, metricf, 
		    (debug ? printf : 0), copyf, cctorf, destroyf, 0, _params);
    v = *(d->_data);
  }

  //__________________________________________________________________
  template <typename T, class Printer, class Allocator> 
  inline void 
  annealing<T,Printer,Allocator>::copy(const T& src, T& dest) 
  {
    dest = src;
  }  
};


  

      

#endif
//____________________________________________________________________
//
// EOF
//
