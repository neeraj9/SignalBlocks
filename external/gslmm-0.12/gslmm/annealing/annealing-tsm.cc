//
// $Id: annealing-tsm.cc,v 1.2 2006-05-09 07:40:34 cholm Exp $ 
//  
//  annealing/annealing-test.cc
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
# include <gslmm/annealing/annealing.hh>
#endif
#include <cmath>
#include <string>
#include <vector>

//____________________________________________________________________
struct city 
{
  std::string _name;
  double      _latitude;
  double      _longitude;
  double distance(const city& o) const 
  {
    // Same position. 
    if (_latitude == o._latitude && _longitude == o._longitude) return 0;
    // 6000KM approximately 
    const double earth_radius = 6375.000; 
    // sin and cos of lat and long; must convert to radians 
    const double deg2rad      = M_PI/180;
    double       sla1         = sin(_latitude  * deg2rad);
    double       cla1         = cos(_latitude  * deg2rad);
    double       slo1         = sin(_longitude * deg2rad);
    double       clo1         = cos(_longitude * deg2rad);
    double       sla2         = sin(o._latitude  * deg2rad);
    double       cla2         = cos(o._latitude  * deg2rad);
    double       slo2         = sin(o._longitude * deg2rad);
    double       clo2         = cos(o._longitude * deg2rad);
    double       x1           = cla1 * clo1;
    double       x2           = cla2 * clo2;
    double       y1           = cla1 * slo1;
    double       y2           = cla2 * slo2;
    double       z1           = sla1;
    double       z2           = sla2;
    double       dot_product  = x1 * x2 + y1 * y2 + z1 * z2;
    double       angle        = acos(dot_product);
    // distance is the angle (in radians) times the earth radius 
    return angle*earth_radius;
  }
};

//____________________________________________________________________
struct route_print
{
  void operator()(const std::vector<size_t>& v) 
  {
    printf(" [");
    for (size_t i = 0; i < v.size(); i++) printf("%3d", v[i]);
    printf("]");
  }
};

//____________________________________________________________________
struct traveling_sales_man : 
  public gslmm::annealing<std::vector<size_t>,route_print>
{
  typedef std::vector<size_t> route_t;
  typedef gslmm::annealing<route_t,route_print> base_t;
  traveling_sales_man(gslmm::generator g)
    : base_t(g, 200, 5e-1, 5000, 1, 1.002, 1) 
  {}
  void add_city(const char* name, double lat, double lng) 
  {
    city c;
    c._name      = name;
    c._latitude  = lat;
    c._longitude = lng;
    add_city(c);
  }
  void add_city(const city& c) 
  {
    _cities.push_back(c);
  }
  void init() 
  {
    // Set up distance matrix 
    size_t n   = _cities.size();
    _distances = new double[n * n];
    for (size_t i = 0; i < n; i++) 
      for (size_t j = 0; j < n; j++) 
	distance(i,j) = _cities[i].distance(_cities[j]);
    // Set-up initial condition 
    _route.resize(n);
    for (size_t i = 0; i < n; i++) _route[i] = i;
  }
  void solve(bool debug=true) 
  {
    base_t::solve(_route, debug);
  }
  double energy(const route_t& v)
  {
    // an array of integers describing the order 
    double e = 0;
    size_t n = v.size();
    for (size_t i = 0; i < n; i++) {
      e += distance(v[i], v[(i + 1) % n]);
    }
    return e;
  }
  double metric(const route_t& x, const route_t& y)
  {
    double distance = 0;
    size_t n        = std::min(x.size(), y.size());
    for (size_t i = 0; i < n; ++i) 
      distance += (x[i] == y[i] ? 0 : 1);
    return distance;
  }
  void step(route_t& v, double size)
  {
    // prevent warnings about unused parameter 
    size = 0 ; 
    // pick the two cities to swap in the matrix; we leave the first
    // city fixed 
    size_t n  = v.size();
    size_t x1 = _generator.integer(n-1) + 1;
    size_t x2;
    do {
      x2 = _generator.integer(n-1) + 1;
    } while (x2 == x1);
    std::swap(v[x1], v[x2]);
  }
  const double& distance(size_t i, size_t j) const 
  {
    size_t n = _cities.size();
    return _distances[i * n + j];
  }
  double& distance(size_t i, size_t j)
  {
    size_t n = _cities.size();
    return _distances[i * n + j];
  }
  void print_distances() const
  {
    size_t n = _cities.size();
    for (size_t i = 0; i < n; ++i) {
      printf("# ");
      for (size_t j = 0; j < n; ++j) 
	printf("%15.8f   ", distance(i,j));
      printf("\n");
    }
  }
  void print_route(const char* prefix="##") const 
  {
    size_t n = _route.size();
    double d = 0;
    for (size_t i = 0; i < n; i++) {
      double s =  distance(i % n, (i + 1) % n);
      printf("%s: %8.3f %8.3f %12.3f %12.3f \"%s\"\n",
	     prefix, -_cities[_route[i % n]]._longitude,
	     _cities[_route[i % n]]._latitude,
	      s, d,
	     _cities[_route[i % n]]._name.c_str());
      d += s;
    }
  }
protected:
  typedef std::vector<city> city_list;
  city_list _cities;
  route_t   _route;
  double*   _distances;
};

/* set up parameters for this simulated annealing run */
int main(void)
{
  gslmm::generator g;
  traveling_sales_man tsm(g);
  tsm.add_city("Santa Fe",    35.68,   105.95);
  tsm.add_city("Phoenix",     33.54,   112.07);
  tsm.add_city("Albuquerque", 35.12,   106.62);
  tsm.add_city("Clovis",      34.41,   103.20);
  tsm.add_city("Durango",     37.29,   107.87);
  tsm.add_city("Dallas",      32.79,    96.77);
  tsm.add_city("Tesuque",     35.77,   105.92);
  tsm.add_city("Grants",      35.15,   107.84);
  tsm.add_city("Los Alamos",  35.89,   106.28);
  tsm.add_city("Las Cruces",  32.34,   106.76);
  tsm.add_city("Cortez",      37.35,   108.58);
  tsm.add_city("Gallup",      35.52,   108.74);
  tsm.init();
  tsm.print_route("##");
  tsm.solve();
  tsm.print_route("###");
  return 0;
}

//____________________________________________________________________
//
// EOF
//
