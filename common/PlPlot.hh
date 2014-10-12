// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_PlPlot_hh
#define iblocks_common_PlPlot_hh

#include "GenericCopy.hh"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <plstream.h>

namespace iblocks
{
  template <class X>
  void Plot1d(plstream* pPls, const X* src, size_t count)
  {
    PLFLT ybuffer[count];
    GenericCopy<X, PLFLT>::Copy(src, src + count, ybuffer);

    PLFLT* max_value = std::max_element(ybuffer, ybuffer + count);
    PLFLT* min_value = std::min_element(ybuffer, ybuffer + count);
    //std::copy(src, src + count, std::ostream_iterator<X>(std::cout, ","));
    //std::cout << std::endl;
    std::cout << "max_value = " << *max_value << std::endl;
    std::cout << "min_value = " << *min_value << std::endl;
    pPls->col0(1);
    pPls->prec(1,1);
    //pPls->env(0, count - 1, -1.0, 1.0, 0, 2);
    pPls->env(0, count - 1, *min_value, *max_value, 0, 2);
    PLFLT x_axis_buffer[count];
    for (int i = 0; i < static_cast<int>(count); ++i)
    {
      x_axis_buffer[i] = static_cast<PLFLT>(i);
    }
    pPls->col0(3);
    pPls->lab("sample number", "value", "Value vs sample number");
    pPls->col0(4);
    pPls->line(count, x_axis_buffer, ybuffer);
    pPls->prec(0,0);
  }

  enum PlotDriver {
    PLP_PS = 0,
    PLP_XWIN = 1,
    PLP_WXWIDGETS = 2,
  };

  /**
   * PlPlot 2d plot of the input signal.
   */
  template <PlotDriver T, class X>
  class PlPlot
  {
  public:
    PlPlot(const char* name)
      : mPls()
    {
      if (T == PLP_XWIN)
      {
        mPls.sdev("xwin");
      }
      else if (T == PLP_PS)
      {
        mPls.sdev("ps");
      }
      else if (T == PLP_WXWIDGETS)
      {
        mPls.sdev("wxwidgets");
      }
      //mPls.sfname(name);
      mPls.init();
    }

    void Plot1D(const X* src, size_t count)
    {
      Plot1d<X>(&mPls, src, count);
    }

  private:
    plstream mPls;
  };
}

#endif // iblocks_common_PlPlot_hh
