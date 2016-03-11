// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_sink_PlotSink_hh
#define sigblocks_blocks_sink_PlotSink_hh

#ifdef HAVE_PLPLOT
#include "../../common/PlPlot.hh"
#endif

#include "../../common/Port.hh"

namespace sigblocks
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
      int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime);

  private:
#ifdef HAVE_PLPLOT
    PlPlot<PLP_WXWIDGETS, T> mPlot;
#endif
    T mBuffer[MAX_SAMPLES];
    int mNumSamples;
  };
}

#endif // sigblocks_blocks_sink_PlotSink_hh
