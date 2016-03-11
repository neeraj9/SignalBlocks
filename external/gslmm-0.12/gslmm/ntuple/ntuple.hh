//
// $Id: ntuple.hh,v 1.11 2004-06-01 13:43:18 cholm Exp $ 
//  
//  gslmm::ntuple
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
#ifndef GSLMM_ntuple
#define GSLMM_ntuple
#ifndef GSLMM_histogram_1d
#include <gslmm/histogram/histogram_1d.hh>
#endif
#ifndef GSLMM_histogram_2d
#include <gslmm/histogram/histogram_2d.hh>
#endif
#ifndef __GSL_NTUPLE_H__
#include <gsl/gsl_ntuple.h>
#endif
#ifndef __STRING__
#include <string>
#endif

/** @file   ntuple/ntuple.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  ntuple class */

/** @defgroup ntuple N-tuples */

namespace gslmm 
{
  //@{
  /// @name ntuple helper functions. 
  //__________________________________________________________________
  /** Helper function to evalute a Value object for projections of
      N-tuple data.  The first template parameter @a Data is  the data
      type, and the second @a Value the class to do the evaluation.
      The class must define the method @c double @c
      Value:operator()(Data&) that evaluates to the value to
      histogram.  
      @param data_ptr A pointer to an N-tuple row. 
      @param param_ptr A pointer to a @a Value object. 
      @return The @a Value object evaluated at @a data_ptr.
      @ingroup ntuple 
  */ 
  template <typename Data, typename Value>
  inline double ntuple_value(void* data_ptr, void* param_ptr) 
  {
    Data*  data  = static_cast<Data*>(data_ptr);
    Value* value = static_cast<Value*>(param_ptr);
    return value->operator()(*data);
  }
  //__________________________________________________________________
  /** Helper function to select rows for projections of N-tuple data. 
      First template parameter @a Data is the data type.  Second
      template parameter @a Selector is the row selector class.   The
      class must define the method 
      @c bool @c Selector:operator()(Data&) that evaluates to the
      value to histogram.  
      @param data_ptr A pointer to an N-tuple row. 
      @param param_ptr A pointer to a @a Selector object. 
      @return true if row is to be included in projection, false
      otherwise. 
      @ingroup ntuple 
  */
  template <typename Data, typename Selector>
  inline int ntuple_select(void* data_ptr, void* param_ptr) 
  {
    Data*     data     = static_cast<Data*>(data_ptr);
    Selector* selector = static_cast<Selector*>(param_ptr);
    int ret = (selector->operator()(*data) ? 1 : 0);
    return ret;
  }
  //__________________________________________________________________
  /** Helper function to evalute a Value object for projections of
      N-tuple data. 
      First template parameter @a Data is the data type.  Second
      template parameter @a Value the class to do the evaluation.  The
      class must define the method @c double @c Value::value(Data&)
      that evaluates to the value to histogram. 
      @param data_ptr A pointer to an N-tuple row. 
      @param param_ptr A pointer to a @a ValueSelector object. 
      @return The @a Value object evaluated at @a data_ptr.
      @ingroup ntuple 
  */
  template <typename Data, typename ValueSelector>
  inline double ntuple_sv_value(void* data_ptr, void* param_ptr) 
  {
    Data*          data  = static_cast<Data*>(data_ptr);
    ValueSelector* value = static_cast<ValueSelector*>(param_ptr);
    return value->value(*data);
  }
  //__________________________________________________________________
  /** Helper function to select rows for projections of N-tuple data. 
      First template parameter @a Data is the data type.  Second
      template parameter @a ValueSelector is the row selector class.
      The class must define the method @c bool @c
      ValueSelector:select()(Data&) that evaluates to the value to
      histogram.  
      @param data_ptr A pointer to an N-tuple row. 
      @param param_ptr A pointer to a @a ValueSelector object. 
      @return true if row is to be included in projection, false
      otherwise. 
      @ingroup ntuple 
  */
  template <typename Data, typename ValueSelector>
  inline int ntuple_sv_select(void* data_ptr, void* param_ptr) 
  {
    Data*          data     = static_cast<Data*>(data_ptr);
    ValueSelector* selector = static_cast<ValueSelector*>(param_ptr);
    return selector->select(*data) ? 1 : 0;
  }
  //@}

  //__________________________________________________________________
  /** @class ntuple ntuple.hh <gslmm/ntuple/ntuple.hh>
      @brief An N-tuple class. 
      
      One needs to define some data structure to hold that represents
      the data stored by the ntuple.  Note, that the data should be
      plain old data (POD) - that is, no pointers or references.
      Class members are OK, as long as they contain only POD.   An
      example of the data structure could be. 
      
      @dontinclude ntuple-ex.cc 
      @skip struct data
      @until };

      Here's an example of a function that writes data of the above
      type to an ntuple. 

      First, we make an object of the above type, and pass that to the
      constructor of an @c gslmm::ntuple<data> type.  Note the second
      parameter @c gslmm::ntuple<data>::create that says we need a new
      file, and that we're writting to it. 
      @skip void out()  
      @until gslmm::ntuple<data>
      
      Then we make a random number generator, and a probability
      distribution function to use for filling the ntuple object. 
      @skip gslmm::generator::env_setup();
      @until gslmm::gaussian g(1, rg);
      
      Then we fill the ntuple with 10000 random @f$(x,y,z)@f$ points,
      all gaussian distributed. 
      @skip for 
      @until }
      
      And finally, we close the ntuple (and underlying file) 
      @until }

      Here's an example of reading back the ntuple, and filling a 1D
      histrogram with the values of some function of the data in the
      ntuple that fulfills a certain criteria.  

      First we define our criteria (or cut), by declaring a functor
      called @c selector.  The function operator evaluates 
      @f$ e^2_i = x^2_i + y^2_i + z^2_i@f$ for the @f$ i@f$ row in the
      ntuple, and returns @c true if @f$ e^2_i > c@f$, where @f$ c@f$
      is the number passed to the constructor of the selection object.  
      @skip struct selector 
      @until }; 

      Second we define a functor that evalutes the data row in the
      ntuple to some single number.  In this case, the function
      operator returns @f$ e^2_i = x^2_i + y^2_i + z^2_i@f$ for the
      @f$ i@f$ row of the ntuple. 
      @skip struct value
      @until }; 

      The following function uses these two functors to loop through a
      ntuple and histogram @f$ e^2_i = x^2_i + y^2_i + z^2_i@f$ for
      the @f$ i@f$ row of the ntuple, when @f$ e^2_i > c@f$, where 
      @f$ c@f$ is the parameter passed to the 
      function.  

      First, we declare an object of our data type, and pass that to
      the constructor of an @c gslmm::ntuple<data> type.  Note the
      second parameter @c gslmm::ntuple<data>::open that says we
      need a exisiting file, and that we're reading from it.
      @skip void in(float cut) 
      @until gslmm::ntuple<data> 
      
      Next, we declare a 1D histogram, an make objects of our
      selection and value functors
      @until selector s(cut);

      Then we project @f$ e^2_i = x^2_i + y^2_i + z^2_i@f$ into the
      histogram if @f$ e^2_i > c@f$
      @until n.project(h, v, s);

      Finally we print the resulting histogram to the standard output
      stream, and close the ntuple. 
      @until }

      @param Data The data type to store in the rows of the ntuple.
      The class should be POD (plain-old-data), that is, it should not
      contain pointers, have non-POD base classes, and so on. 
      @param Size The size of a @a Data object.  Per default this is
      just @c sizeof(Data), but if the class has some non-trivial
      members, then this argument can be used to reserve additional
      space in the rows. 
      @ingroup ntuple 
  */
  template <typename Data, int Size=sizeof(Data)>
  class ntuple 
  {
  public:
    /// the data type of the rows in the N-tuple
    typedef Data data_type;
    /// Modes of constructing an N-tuple object
    enum {
      /// Create a new N-tuple file, flushing any existing file.
      create, 
      /// Open an existing N-tuple file
      open
    };
  private:
    /// The N-tuple
    gsl_ntuple* _ntuple;
    /// The current data row. 
    data_type* _data;
    /// The size of the data 
    const int _size;
    /// Protected copy ctor
    ntuple(const ntuple<Data,Size>& other) {}
    /// Protected assignment operator 
    ntuple<Data,Size>& operator=(const ntuple<Data,Size>&) { return *this; }
  public:
    /** Constructor. 
	@param filename The N-tuple file name 
	@param data Pointer to the data object to read from/to 
	@param mode How to open the file. */
    ntuple(const std::string& filename, data_type* data, int mode=create);
    /** Destructor.  Flushes file to disk. */ 
    virtual ~ntuple();
    /** Read one row into memory. 
	
        This member function can be used in a loop like 
	@code
	while((d = nt.read())) { ... }
	@endcode
	@return 0 if end-of-file or some error was encountered, other
	wise a pointer to the data row read. */
    data_type* read();
    /** Add a new row to the N-tuple.  The data stored in the argument
	passed to the constructor is writen to disk. For example 
	@code 
	data d; 
        gslmm::ntuple<data> nt("particles.dat", d);
	for (int i = 0; i < n; i++) { 
	  d.x = ...
	  d.y = ...
	  d.z = ... 
	  nt.fill();
	}
	@endcode
	@return true if successful, false otherwise. */
    bool fill();
    /** Close the N-tuple file, an possible flush to disk. 
        @return true on success. */
    bool close();
    /** Project some data in to a 1D histogram.  The user must supply
	two classes: @a Value and @a Selector - both must define the
	member function @c operator()(Data&). Rows for which 
	@c Selector::operator()(Data&) return true are accepted and
	the (@c double) value returned by @c Value::operator()(Data&) 
	is histogrammed. 
	An example could be: 
	@code 
	struct unit_circle { 
	  bool operator(const data& d) const { 
	    return (d.x*d.x + d.y*d.y + d.z*d.z) <= 1;
	  }
	};
	struct radius { 
	  double operator(const data& d) const { 
	    return sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
	  }
	};

	gslmm:::histogram<double,size_t> h(100,0,1);
	radius r;
	unit_circle c;
	nt.project(h, r, c);
	@endcode
	@param h The histogram to fill. 
	@param v An object of type @a Value. 
	@param s An object of type @a Selector. 
	@return true if projection was successful. false otherwise. */
    template <typename Value, typename Selector>
    bool project(histogram<double,size_t>& h, Value& v, Selector& s);
    /** Project some data in to a 1D histogram.  The user must supply
	the classes @a ValueSelector - which must define the
	member functions @c select(Data&) and @c value(Data&). Rows
	for which  @c ValueSelector::select(Data&) returns true are
	accepted and the (@c double) value returned by 
	@c ValueSelector::value(Data&) is histogrammed. 
	An example could be: 
	@code 
	struct unit_circle { 
	  bool select(const data& d) const { 
	    return (d.x*d.x + d.y*d.y + d.z*d.z) <= 1;
	  }
	  double value(const data& d) const { 
	    return sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
	  }
	};

	gslmm:::histogram<double,size_t> h(100,0,1);
	unit_circle c;
	nt.project(h, c);
	@endcode
	@param h The histogram to fill. 
	@param vs An object of type @a ValueSelector. 
	@return true if projection was successful. false otherwise. */
    template <typename ValueSelector>
    bool project(histogram<double,size_t>& h, ValueSelector& vs);
    /** Project some data into a 2D histogram. 
	The member function is just like the 1D case, only that 
	the @a Value class member function @c operator()(Data&)
	must return a std::pair<double,double>. 
	@param h The histogram to project into 
	@param v The @a Value functor 
	@param s The row @a Selector functor. 
	@return true on success, false otherwise. */
    template <typename Value, typename Selector>
    bool project(histogram<std::pair<double,double>, 
                           std::pair<size_t,size_t> >& h,
		 Value& v, Selector& s);

    bool is_open() const
    {
      return (_ntuple ? true : false);
    }
    
  };
  //__________________________________________________________________
  template <typename Data, int Size>
  ntuple<Data,Size>::ntuple(const std::string& filename, data_type* data, 
			    int mode) 
    : _ntuple(0), _data(data), _size(Size)
  {
    if (mode == open) 
      _ntuple = 
	gsl_ntuple_open(const_cast<char*>(filename.c_str()), 
			static_cast<void*>(_data), _size);
    else 
      _ntuple = 
	gsl_ntuple_create(const_cast<char*>(filename.c_str()), 
			  static_cast<void*>(_data), _size);
  }
  //__________________________________________________________________
  template <typename Data, int Size>
  ntuple<Data,Size>::~ntuple()
  {
    close();
  }
  //__________________________________________________________________
  template <typename Data, int Size>
  typename ntuple<Data,Size>::data_type*
  ntuple<Data,Size>::read() 
  {
    if (!_ntuple) return 0;
    if (gsl_ntuple_read(_ntuple) != GSL_SUCCESS) return 0;
    _data = static_cast<data_type*>(_ntuple->ntuple_data);
    return _data;
  }
  //__________________________________________________________________
  template <typename Data, int Size>
  bool 
  ntuple<Data,Size>::fill() 
  {
    if (!_ntuple) return false;
    int ret = gsl_ntuple_write(_ntuple);
    return ret == GSL_SUCCESS;
  }
  //__________________________________________________________________
  template <typename Data, int Size>
  bool 
  ntuple<Data,Size>::close() 
  {
    if (!_ntuple) return false;
    int ret = gsl_ntuple_close(_ntuple);
    _ntuple = 0;
    return ret == GSL_SUCCESS;
  }
  //__________________________________________________________________
  template <typename Data, int Size>
  template <typename Value, typename Select>  
  bool 
  ntuple<Data,Size>::project(histogram<double,size_t>& h, 
			     Value& v, Select& s) 
  {
    if (!_ntuple) return false;
    gsl_ntuple_select_fn select;
    select.function = &ntuple_select<data_type,Select>;
    select.params   = static_cast<void*>(&s);
    gsl_ntuple_value_fn value;
    value.function  = &ntuple_value<data_type,Value>;
    value.params    = static_cast<void*>(&v);
    return gsl_ntuple_project(h._histogram,_ntuple, &value, &select) 
      == GSL_SUCCESS;
  }
  //__________________________________________________________________
  template <typename Data, int Size>
  template <typename ValueSelector>  
  bool 
  ntuple<Data,Size>::project(histogram<double,size_t>& h, 
			     ValueSelector& s) 
  {
    if (!_ntuple) return false;
    gsl_ntuple_select_fn select;
    select.function = &ntuple_sv_select<data_type,ValueSelector>;
    select.params   = static_cast<void*>(&s);
    gsl_ntuple_value_fn value;
    value.function  = &ntuple_sv_value<data_type,ValueSelector>;
    value.params    = static_cast<void*>(&s);
    return gsl_ntuple_project(h._histogram,_ntuple, &value, &select)
      == GSL_SUCCESS;
  }
  //__________________________________________________________________
  template <typename Data, int Size>
  template <typename Value, typename Selector>  
  bool 
  ntuple<Data,Size>::project(histogram<std::pair<double,double>,
			     std::pair<size_t,size_t> >& h, 
			     Value& v, Selector& s) 
  {
    if (!_ntuple) return false;
    do {
      if (!read())   break;
      if (s(*_data)) h.fill(v(*_data));
    } while(true);
    return true;
  }
}
    
#endif
//____________________________________________________________________
//
// EOF
//
