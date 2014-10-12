// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_converters_BasicTypeConverter_hh
#define iblocks_converters_BasicTypeConverter_hh

#include "../../common/MixedPort.hh"

namespace iblocks
{
  template <class TFROM, class TTO>
  class BasicTypeConverter
    : public MixedPort<1,1,TFROM,TTO>
  {
  protected: // Override MixedPort interfaces for Sink
    virtual void Process(int sourceIndex, const TFROM& data, const TimeTick& startTime);
    virtual void Process(
      int sourceIndex, MultiPtr<TFROM> pData, int len, const TimeTick& startTime);
  };
}

#endif // iblocks_converters_BasicTypeConverter_hh
