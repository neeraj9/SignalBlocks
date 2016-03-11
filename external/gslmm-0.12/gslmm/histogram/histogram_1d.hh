//
// $Id: histogram_1d.hh,v 1.13 2006-02-08 00:07:19 cholm Exp $ 
//  
//  gslmm::histogram
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
#ifndef GSLMM_histogram_1d
#define GSLMM_histogram_1d
#ifndef __VECTOR__
#include <vector>
#endif
#ifndef GSLMM_histogram_base
#include <gslmm/histogram/histogram_base.hh>
#endif
#ifndef GSLMM_util
#include <gslmm/gslmm-util.hh>
#endif
#ifndef _GSL_ERRNO_H_
#include <gsl/gsl_errno.h>
#endif
#ifndef __GSL_HISTOGRAM_H__
#include <gsl/gsl_histogram.h>
#endif
/** @file   histogram/histogram_1d.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  histogram class */

namespace gslmm 
{
  /** @brief A 1D histogram class. 
      @ingroup histogram
  */
  template<>
  class histogram<double,size_t>
  {
  public:
    /// Value type 
    typedef double value_type;
    /// Bin type
    typedef size_t bin_type;
    /// The size type 
    typedef size_t size_type;
  protected:
    /// The low level histogram
    gsl_histogram* _histogram;
    template <typename Data, int Size> friend class ntuple;
  public:
    /** Constructor.  The bin ranges is not set.  Set the bin ranges
	with the member function range.  
	@param bins the number of bins 
	@param zero is @c true, then the bins are zeroed. 
     */
    histogram(const size_type& bins, bool zero=true);
    /** Constructor. 
	Construct a histogram with @a bins bins uniformly distributed
	between @a min and @a max.  
	@param bins The number of bins. 
	@param min The minimum of the range. 
	@param max The maximum of the range. Must be larger than @a
	min. 
     */
    histogram(const size_type& bins, const value_type& min, 
	      const value_type& max);
    /** Constructor 
	Create a histogram with @a bins, with the (variable) ranges
	given by @a  ranges.  For example: 
	@code 
	double x[] = { 1, 10, 100, 1000 };
	gslmm::histogram<double>::histogram h(3, x);
	@endcode 
	This will allocate a histogram with 3 bins, distributed
	between 1 and 1000 logarithimicly 
	@param bins The number of bins
	@param ranges Array of @a bins + 1 ranges.  The limits must be
	in increasing order. 
     */
    histogram(const size_type& bins, const value_type* ranges);
    /** Constructor 
	Allocates a histogram with @f$ N@f$ bins, where @f$ N@f$ is 
	@a ranges.size() - 1.  For example:
	@code 
	std::vector<double> x(4);
	for (size_t i = 0; i < x.size(); ++i) x[i]  = pow(10,i);
	gslmm::histogram<double> h(x);
	@endcode
	This makes a histogram with the bins @f$ [1,10), [10, 100), 
	[100, 1000] @f$
	@param ranges Array of the bin limits. 
    */
    histogram(const std::vector<value_type>& ranges);
    /** Copy constructor */
    histogram(const histogram<value_type,bin_type>& other);
    /** Destructor */
    virtual ~histogram();
    
    /** Set the ranges. */
    bool range(const value_type& min, const value_type& max);
    /** Set the ranges. */
    bool range(const value_type* ranges);
    /** Set the ranges. */
    bool range(const std::vector<value_type>& ranges);
    
    /** Get the maximum of the range */
    value_type max() const;
    /** Get the minimum of the range */
    value_type min() const;
    /** Get the number of bins */
    size_type bins() const;
  
    /** Reset all bins to zero */
    void reset();

    /** Increment a bin */
    bool fill(const value_type& x);
    /** Increment a bin with a weight*/
    bool fill(const value_type& x, double w);

    /** Get the bin contents */
    double bin_content(const bin_type& bin) const;
    /** Get the bin range */
    bool bin_range(const bin_type& bin, value_type& min, 
		   value_type& max) const;
    /** Get the bin range */
    value_type bin_min(const bin_type& bin) const;
    /** Get the bin range */
    value_type bin_max(const bin_type& bin) const;
    /** Get bin number corresponding to @a x  and @a y*/
    bin_type bin_number(const value_type& x) const;
    
    /** Get the value of the maximal bin */
    double max_content() const;
    /** Get the value of the minimal bin */
    double min_content() const;
    /** Get the bin number of the maximal bin */
    bin_type max_content_bin() const;
    /** Get the bin number of the minimal bin */
    bin_type min_content_bin() const;
    
    /** Get the mean of the histogram */
    value_type mean() const;
    /** Get the spread of the histogram */
    value_type sigma() const;
    /** Get x,y covariance */
    double covariance() const;
    /** Get the sum of the histogram */
    double sum() const;
    
    /** Check if to histograms have compatible ranges. */
    bool compatible(const histogram<value_type,bin_type>& other);
    
    /** Bin content */
    const value_type& operator[](const bin_type& bin) const;
    /** Bin content */
    value_type& operator[](const bin_type& bin);
    /** Assign one histogram to another. */
    histogram<value_type,bin_type>& 
    operator=(const histogram<value_type,bin_type>& other);
    /** Add the contents of a compatible histogram */
    histogram<value_type,bin_type>& 
    operator+=(const histogram<value_type,bin_type>& other);
    /** Subtract the contents of a compatible histogram */
    histogram<value_type,bin_type>& 
    operator-=(const histogram<value_type,bin_type>& other);
    /** Multiply by the contents of a compatible histogram */
    histogram<value_type,bin_type>& 
    operator*=(const histogram<value_type,bin_type>& other);
    /** Divide with the contents of a compatible histogram */
    histogram<value_type,bin_type>& 
    operator/=(const histogram<value_type,bin_type>& other);

    /** Add a constant to the bins. */
    histogram<value_type,bin_type>& operator+=(value_type x);
    /** Subtract a constant from this histogram */
    histogram<value_type,bin_type>& operator-=(value_type x);    
    /** Scale this histogram with a constant */
    histogram<value_type,bin_type>& operator*=(value_type x);
    /** Scale this histogram with a constant */
    histogram<value_type,bin_type>& operator/=(value_type x);
    
    /** @brief Probablity distribution function from a 1D histogram 
	@ingroup histogram 
    */
    class pdf 
    {
    protected:
      /// The low-level type 
      gsl_histogram_pdf* _pdf;
    public:
      /// Value type 
      typedef double value_type;
      /// Bin type
      typedef size_t bin_type;
      /// The size type 
      typedef size_t size_type;
      /** Constrcutor 
	  @param size Number of enries in corresponding histogram. */
      pdf(const size_type& size);
      /** Constructor 
	  @param h Histogram to make this PDF from. */
      pdf(const histogram<value_type,bin_type>& h);
      /** Copy constructor */ 
      pdf(const pdf& other);
      /** Assignment operator */
      pdf& operator=(const pdf& other);
      /** Destructor. */
      virtual ~pdf();
      /** Initialise this PDF with a histogram.
	  @param h Histogram to initialise with. */
      void init(const histogram<value_type,bin_type>& h);
      /** Draw a number from the PDF. 
	  @param r a uniform deviate in [0,1] 
	  @return a number drawn from the PDF. */
      value_type sample(const double& r) const;
    };
    friend class pdf;

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
	@param r The format of the write for the ranges. 
	@param b The format of the write for the bins. 
	@return @c true on success, @c false otherwise 
    */
    bool print(FILE* s, const char* r="%g", const char* b="%f") const;
    /** Read into this object from a (C) stream.
	@param s Stream to read from. 
	@return @c true on success, @c false otherwise 
    */
    bool scan(FILE* s);
  };

  
  //__________________________________________________________________
  inline 
  histogram<double,size_t>::histogram(const size_type& bins, bool zero) 
    : _histogram(0)
  {
    if (zero) _histogram = gsl_histogram_calloc(bins);
    else      _histogram = gsl_histogram_alloc(bins);
  }
  //__________________________________________________________________
  inline 
  histogram<double,size_t>::histogram(const size_type& bins, 
				      const value_type& min, 
				      const value_type& max) 
    : _histogram(0)
  {
    _histogram = gsl_histogram_calloc_uniform(bins,min,max);
  }  
  //__________________________________________________________________
  inline 
  histogram<double,size_t>::histogram(const std::vector<value_type>& ranges)
    : _histogram(0)
  {
    value_type* axis = const_cast<value_type*>(&(ranges[0]));
    _histogram = gsl_histogram_calloc_range(ranges.size()-1,axis);
  }  
  //__________________________________________________________________
  inline 
  histogram<double,size_t>::histogram(const size_type& bins, 
				      const value_type* ranges)
    : _histogram(0)
  {
    value_type* axis = const_cast<value_type*>(ranges);
    _histogram = gsl_histogram_calloc_range(bins-1,axis);
  }  
  //__________________________________________________________________
  inline 
  histogram<double,size_t>::histogram(const histogram<value_type,bin_type>& 
				      other) 
    : _histogram(0)
  {
    _histogram = gsl_histogram_clone(other._histogram);
  }
  //__________________________________________________________________
  inline 
  histogram<double,size_t>::~histogram() 
  {
    gsl_histogram_free(_histogram);
  }
  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::range(const value_type& min, const value_type& max)
  {
    return gsl_histogram_set_ranges_uniform(_histogram,min,max) 
      == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::range(const value_type* ranges)
  {
    value_type* axis = const_cast<value_type*>(ranges);
    return gsl_histogram_set_ranges(_histogram,axis,bins())
      == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::range(const std::vector<value_type>& ranges)
  {
    value_type* axis = const_cast<value_type*>(&(ranges[0]));
    return gsl_histogram_set_ranges(_histogram,axis,
					     ranges.size())
      == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::value_type
  histogram<double,size_t>::max() const
  {
    return gsl_histogram_max(_histogram);
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::value_type
  histogram<double,size_t>::min() const
  {
    return gsl_histogram_min(_histogram);
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::size_type
  histogram<double,size_t>::bins() const
  {
    return gsl_histogram_bins(_histogram);
  }

  //__________________________________________________________________
  inline void
  histogram<double,size_t>::reset()
  {
    return gsl_histogram_reset(_histogram);
  }

  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::fill(const value_type& x)
  {
    return gsl_histogram_increment(_histogram,x) == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::fill(const value_type& x, double w)
  {
    return gsl_histogram_accumulate(_histogram,x,w) == GSL_SUCCESS;
  }

  //__________________________________________________________________
  inline double
  histogram<double,size_t>::bin_content(const bin_type& bin) const
  {
    return gsl_histogram_get(_histogram,bin);
  }
  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::bin_range(const bin_type& bin, value_type& min, 
				      value_type& max) const
  {
    return gsl_histogram_get_range(_histogram,bin,&min,&max) 
      != GSL_EDOM;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::value_type
  histogram<double,size_t>::bin_min(const bin_type& bin) const
  {
    value_type min, max;
    gsl_histogram_get_range(_histogram,bin,&min,&max);
    return min;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::value_type
  histogram<double,size_t>::bin_max(const bin_type& bin) const
  {
    value_type min, max;
    gsl_histogram_get_range(_histogram,bin,&min,&max);
    return max;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::bin_type
  histogram<double,size_t>::bin_number(const value_type& x) const
  {
    size_t bin;
    if (gsl_histogram_find(_histogram,x,&bin) == GSL_EDOM)
      return 0;
    return bin;
  }
    
  //__________________________________________________________________
  inline double
  histogram<double,size_t>::max_content() const
  {
    return gsl_histogram_max_val(_histogram);
  }
  //__________________________________________________________________
  inline double
  histogram<double,size_t>::min_content() const
  {
    return gsl_histogram_min_val(_histogram);
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::bin_type
  histogram<double,size_t>::max_content_bin() const
  {
    return gsl_histogram_max_bin(_histogram);
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::bin_type
  histogram<double,size_t>::min_content_bin() const
  {
    return gsl_histogram_min_bin(_histogram);
  }
    
  //__________________________________________________________________
  inline histogram<double,size_t>::value_type
  histogram<double,size_t>::mean() const
  {
    return gsl_histogram_mean(_histogram);
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::value_type
  histogram<double,size_t>::sigma() const
  {
    return gsl_histogram_sigma(_histogram);
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::value_type
  histogram<double,size_t>::covariance() const
  {
    return sigma();
  }
  //__________________________________________________________________
  inline double
  histogram<double,size_t>::sum() const
  {
    return gsl_histogram_sum(_histogram);
  }    
  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::compatible(const histogram<value_type,bin_type>& 
				       other)
  {
    return 
      gsl_histogram_equal_bins_p(_histogram,other._histogram) == 1;
  }
    
  //__________________________________________________________________
  inline histogram<double,size_t>::value_type&
  histogram<double,size_t>::operator[](const bin_type& bin)
  {
    return _histogram->bin[bin];
  }
  //__________________________________________________________________
  inline const histogram<double,size_t>::value_type&
  histogram<double,size_t>::operator[](const bin_type& bin) const
  {
    return _histogram->bin[bin];
  }
  //__________________________________________________________________
  inline histogram<double,size_t>&
  histogram<double,size_t>::operator=(const histogram<value_type,bin_type>& 
				      other)
  {
    gsl_histogram_memcpy(_histogram,other._histogram);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>&
  histogram<double,size_t>::operator+=(const histogram<value_type,bin_type>& 
				       other)
  {
    gsl_histogram_add(_histogram,other._histogram);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>&
  histogram<double,size_t>::operator-=(const histogram<value_type,bin_type>& 
				       other)
  {
    gsl_histogram_sub(_histogram,other._histogram);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>&
  histogram<double,size_t>::operator*=(const histogram<value_type,bin_type>& 
				       other)
  {
    gsl_histogram_mul(_histogram,other._histogram);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>&
  histogram<double,size_t>::operator/=(const histogram<value_type,bin_type>& 
				       other)
  {
    gsl_histogram_div(_histogram,other._histogram);
    return *this;
  }

  //__________________________________________________________________
  inline histogram<double,size_t>&
  histogram<double,size_t>::operator+=(value_type x)
  {
    gsl_histogram_shift(_histogram,x);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>&
  histogram<double,size_t>::operator-=(value_type x)
  {
    gsl_histogram_shift(_histogram,-x);
    return *this;
  }    
  //__________________________________________________________________
  inline histogram<double,size_t>&
  histogram<double,size_t>::operator*=(value_type x)
  {
    gsl_histogram_scale(_histogram,x);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>&
  histogram<double,size_t>::operator/=(value_type x)
  {
    gsl_histogram_scale(_histogram,1/x);
    return *this;
  }
  //__________________________________________________________________
  inline 
  histogram<double,size_t>::pdf::pdf(const size_type& size) 
    : _pdf(0)
  {
    _pdf = gsl_histogram_pdf_alloc(size);
  }
  //__________________________________________________________________
  inline 
  histogram<double,size_t>::pdf::pdf(const histogram<value_type,bin_type>& h)
    : _pdf(0)
  {
    _pdf = gsl_histogram_pdf_alloc(h.bins());
    init(h);
  }
  //__________________________________________________________________
  inline 
  histogram<double,size_t>::pdf::pdf(const pdf& other) 
    : _pdf(0)
  {
    _pdf = gsl_histogram_pdf_alloc(other._pdf->n);
    *this = other;
  }
  //__________________________________________________________________
  inline histogram<double,size_t>::pdf&
  histogram<double,size_t>::pdf::operator=(const pdf& other) 
  {
    for (size_t i = 0; i <= _pdf->n && i < other._pdf->n; i++) {
      _pdf->range[i] = other._pdf->range[i];
      _pdf->sum[i] = other._pdf->sum[i];
    }
    return *this;
  }
  //__________________________________________________________________
  inline 
  histogram<double,size_t>::pdf::~pdf() 
  {
    gsl_histogram_pdf_free(_pdf);
  }
  //__________________________________________________________________
  inline void
  histogram<double,size_t>::pdf::init(const histogram<value_type,bin_type>& h)
  {
    gsl_histogram_pdf_init(_pdf,h._histogram);
  }
  //__________________________________________________________________
  inline double
  histogram<double,size_t>::pdf::sample(const double& r) const 
  {
    return gsl_histogram_pdf_sample(_pdf,r);
  }

  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::write(FILE* f) const
  {
    int ret = gsl_histogram_fwrite(f, _histogram);
    return (ret == GSL_EFAILED ? false: true);
  }
  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::read(FILE* f)
  {
    int ret = gsl_histogram_fread(f, _histogram);
    return (ret == GSL_EFAILED ? false: true);
  }
  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::print(FILE* s, const char* r, const char* b) const
  {
    int ret = gsl_histogram_fprintf(s, _histogram, r, b);
    return (ret == GSL_EFAILED ? false: true);
  }
  //__________________________________________________________________
  inline bool
  histogram<double,size_t>::scan(FILE* s)
  {
    int ret = gsl_histogram_fscanf(s, _histogram);
    return (ret == GSL_EFAILED ? false: true);
  }
}

  
#endif
//____________________________________________________________________
//
// EOF
//
