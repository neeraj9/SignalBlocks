// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
// copyright (c) 2010-12 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE.txt for license information
#ifndef iblocks_common_IPort_hh
#define iblocks_common_IPort_hh

#include "MultiPtr.hh"
#include "SharedPtr.hxx"
#include "TimeTick.hh"

#include <iostream>
using namespace std;

namespace iblocks
{
  template <class T = float>
  class IPort
  {
  public:
    virtual ~IPort()
    {
    }

    virtual void SetSource(IPort<T>* peer, int index) = 0;
    virtual void SetSink(BoostPort::SharedPtr<IPort<T> >& peer, int index) = 0;

    virtual void DisconnectSource(IPort<T>* peer) = 0;

    virtual void ConsumeData(
      const IPort<T>* pSender, const T& data, const TimeTick& startTime) = 0;
    virtual void ConsumeData(
      const IPort<T>* pSender, MultiPtr<T> pData,
      int len, const TimeTick& startTime) = 0;

  };
}

#endif // iblocks_common_IPort_hh
