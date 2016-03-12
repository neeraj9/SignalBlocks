// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_MixedPort_hh
#define sigblocks_common_MixedPort_hh

#include "IPort.hh"
#include "TimeTick.hh"

namespace sigblocks {
    // XXX specialze a case when N=1, M=1 and optimize it
    template<int N, int M, class TN, class TM>
    class MixedPort
            : public IPort<TN>,
              public IPort<TM> {
    public:
        MixedPort();

        virtual ~MixedPort();

    public: // IPort interface
        void SetSource(IPort<TN>* peer, int index);

        void SetSink(std::shared_ptr<IPort<TM> >& peer, int index);

        void DisconnectSource(IPort<TN>* peer);

        /// use this interface when T is a primitive data type, ex. int, float, etc
        void ConsumeData(
                const IPort<TN>* pSender, const TN& data, const TimeTick& startTime);

        void ConsumeData(
                const IPort<TN>* pSender, std::unique_ptr<TN[]> data, int len, const TimeTick& startTime);

    private: // IPort interface (which should not be used)
        // NOT TO BE USED
        void SetSource(IPort<TM>* peer, int index);

        void SetSink(std::shared_ptr<IPort<TN> >& peer, int index);

        void DisconnectSource(IPort<TM>* peer);

        void ConsumeData(
                const IPort<TM>* pSender, const TM& data, const TimeTick& startTime);

        void ConsumeData(
                const IPort<TM>* pSender, std::unique_ptr<TM[]> data, int len, const TimeTick& startTime);

    protected:
        /// use this interface when T is a primitive data type, ex. int, float, etc
        void LeakData(
                int index, const TM& data, const TimeTick& startTime);

        void LeakData(
                int index, std::unique_ptr<TM[]> data, int len, const TimeTick& startTime);

        virtual void Process(int sourceIndex, const TN& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<TN[]> data, int len, const TimeTick& startTime);

    private:
        //mutable BoostPort::Mutex mMutex;
        IPort<TN>* mpSource[N];
        std::shared_ptr<IPort<TM> > mpSink[M];
    };
}

#endif // sigblocks_common_MixedPort_hh
