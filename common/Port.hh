// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_Port_hh
#define sigblocks_common_Port_hh

#include "IPort.hh"
#include "MultiPtr.hh"
#include "TimeTick.hh"

namespace sigblocks
{
  // XXX specialze a case when N=1, M=1 and optimize it
  template <int N, int M, class T>
  class Port
    : public IPort<T>
  {
  public:
    Port();
    virtual ~Port();

  public: // IPort interface
    void SetSource(IPort<T>* peer, int index);
    void SetSink(std::shared_ptr<IPort<T> >& peer, int index);

    void DisconnectSource(IPort<T>* peer);

    /// use this interface when T is a primitive data type, ex. int, float, etc
    void ConsumeData(
      const IPort<T>* pSender, const T& data, const TimeTick& startTime);

    void ConsumeData(
      const IPort<T>* pSender, MultiPtr<T> pData, int len, const TimeTick& startTime);

  protected:
    /// use this interface when T is a primitive data type, ex. int, float, etc
    void LeakData(
      int index, const T& data, const TimeTick& startTime);

    void LeakData(
      int index, MultiPtr<T> pData, int len, const TimeTick& startTime);

    virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime);
    virtual void Process(
      int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime);

  private:
    //mutable BoostPort::Mutex mMutex;
    IPort<T>* mpSource[N];
    std::shared_ptr<IPort<T> > mpSink[M];
  };

  /// Partial Template Specialization for a information source
  template <int M, class T>
  class Port<0, M, T>
    : public IPort<T>
  {
  public:
    Port();
    virtual ~Port();

  private: // IPort interface which is not allowed!
    // NOT ALLOWED to be used
    void SetSource(IPort<T>* peer, int index);
    void DisconnectSource(IPort<T>* peer);
    /// use this interface when T is a primitive data type, ex. int, float, etc
    void ConsumeData(
      const IPort<T>* pSender, const T& data, const TimeTick& startTime);
    void ConsumeData(
      const IPort<T>* pSender, MultiPtr<T> pData, int len, const TimeTick& startTime);

  public: // IPort interface
    void SetSink(std::shared_ptr<IPort<T> >& peer, int index);

  protected:
    /// use this interface when T is a primitive data type, ex. int, float, etc
    void LeakData(
      int index, const T& data, const TimeTick& startTime);

    void LeakData(
      int index, MultiPtr<T> pData, int len, const TimeTick& startTime);

  private:
    //mutable BoostPort::Mutex mMutex;
    std::shared_ptr<IPort<T> > mpSink[M];
  };

  /// Partial Template Specialization for a information sink
  template <int N, class T>
  class Port<N, 0, T>
    : public IPort<T>
  {
  public:
    Port();
    virtual ~Port();

  private: // IPort interface which is not allowed
    void SetSink(std::shared_ptr<IPort<T> >& peer, int index);

  public: // IPort interface
    void SetSource(IPort<T>* peer, int index);

    void DisconnectSource(IPort<T>* peer);

    /// use this interface when T is a primitive data type, ex. int, float, etc
    void ConsumeData(
      const IPort<T>* pSender, const T& data, const TimeTick& startTime);

    void ConsumeData(
      const IPort<T>* pSender, MultiPtr<T> pData, int len, const TimeTick& startTime);

  protected:
    virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime);
    virtual void Process(
      int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime);

  private:
    //mutable BoostPort::Mutex mMutex;
    IPort<T>* mpSource[N];
  };

  /// Partial Template Specialization for a null port
  template <class T>
  class Port<0, 0, T>
    : public IPort<T>
  {
  public:
    virtual ~Port();

  private: // IPort interface not allowed
    void SetSource(IPort<T>* peer, int index);
    void SetSink(std::shared_ptr<IPort<T> >& peer, int index);

    void DisconnectSource(IPort<T>* peer);

  public: // IPort interface (XXX do we need to use these?)
    /// use this interface when T is a primitive data type, ex. int, float, etc
    void ConsumeData(
      const IPort<T>* pSender, const T& data, const TimeTick& startTime);

    void ConsumeData(
      const IPort<T>* pSender, MultiPtr<T> pData, int len, const TimeTick& startTime);
  };
}

#endif // sigblocks_common_Port_hh
