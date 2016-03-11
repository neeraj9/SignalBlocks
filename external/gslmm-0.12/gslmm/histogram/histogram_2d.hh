//
// $Id: histogram_2d.hh,v 1.12 2006-02-08 00:07:19 cholm Exp $ 
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
#ifndef GSLMM_histogram_2d
#define GSLMM_histogram_2d
#ifndef __VECTOR__
#include <vector>
#endif
#ifndef __UTILITY__
#include <utility>
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
#ifndef __GSL_HISTOGRAM2D_H__
#include <gsl/gsl_histogram2d.h>
#endif
/** @file   histogram/histogram_2d.hh
    @author Christian Holm
    @date   Wed Mar 12 14:11:20 2003
    @brief  histogram class */

namespace gslmm 
{
  /** @brief A 2D histogram class. 
      @ingroup histogram
  */
  template<>
  class histogram<std::pair<double,double>,std::pair<size_t,size_t> >
  {
  public:
    /// Value type 
    typedef std::pair<double,double> value_type;
    /// Bin type
    typedef std::pair<size_t,size_t> bin_type;
  protected:
    /// The low level histogram
    gsl_histogram2d* _histogram;
  public:
    /** Constructor 
	Make a histogram with @a bins 
	@param bins The number of bins in each dimension 
	@param zero Zero the histogram? */
    histogram(const bin_type& bins, bool zero=true);
    /** Constructor.  Creates a histogram with @a bins evenly spaced 
	bins in the ranges from @a min to @a max
	@code 
	std::pair<size_t,size_t> bins(3,4);
	std::pair<double,double> min(1,10);
	std::pair<double,double> max(3,15);
	gslmm::histogram<std::pair<size_t,size_t>,std::pair<double,double> >
	  h(bins, min, max);
	@endcode
	@param bins Number of bins 
	@param min Minimum of the histogram
	@param max Maximum of the histogram   */
    histogram(const bin_type& bins, const value_type& min, 
	      const value_type& max);
    /** Construct a histogram with @f$ N \times M@f$ bins, where @f$
	N@f$ is @a bins.first and @f$ M@f$ is @a bins.second, variable
	ranges.

	Notice, that @a bins should be the size of the arrays @e minus 1
	@a xranges and @a yranges.   So, to construct a histogram with
	logarithmic scales, one would do 
	@code 
	std::pair<size_t,size_t> bins(2,3);
	double x[] = { 1, 10, 100 };
	double y[] = { 10, 100, 1000, 10000 };
	gslmm::histogram<std::pair<size_t,size_t>,std::pair<double,double> >
	  h(bins, x, y);
	@endcode
	@param bins The number of bins in each dimenstion
	@param xranges The ranges in X
	@param yranges The ranges in Y  */
    histogram(const bin_type& bins, const double* xranges, 
	      const double* yranges);
    /** Constructor 
	Allocates 
	@code 
	std::vector<double> x(3);
	std::vector<double> y(4);
	x[0] =  1; x[1] =  10; x[2] =  100;
	y[0] = 10; y[1] = 100; y[2] = 1000; y[3] = 10000;
	gslmm::histogram<std::pair<size_t,size_t>,std::pair<double,double> >
	  h(x, y);
	@endcode
	@param xranges The ranges in X
	@param yranges The ranges in Y  */
    histogram(const std::vector<double>& xranges,
	      const std::vector<double>& yranges);
    /** Construct a histogram with variable ranges.  Note, that there
	doesn't have to be the same number of bins in each dimension.
	It's enough to specify the number of bins for each dimension
	seperately, like 
	@code 
	std::pair<size_t,size_t>                bins(2,3);
	std::vector<std::pair<double,double> >  
	  ranges(std::max(bins.first, bins.second));
        ranges[0].first  = 1;
	ranges[0].second = 10;
        ranges[1].first  = 10;
	ranges[1].second = 100;
        ranges[2].first  = 100;
	ranges[2].second = 1000;
	ranges[3].second = 10000;
	gslmm::histogram<std::pair<size_t,size_t>,std::pair<double,double> >
	  h(bins, ranges);
	@endcode
	This will create a histogram with the X ranges
	@f$[1,10),[10,100]@f$ and the Y ranges
	@f$[10,100),[100,1000),[1000,10000]@f$ 
	@param bins The number of bins in each dimension. 
	@param ranges the ranges. */
    histogram(const bin_type& bins, const std::vector<value_type>& ranges);
    /** Copy constructor */
    histogram(const histogram<value_type,bin_type>& other);
    /** Destructor */
    virtual ~histogram();
    
    /** Set the ranges. */
    bool range(const value_type& min, const value_type& max);
    /** Set the ranges. */
    bool range(const double* xranges, const double* yranges);
    /** Set the ranges. */
    bool range(const std::vector<double>& xranges,
	       const std::vector<double>& yranges);
    
    /** Get the maximum of the range */
    value_type max() const;
    /** Get the minimum of the range */
    value_type min() const;
    /** Get the number of bins */
    bin_type bins() const;
  
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
    const double& operator[](const bin_type& bin) const;
    /** Bin content */
    double& operator[](const bin_type& bin);
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
    histogram<value_type,bin_type>& operator+=(double x);
    /** Subtract a constant from this histogram */
    histogram<value_type,bin_type>& operator-=(double x);    
    /** Scale this histogram with a constant */
    histogram<value_type,bin_type>& operator*=(double x);
    /** Scale this histogram with a constant */
    histogram<value_type,bin_type>& operator/=(double x);
    
    /** @brief Probablity distribution function from a 2D histogram 
	@ingroup histogram 
    */
    class pdf 
    {
    protected:
      /// Low-level type
      gsl_histogram2d_pdf* _pdf;
    public:
      /** Constrcutor 
	  @param size Pair of number of entries in corresponding
	  histogram. */ 
      pdf(const bin_type& size);
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
	  @param r a pair of uniform deviates in [0,1] 
	  @return a pair of numbers drawn from the PDF. */
      value_type sample(const std::pair<double,double>& r) const;
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
#define TEMPL std::pair<double,double>, std::pair<size_t,size_t>
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::histogram(const bin_type& bins, bool zero) 
    : _histogram(0)
  {
    if (zero) _histogram = gsl_histogram2d_calloc(bins.first,bins.second);
    else      _histogram = gsl_histogram2d_alloc(bins.first,bins.second);
  }
  //__________________________________________________________________
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::histogram(const bin_type& bins, 
			       const value_type& min, 
			       const value_type& max) 
    : _histogram(0)
  {
    _histogram = gsl_histogram2d_calloc_uniform(bins.first,
						bins.second,
						min.first,
						max.first,
						min.second,
						max.second);
  }  
  //__________________________________________________________________
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::histogram(const std::vector<double>& xranges,
			       const std::vector<double>& yranges)
    : _histogram(0)
  {
    double* xaxis = const_cast<double*>(&(xranges[0]));
    double* yaxis = const_cast<double*>(&(yranges[0]));
    _histogram = gsl_histogram2d_calloc_range(xranges.size() - 1, 
					      yranges.size() - 1,
					      xaxis,
					      yaxis);
  }  
  //__________________________________________________________________
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::histogram(const bin_type& bins, 
			       const double* xranges,
			       const double* yranges)
    : _histogram(0)
  {
    double* xaxis = const_cast<double*>(xranges);
    double* yaxis = const_cast<double*>(yranges);
    _histogram = gsl_histogram2d_calloc_range(bins.first,
					      bins.second,
					      xaxis, yaxis);
  }  
  //__________________________________________________________________
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::histogram(const bin_type& bins, 
			       const std::vector<value_type>& ranges)
    : _histogram(0)
  {
    std::vector<double> xranges(bins.first);
    std::vector<double> yranges(bins.second);
    size_t j = 0;
    for (std::vector<value_type>::const_iterator i = ranges.begin();
	 i != ranges.end(); ++i, ++j) {
      xranges[j] = (*i).first;
      yranges[j] = (*i).second;
    }
    double* xaxis = const_cast<double*>(&(xranges[0]));
    double* yaxis = const_cast<double*>(&(yranges[0]));
    _histogram = gsl_histogram2d_calloc_range(bins.first,bins.second, 
					      xaxis,yaxis);
  }  
  //__________________________________________________________________
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::histogram(const histogram<value_type,bin_type>& other) 
    : _histogram(0)
  {
    _histogram = gsl_histogram2d_clone(other._histogram);
  }
  //__________________________________________________________________
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::~histogram() 
  {
    gsl_histogram2d_free(_histogram);
  }
  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::range(const value_type& min, const value_type& max)
  {
    return gsl_histogram2d_set_ranges_uniform(_histogram,
						       min.first,
						       max.first,
						       min.second,
						       max.second) 
      == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::range(const double* xranges, const double* yranges)
  {
    double* xaxis = const_cast<double*>(xranges);
    double* yaxis = const_cast<double*>(yranges);
    return gsl_histogram2d_set_ranges(_histogram,
					       xaxis, bins().first, 
					       yaxis, bins().second)
      == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::range(const std::vector<double>& 
			   xranges,
			   const std::vector<double>& 
			   yranges)
  { 
    double* xaxis = const_cast<double*>(&(xranges[0]));
    double* yaxis = const_cast<double*>(&(yranges[0]));
    return gsl_histogram2d_set_ranges(_histogram,
					       xaxis, xranges.size(), 
					       yaxis, yranges.size())
      == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::value_type
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::max() const
  {
    value_type vmax;
    vmax.first  = gsl_histogram2d_xmax(_histogram);
    vmax.second = gsl_histogram2d_ymax(_histogram);
    return vmax;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::value_type
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::min() const
  {
    value_type vmin;
    vmin.first  = gsl_histogram2d_xmin(_histogram);
    vmin.second = gsl_histogram2d_ymin(_histogram);
    return vmin;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::bin_type
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::bins() const
  {
    bin_type vbin;
    vbin.first  = gsl_histogram2d_nx(_histogram);
    vbin.second = gsl_histogram2d_ny(_histogram);
    return vbin;
  }

  //__________________________________________________________________
  inline void
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::reset()
  {
    return gsl_histogram2d_reset(_histogram);
  }

  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::fill(const value_type& x)
  {
    return gsl_histogram2d_increment(_histogram,x.first, x.second) 
      == GSL_SUCCESS;
  }
  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::fill(const value_type& x, double w)
  {
    return gsl_histogram2d_accumulate(_histogram,x.first, x.second,w) 
      == GSL_SUCCESS;
  }

  //__________________________________________________________________
  inline double
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::bin_content(const bin_type& bin) const
  {
    return gsl_histogram2d_get(_histogram,bin.first,bin.second);
  }
  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::bin_range(const bin_type& bin, 
			       value_type& min, 
			       value_type& max) const
  {
    bool ret = true;
    ret = gsl_histogram2d_get_xrange(_histogram,bin.first, 
				     &min.first, &max.first) != GSL_EDOM;
    ret = gsl_histogram2d_get_yrange(_histogram,bin.second, 
				     &min.second, &max.second) != GSL_EDOM;
    return ret;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::value_type
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::bin_min(const bin_type& bin) const
  {
    value_type min, max;
    bin_range(bin,min,max);
    return min;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::value_type
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::bin_max(const bin_type& bin) const
  {
    value_type min, max;
    bin_range(bin,min,max);
    return max;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::bin_type
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::bin_number(const value_type& x) const
  {
    bin_type bin;
    gsl_histogram2d_find(_histogram,x.first, x.second,
				  &bin.first, &bin.second);
    return bin;
  }
    
  //__________________________________________________________________
  inline double
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::max_content() const
  {
    return gsl_histogram2d_max_val(_histogram);
  }
  //__________________________________________________________________
  inline double
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::min_content() const
  {
    return gsl_histogram2d_min_val(_histogram);
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::bin_type
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::max_content_bin() const
  {
    bin_type bin;
    gsl_histogram2d_max_bin(_histogram,&bin.first,&bin.second);
    return bin;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::bin_type
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::min_content_bin() const
  {
    bin_type bin;
    gsl_histogram2d_min_bin(_histogram,&bin.first,&bin.second);
    return bin;
  }
    
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::value_type
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::mean() const
  {
    value_type m;
    m.first  =  gsl_histogram2d_xmean(_histogram);
    m.second =  gsl_histogram2d_ymean(_histogram);
    return m;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::value_type
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::sigma() const
  {
    value_type s;
    s.first  =  gsl_histogram2d_xsigma(_histogram);
    s.second =  gsl_histogram2d_ysigma(_histogram);
    return s;
  }
  //__________________________________________________________________
  inline double
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::covariance() const
  {
    return gsl_histogram2d_cov(_histogram);;
  }
  //__________________________________________________________________
  inline double
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::sum() const
  {
    return gsl_histogram2d_sum(_histogram);
  }    
  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::compatible(const histogram<value_type, bin_type>& other)
  {
    return 
      gsl_histogram2d_equal_bins_p(_histogram,other._histogram) == 1;
  }
    
  //__________________________________________________________________
  inline double&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator[](const bin_type& bin)
  {
    return _histogram->bin[bin.first * _histogram->ny + bin.second];
  }
  //__________________________________________________________________
  inline const double&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator[](const bin_type& bin) const
  {
    return _histogram->bin[bin.first * _histogram->ny + bin.second];
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator=(const histogram<value_type, bin_type>& other)
  {
    gsl_histogram2d_memcpy(_histogram,other._histogram);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator+=(const histogram<value_type, bin_type>& other)
  {
    gsl_histogram2d_add(_histogram,other._histogram);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator-=(const histogram<value_type, bin_type>& other)
  {
    gsl_histogram2d_sub(_histogram,other._histogram);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator*=(const histogram<value_type, bin_type>& other)
  {
    gsl_histogram2d_mul(_histogram,other._histogram);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator/=(const histogram<value_type, bin_type>& other)
  {
    gsl_histogram2d_div(_histogram,other._histogram);
    return *this;
  }

  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator+=(double x)
  {
    gsl_histogram2d_shift(_histogram,x);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator-=(double x)
  {
    gsl_histogram2d_shift(_histogram,-x);
    return *this;
  }    
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator*=(double x)
  {
    gsl_histogram2d_scale(_histogram,x);
    return *this;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::operator/=(double x)
  {
    gsl_histogram2d_scale(_histogram,1/x);
    return *this;
  }
  //__________________________________________________________________
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::pdf::pdf(const bin_type& size) 
    : _pdf(0)
  {
    _pdf = gsl_histogram2d_pdf_alloc(size.first,size.second);
  }
  //__________________________________________________________________
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::pdf::pdf(const histogram<value_type, bin_type>& h)
    : _pdf(0)
  {
    _pdf = gsl_histogram2d_pdf_alloc(h.bins().first, 
					      h.bins().second);
    init(h);
  }
  //__________________________________________________________________
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::pdf::pdf(const pdf& other)
    : _pdf(0)
  {
    _pdf = gsl_histogram2d_pdf_alloc(other._pdf->nx, 
				     other._pdf->ny);
    *this = other;
  }
  //__________________________________________________________________
  inline histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::pdf&
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::pdf::operator=(const pdf& other) 
  {
    for (size_t i = 0; i <= _pdf->nx && i <= other._pdf->nx; i++) 
      _pdf->xrange[i] = other._pdf->xrange[i];
    for (size_t i = 0; i <= _pdf->ny && i <= other._pdf->ny; i++) 
      _pdf->yrange[i] = other._pdf->yrange[i];
    size_t n1 = _pdf->nx * _pdf->ny;
    size_t n2 = other._pdf->nx * other._pdf->ny;
    for (size_t i = 0; i <= n1 && i <= n2; i++) 
      _pdf->sum[i] = other._pdf->sum[i];
    return *this;
  }    
  //__________________________________________________________________
  inline 
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::pdf::~pdf() 
  {
    gsl_histogram2d_pdf_free(_pdf);
  }
  //__________________________________________________________________
  inline void
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::pdf::init(const histogram<value_type, bin_type>& h)
  {
    gsl_histogram2d_pdf_init(_pdf,h._histogram);
  }
  //__________________________________________________________________
  inline std::pair<double,double>
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::pdf::sample(const std::pair<double,double>& r) const 
  {
    std::pair<double,double> v;
    gsl_histogram2d_pdf_sample(_pdf,r.first,r.second, 
					&v.first,&v.second);
    return v;
  }

  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::write(FILE* f) const
  {
    int ret = gsl_histogram2d_fwrite(f, _histogram);
    return (ret == GSL_EFAILED ? false: true);
  }
  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::read(FILE* f)
  {
    int ret = gsl_histogram2d_fread(f, _histogram);
    return (ret == GSL_EFAILED ? false: true);
  }
  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::print(FILE* s, const char* r, const char* b) const
  {
    int ret = gsl_histogram2d_fprintf(s, _histogram, r, b);
    return (ret == GSL_EFAILED ? false: true);
  }
  //__________________________________________________________________
  inline bool
  histogram<std::pair<double,double>,
            std::pair<size_t,size_t> >::scan(FILE* s)
  {
    int ret = gsl_histogram2d_fscanf(s, _histogram);
    return (ret == GSL_EFAILED ? false: true);
  }
}
#undef TEMPL
  
#endif
//____________________________________________________________________
//
// EOF
//
