// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_IPort_hh
#define sigblocks_common_IPort_hh

#include "TimeTick.hh"

#include <memory>
#include <iostream>

namespace sigblocks {
    template<class T = float>
    class IPort {
    public:
        virtual ~IPort() {
        }

        virtual void SetSource(IPort<T>* peer, int index) = 0;

        virtual void SetSink(std::shared_ptr<IPort<T> >& peer, int index) = 0;

        virtual void DisconnectSource(IPort<T>* peer) = 0;

        virtual void ConsumeData(
                const IPort<T>* pSender, const T& data, const TimeTick& startTime) = 0;

        virtual void ConsumeData(
                const IPort<T>* pSender, std::unique_ptr<T[]> data,
                int len, const TimeTick& startTime) = 0;

    };
}

#endif // sigblocks_common_IPort_hh
