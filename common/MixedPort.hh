// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
// copyright (c) 2010-12 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE.txt for license information
#ifndef sigblocks_common_MixedPort_hh
#define sigblocks_common_MixedPort_hh

#include "IPort.hh"
#include "MultiPtr.hh"
#include "Mutex.hxx"
#include "SharedPtr.hxx"
#include "TimeTick.hh"

namespace sigblocks
{
  // XXX specialze a case when N=1, M=1 and optimize it
  template <int N, int M, class TN, class TM>
  class MixedPort
    : public IPort<TN>,
      public IPort<TM>
  {
  public:
    MixedPort();
    virtual ~MixedPort();

  public: // IPort interface
    void SetSource(IPort<TN>* peer, int index);
    void SetSink(BoostPort::SharedPtr<IPort<TM> >& peer, int index);

    void DisconnectSource(IPort<TN>* peer);

    /// use this interface when T is a primitive data type, ex. int, float, etc
    void ConsumeData(
      const IPort<TN>* pSender, const TN& data, const TimeTick& startTime);

    void ConsumeData(
      const IPort<TN>* pSender, MultiPtr<TN> pData, int len, const TimeTick& startTime);

  private: // IPort interface (which should not be used)
    // NOT TO BE USED
    void SetSource(IPort<TM>* peer, int index);
    void SetSink(BoostPort::SharedPtr<IPort<TN> >& peer, int index);
    void DisconnectSource(IPort<TM>* peer);
    void ConsumeData(
      const IPort<TM>* pSender, const TM& data, const TimeTick& startTime);
    void ConsumeData(
      const IPort<TM>* pSender, MultiPtr<TM> pData, int len, const TimeTick& startTime);

  protected:
    /// use this interface when T is a primitive data type, ex. int, float, etc
    void LeakData(
      int index, const TM& data, const TimeTick& startTime);

    void LeakData(
      int index, MultiPtr<TM> pData, int len, const TimeTick& startTime);

    virtual void Process(int sourceIndex, const TN& data, const TimeTick& startTime);
    virtual void Process(
      int sourceIndex, MultiPtr<TN> pData, int len, const TimeTick& startTime);

  private:
    mutable BoostPort::Mutex mMutex;
    IPort<TN>* mpSource[N];
    BoostPort::SharedPtr<IPort<TM> > mpSink[M];
  };
}

#endif // sigblocks_common_MixedPort_hh
