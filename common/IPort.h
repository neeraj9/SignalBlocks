// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// This file is part of SignalBlocks.
//
// SignalBlocks is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SignalBlocks is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SIGNALBLOCKS_IPORT_H
#define SIGNALBLOCKS_IPORT_H

#include "BlockManager.h"
#include "TimeTick.h"

#include <assert.h>
#include <memory>
#include <vector>

namespace signalblocks {

    /// Matrix data is stored as shown in the example below, which is row-major order as in C 2D array.
    /// This format is intentionally choosen for compatibility with the C/C++ multidimensional arrays.
    ///
    /// Example:
    ///
    /// dims = {4, 2}
    /// data = { {a0, a1},
    ///          {a2, a3},
    ///          {a4, a5},
    ///          {a6, a7}
    ///         }
    ///
    /// That is the first dimension is stored and then the other
    /// dimension. For more than 2 dimensions this scales similarly
    /// and the first dimension is stored while assuming that all
    /// the other dimension are 0th index, and then subsequent
    /// dimensions follow.
    ///
    /// dims = {4, 3, 2}
    /// data = { { {a0, a1}, {a2, a3}, {a4, a5} },
    ///          { {a6, a7}, {a8, a9}, {a10, a11} },
    ///          { {a12, a13}, {a14, a15}, {a16, a17} },
    ///          { {a18, a19}, {a20, a21}, {a22, a23} }
    ///        }
    ///
    ///
    /// The following sample c++ code can read 3-D matrix (based on
    /// above format), which is provided for illustrative purpose only.
    ///
    /// std::vector<int> dims = {4, 3, 2};
    /// int data[4*3*2];  // some data
    /// for (int i = 0; i < dims[0]; ++i) {
    ///     for (int j = 0; j < dims[1]; ++j) {
    ///         for (int k = 0; k < dims[2]; ++k) {
    ///             std::cout << data[(i * (dims[1] * dims[2])) + (j * dims[2]) + k] << ",";
    ///         }
    ///     }
    /// }
    ///
    template<class T = float>
    class IPort {
    public:
        using type = T;

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

        virtual const BlockManager::BlockInfo& GetBlockInfo() = 0;

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

#endif // SIGNALBLOCKS_IPORT_H
