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

#ifndef SIGBLOCKS_OSTREAMSINK_H
#define SIGBLOCKS_OSTREAMSINK_H

#include "../../common/Port.h"

#include <ostream>
#include <memory>

namespace signalblocks {
    template<class T>
    class OstreamSink
            : public Port<1, 0, T> {
    public:
        OstreamSink(std::string name);

        OstreamSink(std::string name, std::unique_ptr<std::ostream> pIns);

        void SetStreamSink(std::unique_ptr<std::ostream> pIns);

    public: // Port interface
        void Process(int sourceIndex, const T& data, const TimeTick& startTime);

        void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime);

    private:
        std::unique_ptr<std::ostream> mpOstream;
    };
}

#endif // SIGBLOCKS_OSTREAMSINK_H
