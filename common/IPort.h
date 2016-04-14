// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_IPORT_H
#define SIGBLOCKS_IPORT_H

#include "TimeTick.h"

#include <assert.h>
#include <memory>
#include <vector>

namespace sigblocks {
    template<class T = float>
    class IPort {
    public:
        virtual ~IPort() {
        }

        virtual void SetSource(IPort<T>* peer, int index) = 0;
        virtual bool IsSourceConnected(int index) const = 0;

        virtual void SetSink(std::shared_ptr<IPort<T> >& peer, int index) = 0;
        virtual bool IsSinkConnected(int index) const = 0;

        virtual void DisconnectSource(IPort<T>* peer) = 0;

        virtual void ConsumeScalar(
                const IPort<T>* pSender, const T& data, const TimeTick& startTime) = 0;

        virtual void ConsumeVector(
                const IPort<T>* pSender, std::unique_ptr<T[]> data,
                int len, const TimeTick& startTime) = 0;

        virtual void ConsumeMatrix(
                const IPort<T>* pSender, std::unique_ptr<T[]> data,
                const std::vector<int>& dims, const TimeTick& startTime) = 0;

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
        assert(! src->IsSinkConnected(srcindex));
        src->SetSink(dest, srcindex);
        assert(! dest->IsSourceConnected(destindex));
        dest->SetSource(src.get(), destindex);
        return src;
    }
}

#endif // SIGBLOCKS_IPORT_H
