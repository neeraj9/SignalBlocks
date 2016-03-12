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

        virtual void ClockCycle(const TimeTick& timeTick) = 0;

    };

    /**
     * Connects source and destination ports, while returning reference to source.
     * @param src The source port.
     * @param dest The destination port.
     * @param srcindex The index within the source port where the destination
     *        port must connect from (default is 0).
     * @param destindex The index within the destination port where the source
     *        port must connect to (default is 0).
     * @return Reference to source port for chaining subsequently.
     *
     * Connects source and destination ports, while returning reference to
     * the source port as a mechanism to enable chaining.
     * Note that the source port keeps a reference of the destination port as
     * std::shared_ptr, while the destination keeps a raw pointer of the
     * source port. This is deliberately done to allow automatic deletion of
     * the destination port when the source port is destroyed (assuming that
     * only one source port connects to that destination).
     */
    template <typename T>
    std::shared_ptr<IPort<T> >& connect(std::shared_ptr<IPort<T> >& src,
                                        std::shared_ptr<IPort<T> >& dest,
                                        int srcindex = 0,
                                        int destindex = 0) {
        src->SetSink(dest, srcindex);
        dest->SetSource(src.get(), destindex);
        return src;
    }
}

#endif // sigblocks_common_IPort_hh
