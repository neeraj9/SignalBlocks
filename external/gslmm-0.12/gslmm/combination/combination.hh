//
// $Id: combination.hh,v 1.5 2004-04-13 15:10:47 cholm Exp $ 
//  
//  gslmm::combination
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
#ifndef GSLMM_combination
#define GSLMM_combination
#ifndef _GSL_COMBINATION_H_
#include <gsl/gsl_combination.h>
#endif
#ifndef GSLMM_util
#include <gslmm/gslmm-util.hh>
#endif

/** @file   combination.hh
    @author Christian Holm
    @date   Sun Mar 09 20:37:11 2003
    @brief  A combination representation */

/** @defgroup combination Combinations */
namespace gslmm
{
  /** @class combination combination.hh <gslmm/combination/combination.hh>
      A combination representation.
      @ingroup combination
   */
  class combination
  {
  protected:
    /// The low-level data 
    gsl_combination* _combination;
  public:
    /** Constructor. 
	@param n Which numbers to allow in the range @f$ [0,n)@f$
	@param k How many elements that should be in the combination
	@param first If true, initialise the lexigraphical first
	combination, otherwise, it's left uninitialised. */
    combination(size_t n, size_t k, bool first=true);
    /** Copy constructor. */
    combination(const combination& other);
    /** Destructor. */
    virtual ~combination();

    /** Returns the value of the @a i element of the
	combination.  If @a i lies outside the allowed range of 0 to
	@f$ K-1 @f$ then the error handler is invoked and 0 is
	returned. 
	@param i the element number 
	@return the @a i element. */
    size_t operator[](size_t i) const;

    /** Assignment operator */
    combination& operator=(const combination& other);

    /** Make this the first combination.  @f$(0,1,2,...,k-1)@f$ */
    void first(); 
    /** Make this the last combination. @f$(n-k,n-k+1,...,n-1)@f$ */
    void last(); 
    
    /** Get the parameter @f$ n @f$ of the combination. 
	@return the parameter @f$ n @f$ */
    size_t max() const;
    /** Get the parameter @f$ k @f$ of the combination. 
	@return the parameter @f$ k @f$ */
    size_t size() const;

    /** Check if this combination is valid. 
	@return true if the combination is valid, false otherwise. */
    bool is_valid() const;

    /** Make this the next combination.  
	@return true if a new combination was made, false if this is
	already the last. */
    bool next();
    /** Make this the previous combination.  
	@return true if a new combination was made, false if this is
	already the first. */
    bool previous();

    /** Write this object to a (C) file. 
	@param f The (C) file to write to 
	@return @c true on success, @c false otherwise 
     */
    bool write(FILE* f) const;
    /** Read into  this object from a (C) file. 
	@param f The (C) file to read from.
	@return @c true on success, @c false otherwise 
     */
    bool read(FILE* f);
    /** print to a (C) stream. 
	@param s The stream to print on. 
	@param f The format of the write.  A good format would be 
	@c "%Zu\n"  
	@return @c true on success, @c false otherwise 
    */
    bool print(FILE* s, const char* f="%Zu\n") const;
    /** Read into this object from a (C) stream.
	@param s Stream to read from. 
	@return @c true on success, @c false otherwise 
    */
    bool scan(FILE* s);
  };
  //__________________________________________________________________
  inline combination::combination(size_t n, size_t k, bool first) 
    :  _combination(0)
  {
    if (first) _combination = gsl_combination_calloc(n, k);
    else       _combination = gsl_combination_alloc(n, k); 
  }
  //__________________________________________________________________
  inline combination::combination(const combination& other) 
    :  _combination(0)
  {
    _combination = gsl_combination_alloc(other.max(),other.size());
    *this = other;
  }  
  //__________________________________________________________________
  inline combination& combination::operator=(const combination& other)
  {
    for (size_t i = 0; i < this->size() && i < other.size(); i++) 
      _combination->data[i] = other._combination->data[i];
    return *this;
  }
  //__________________________________________________________________
  inline combination::~combination() 
  {
    gsl_combination_free(_combination);
  }
  //__________________________________________________________________
  inline size_t
  combination::operator[](size_t i) const
  {
    return gsl_combination_get(_combination, i);
  }
  //__________________________________________________________________
  inline void
  combination::first()
  {
    return gsl_combination_init_first(_combination);
  }
  //__________________________________________________________________
  inline void
  combination::last()
  {
    return gsl_combination_init_last(_combination);
  }
  //__________________________________________________________________
  inline size_t
  combination::max() const
  {
    return gsl_combination_n(_combination);
  }
  //__________________________________________________________________
  inline size_t
  combination::size() const
  {
    return gsl_combination_k(_combination);
  }
  //__________________________________________________________________
  inline bool
  combination::is_valid() const
  {
    return gsl_combination_valid(_combination) == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline bool
  combination::next()
  {
    return gsl_combination_next(_combination) == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline bool
  combination::previous()
  {
    return gsl_combination_prev(_combination) == GSL_SUCCESS;
  }

  //__________________________________________________________________
  inline bool
  combination::write(FILE* f) const
  {
    int ret = gsl_combination_fwrite(f, _combination);
    return (ret == GSL_EFAILED ? false: true);
  }
  //__________________________________________________________________
  inline bool
  combination::read(FILE* f)
  {
    int ret = gsl_combination_fread(f, _combination);
    return (ret == GSL_EFAILED ? false: true);
  }
  //__________________________________________________________________
  inline bool
  combination::print(FILE* s, const char* f) const
  {
    int ret = gsl_combination_fprintf(s, _combination, f);
    return (ret == GSL_EFAILED ? false: true);
  }
  //__________________________________________________________________
  inline bool
  combination::scan(FILE* s)
  {
    int ret = gsl_combination_fscanf(s, _combination);
    return (ret == GSL_EFAILED ? false: true);
  }
  
}


#endif
//____________________________________________________________________
//
// EOF
//
