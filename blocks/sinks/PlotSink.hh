// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_blocks_sink_PlotSink_hh
#define iblocks_blocks_sink_PlotSink_hh

#ifdef HAVE_PLPLOT
#include "../../common/PlPlot.hh"
#endif

#include "../../common/Port.hh"

namespace iblocks
{
  template <class T>
  class PlotSink
    : public Port<1, 0, T>
  {
    static const int MAX_SAMPLES = 500;

  public:
    PlotSink();

  public: // Port interface
    void Process(int sourceIndex, const T& data, const TimeTick& startTime);
    void Process(
      int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime);

  private:
#ifdef HAVE_PLPLOT
    PlPlot<PLP_WXWIDGETS, T> mPlot;
#endif
    T mBuffer[MAX_SAMPLES];
    int mNumSamples;
  };
}

#endif // iblocks_blocks_sink_PlotSink_hh
