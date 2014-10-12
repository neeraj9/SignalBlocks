// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_blocks_sink_OstreamSink_hh
#define iblocks_blocks_sink_OstreamSink_hh

#include "../../common/Port.hh"

#include <ostream>
#include <memory>

namespace iblocks
{
  template <class T>
  class OstreamSink
    : public Port<1, 0, T>
  {
  public:
    OstreamSink();
    OstreamSink(std::auto_ptr<std::ostream> pIns);
    void SetStreamSink(std::auto_ptr<std::ostream> pIns);

  public: // Port interface
    void Process(int sourceIndex, const T& data, const TimeTick& startTime);
    void Process(
      int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime);

  private:
    std::auto_ptr<std::ostream> mpOstream;
  };
}

#endif // iblocks_blocks_sink_OstreamSink_hh
