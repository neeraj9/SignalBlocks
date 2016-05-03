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

#ifndef SIGBLOCKS_BASICTYPECONVERTER_H
#define SIGBLOCKS_BASICTYPECONVERTER_H

#include "../../common/MixedPort.h"

namespace sigblocks {
    template<class TFROM, class TTO>
    class BasicTypeConverter
            : public MixedPort<1, 1, TFROM, TTO> {
    public:
        BasicTypeConverter(std::string name);

    protected: // Override MixedPort interfaces for Sink
        virtual void Process(int sourceIndex, const TFROM& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<TFROM[]> data, int len, const TimeTick& startTime);

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<TFROM[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime);
    };
}

#endif // SIGBLOCKS_BASICTYPECONVERTER_H
