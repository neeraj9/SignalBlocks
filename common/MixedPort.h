// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_MixedPort_hh
#define sigblocks_common_MixedPort_hh

#include "IPort.h"
#include "TimeTick.h"

namespace sigblocks {
    // XXX specialze a case when N=1, M=1 and optimize it
    template<int N, int M, class TN, class TM>
    class MixedPort
            : public IPort<TN> {
    public:
        MixedPort();

        virtual ~MixedPort();

        std::shared_ptr<IPort<TM> >& GetAsSinkType();

    public: // IPort interface
        void SetSource(IPort<TN>* peer, int index);

        void DisconnectSource(IPort<TN>* peer);

        /// use this interface when T is a primitive data type, ex. int, float, etc
        void ConsumeData(
                const IPort<TN>* pSender, const TN& data, const TimeTick& startTime);

        void ConsumeData(
                const IPort<TN>* pSender, std::unique_ptr<TN[]> data, int len, const TimeTick& startTime);

        void ClockCycle(const TimeTick& timeTick);

    private: // IPort interface (which should not be used because sink are of type TM)
        // NOT TO BE USED
        void SetSink(std::shared_ptr<IPort<TN> >& peer, int index);

    protected: // override these methods in derived class to do something useful
        /// use this interface when T is a primitive data type, ex. int, float, etc
        virtual void Process(int sourceIndex, const TN& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<TN[]> data, int len, const TimeTick& startTime);

    private:
        IPort<TN>* mpSource[N];
        std::shared_ptr<IPort<TM> > mInternalSource;
    };
}

#endif // sigblocks_common_MixedPort_hh
