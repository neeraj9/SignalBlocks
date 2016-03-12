//
// $Id: weighted_dataset_skel.hh,v 1.2 2006-02-08 00:08:50 cholm Exp $ 
//  
// WARNING: automatically generated by @progname@
//          options: @skel@, @type@.
//          Do not edit this file.
//
//  gslmm::statistic@name@
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
#ifndef GSLMM_statistic_weighted_dataset@name@
#define GSLMM_statistic_weighted_dataset@name@
#line 28 "weighted_dataset_skel.hh"
#ifndef GSLMM_statistics_dataset@name@
# include <gslmm/statistic/dataset@name@.hh>
#endif
#ifndef __GSL_SORT_H__
# include <gsl/gsl_sort.h>
#endif
#ifndef __GSL_PERMUTE_H__
# include <gsl/gsl_permute.h>
#endif
#ifndef __VECTOR__
# include <vector>
#endif
/** @file   statistic/weighted_dataset@name@.hh
    @author Christian Holm 
    @date   Wed Mar 12 14:11:20 2003
    @brief  statistic class for @c @type@ */

namespace gslmm 
{
  
  /** @brief Class template specialisation for @c @type@ 
      This class template specialisation represents a data set where
      each entry is of type @type@ and has a weight.  One creates a
      data set like  
      @code 
      gslmm::weighted_dataset<@code@> d;
      @endcode 
      after which one can add data points to @c d with weight @c w
      using the @c push_back member function. 
      @code 
      d.push_back(1,1);
      @endcode 
      At any given point, one can ask for various moments of the data
      set, like the mean, standard deviation, skew, and so on.  
      @ingroup statistic
   */
  template <>
  class weighted_dataset<@type@ > : public dataset<@type@ >
  {
  public:
    typedef @type@   data_type;
    typedef @ftype@  return_type;
    typedef std::vector<@type@ > container_type;
    typedef dataset<@type@ > base_type;
    
    /** Constructor, that initialises the data to the array @a d 
	@param d Initial data  
	@param w Initial weights */
    weighted_dataset(const container_type& d, const container_type& w);
    /** Constructor that makes an empty sample. 
	@param n size of data set */
    weighted_dataset(size_t n=0) : base_type(n), _weights(n) {}
    /** Destructor */
    virtual ~weighted_dataset() {}
    
    /** push another element onto the data set 
	@param d Data to push onto the set. The weight is assumed to
	be one */
    void push_back(data_type& d) 
    { 
      _data.push_back(d); 
      _weights.push_back(data_type(1));
    }
    /** push another element onto the data set 
	@param d Data to push onto the set 
	@param w the corresponding weight */
    void push_back(data_type& d, data_type& w) 
    { 
      _data.push_back(d); 
      _weights.push_back(w);
    }
    /** push another element onto the data set 
	@param d Data to push onto the set */
    void push_back(std::pair<data_type,data_type>& d) 
    { 
      _data.push_back(d.first); 
      _weights.push_back(d.second); 
    }

    /** Access individual elements 
	@param i index */ 
    data_type& weight(size_t i) { return _weights[i]; }
    /** Access individual elements 
	@param i index  */ 
    const data_type& weight(size_t i) const { return _weights[i]; }

    /** @return the weight array */
    container_type& weights() { return _weights; }
    /** @return the weight array */
    const container_type& weights() const { return _weights; }

    /** Calculate and return the sample mean: 
	@f[
	\hat\mu =  \frac{\sum x_i}{\sum w_i}
	@f]
	where @f$ x_i@f$ are the elements of the dataset of, and 
	@f$ w_i@f$ the corresponding weight.
	@return @f$\hat\mu@f$ */
    virtual return_type mean() const;
    
    /** This member function returns the estimated, or "sample",
	variance of the dataset of length @f$ N@f$.  The estimated 
	variance is denoted by @f$ \hat\sigma^2@f$ and is defined by,  
	@f[
	  \hat\sigma^2 = \frac{\sum w_i}{(\sum w_i)^2 - \sum w_i^2}
	  \sum w_i (x_i - \hat\mu)^2
	@f]
	where @f$ x_i@f$ are the elements of the dataset, and @f$
	w_i@f$ the corresponding weight. Note that this expression
	reduces to an unweighted variance with the familiar @f$
	1/(N-1)@f$ factor when there are N equal non-zero weights.
	@return @f$\hat\sigma^2@f$ */
    virtual return_type variance() const;
    
    /** This member function returns the sample variance of the
	dataset relative to the given value of @a mean.  The function
	is computed with @f$ \hat\mu@f$ replaced by the value of @a
	mean that you supply, 
	@f[
          \hat\sigma^2 = A \sum w_i (x_i - mean)^2
	@f]
	where 
	@f[ 
 	  A = \frac{\sum w_i}{(\sum w_i)^2 - \sum w_i^2}
	@f]
	is the mean is computed from the sample (@a apriori is @c
	false), or 
	@f[ 
	  A=1/\sum w_i
	@f]
	of the underlying distribution is known @e a @e priori (@a
	apriori is @c true).
	@param mean the given mean
	@param apriori If true, the mean is assumed to be known @e a
	@e priori and the normalisation is @f$ A=(1/N)@f$, otherwise
	the mean is taken to be calculated and the normalisation @f$
	A=(1/(N-1))@f$ is used/
	@return @f$\hat\sigma^2@f$ */
    virtual return_type variance(const return_type& mean, 
			       bool apriori=false) const; 

    /** The standard deviation is defined as the square root of the
	variance.  This member function return the square root of the 
	corresponding variance member functions above. 
	@return @f$\sqrt{\hat\sigma^2} @f$ */
    virtual return_type standard_deviation() const;
    
    /** The standard deviation is defined as the square root of the
	variance.  This member function return the square root of the 
	corresponding variance member functions above. 
	@param mean the given mean
	@param apriori If true, the mean is assumed to be known @e a
	@e priori and the normalisation is @f$ A=(1/N)@f$, otherwise
	the mean is taken to be calculated and the normalisation @f$
	A=(1/(N-1))@f$ is used/
	@return @f$\sqrt{\hat\sigma^2} @f$ */
    virtual return_type standard_deviation(const return_type& mean, 
					 bool apriori=false) const;
    
    /** This member function computes the absolute deviation from the
	mean of the dataset of length @f$ N@f$.  The absolute eviation
	from the mean is defined as, 
	@f[
          absdev  = \frac{\sum w_i |x_i - \hat\mu|}{\sum w_i}
	@f]
	where @f$ x_i@f$ are the elements of the dataset.  The
	absolute deviation from the mean provides a more robust
	measure of the width of a distribution than the variance.
	@return @f$ absdev @f$ */
    virtual return_type absolute_deviation() const;
    
    /** This member function computes the absolute deviation from the
	mean of the dataset of length @f$ N@f$.  The absolute eviation
	from the mean is defined as, 
	@f[
          absdev  = \frac{\sum w_i |x_i - mean|}{\sum w_i}
	@f]
	where @f$ x_i@f$ are the elements of the dataset.  The
	absolute deviation from the mean provides a more robust
	measure of the width of a distribution than the variance.
	@param mean the given mean
	@return @f$ absdev @f$ */
    virtual return_type absolute_deviation(const return_type& mean) const;

    /** This function computes the skewness of a dataset of length @f$
	N @f$.  The skewness is defined as,
	@f[
          skew = \frac{\sum w_i ((x_i - \hat\mu)/\hat\sigma)^3}{\sum w_i}
        @f]
	where @f$ x_i@f$ are the elements of the dataset.  The
	skewness measures the asymmetry of the tails of a
	distribution. 
	@return @f$ skew@f$ */
    virtual return_type skew() const;
    /** This function computes the skewness of a dataset of length @f$
	N @f$ with pre-computed mean @a mean and standard deviation @a
	sd.  The skewness is defined as, 
	@f[
          skew = \frac{\sum w_i ((x_i - mean)/sd)^3}{\sum w_i}
        @f]
	where @f$ x_i@f$ are the elements of the dataset.  The
	skewness measures the asymmetry of the tails of a
	distribution. 
	@param mean Pre-calculated mean
	@param sd Pre-calculated standard deviation
	@return @f$ skew@f$ */
    virtual return_type skew(const return_type& mean, const return_type& sd) const;

    /** This function computes the kurtosisness of a dataset of length @f$
	N @f$.  The kurtosis is defined as,
	@f[
          kurtosis = \frac{\sum w_i ((x_i -
	\hat\mu)/\hat\sigma)^4)}{\sum w_i} - 3 
        @f]
	where @f$ x_i@f$ are the elements of the dataset.  The
	kurtosis measures how sharply peaked a distribution is,
	relative to its width.  The kurtosis is normalized to zero for
	a  Gaussian distribution.
	@return @f$ kurtosis@f$ */
    virtual return_type kurtosis() const;
    /** This function computes the kurtosisness of a dataset of length @f$
	N @f$ with pre-computed mean @a mean and standard deviation @a
	sd.  The kurtosis is defined as, 
	@f[
	  kurtosis = \frac{\sum w_i ((x_i - mean)/sd)^4)}{\sum w_i} - 3
        @f]
	where @f$ x_i@f$ are the elements of the dataset.  The
	kurtosis measures how sharply peaked a distribution is,
	relative to its width.  The kurtosis is normalized to zero for
	a  Gaussian distribution.
	@param mean Pre-calculated mean
	@param sd Pre-calculated standard deviation
	@return @f$ kurtosis@f$ */
    virtual return_type kurtosis(const return_type& mean, 
			       const return_type& sd) const;

    /** Sort the dataset and weights in ascending order.  The sort is
	done by first generating and index for the sort, and then
	apply that permutation to the data and weights.   Therefor,
	this member function is a tad slow and shouldn't be used
	extensively.  Note, that iterators are invalidated after this
	operation. */
    virtual void sort();
    
  protected:
    container_type _weights;
  };
  
  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::weighted_dataset(const container_type& d,
					      const container_type& w) 
    : base_type(d), _weights(w)
  {}
  
  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::mean() const
  {
    return gsl_stats@low@_wmean(&(_weights[0]), 1, &(_data[0]), 1, 
				_data.size());
  }
  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::variance() const
  {
    return gsl_stats@low@_wvariance(&(_weights[0]), 1, &(_data[0]), 1, 
				    _data.size());
  }

  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::variance(const return_type& mean, bool apriori) const
  {
    if (apriori) 
      return gsl_stats@low@_wvariance_with_fixed_mean(&(_weights[0]), 1, 
						      &(_data[0]), 1, 
						      _data.size(), mean);
    return gsl_stats@low@_wvariance_m(&(_weights[0]), 1, &(_data[0]), 1, 
				      _data.size(), mean);
  }
  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::standard_deviation() const
  {
    return gsl_stats@low@_wsd(&(_weights[0]), 1, &(_data[0]), 1,
			      _data.size());
  }

  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::standard_deviation(const return_type& mean, 
					  bool apriori) const
  {
    if (apriori) 
      return gsl_stats@low@_wsd_with_fixed_mean(&(_weights[0]), 1, 
						&(_data[0]), 1, _data.size(), 
						mean);
    return gsl_stats@low@_wsd_m(&(_weights[0]), 1, &(_data[0]), 1, 
				_data.size(), mean);
  }
  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::absolute_deviation() const
  {
    return gsl_stats@low@_wabsdev(&(_weights[0]), 1, &(_data[0]), 1, 
				  _data.size());
  }

  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::absolute_deviation(const return_type& mean) const
  {
    return gsl_stats@low@_wabsdev_m(&(_weights[0]), 1, &(_data[0]), 1, _data.size(), mean);
  }

  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::skew() const
  {
    return gsl_stats@low@_wskew(&(_weights[0]), 1, &(_data[0]), 1, 
				_data.size());
  }

  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::skew(const return_type& mean, 
			    const return_type& sd) const
  {
    return gsl_stats@low@_wskew_m_sd(&(_weights[0]), 1, &(_data[0]), 1, 
				     _data.size(), mean, sd);
  }

  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::kurtosis() const
  {
    return gsl_stats@low@_wkurtosis(&(_weights[0]), 1, &(_data[0]), 1, 
				    _data.size());
  }

  //__________________________________________________________________
  inline 
  weighted_dataset<@type@ >::return_type 
  weighted_dataset<@type@ >::kurtosis(const return_type& mean, 
				      const return_type& sd) const
  {
    return gsl_stats@low@_wkurtosis_m_sd(&(_weights[0]), 1, &(_data[0]), 1, 
					 _data.size(), mean, sd);
  }
  //__________________________________________________________________
  inline 
  void
  weighted_dataset<@type@ >::sort()
  {
    size_t p[_data.size()];
    // Get permutation that sorts the data array in ascending order. 
    gsl_sort@low@_index(p, &(_data[0]), 1, _data.size());
    // Apply that permutation to the weights
    gsl_permute@low@(p, &(_weights[0]), 1, _data.size());
    // Apply that permutation to the data
    gsl_permute@low@(p, &(_data[0]),    1, _data.size());
  }
  

}


#endif
//____________________________________________________________________
//
// EOF
//