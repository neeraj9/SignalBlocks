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

#include <assert.h>
#include "BasicTypeConverter.h"

#include "../../common/GenericCopy.h"

using namespace sigblocks;

#define DESCRIPTION "A basic type converter block."

template<class TFROM, class TTO>
BasicTypeConverter<TFROM, TTO>::BasicTypeConverter(std::string name)
        : MixedPort<1, 1, TFROM, TTO>(std::move(name), DESCRIPTION) {
}

template<class TFROM, class TTO>
void
BasicTypeConverter<TFROM, TTO>::Process(
        int sourceIndex,
        const TFROM& data,
        const TimeTick& startTime) {
    assert(sourceIndex == 0);
    const TTO todata = static_cast<const TTO>(data);
    this->GetAsSinkType()->ConsumeScalar(nullptr, todata, startTime);
}

template<class TFROM, class TTO>
void
BasicTypeConverter<TFROM, TTO>::Process(
        int sourceIndex, std::unique_ptr<TFROM[]> data, int len, const TimeTick& startTime) {
    assert(sourceIndex == 0);
    std::unique_ptr<TTO[]> todata(new TTO[len]);
    GenericCopy<TFROM, TTO>::Copy(data.get(), data.get() + len, todata.get());
    this->GetAsSinkType()->ConsumeVector(nullptr, std::move(todata), len, startTime);
}

template<class TFROM, class TTO>
void
BasicTypeConverter<TFROM, TTO>::ProcessMatrix(int sourceIndex,
                                              std::unique_ptr<TFROM[]> data,
                                              const std::vector<int>& dims,
                                              const TimeTick& startTime) {
    assert(sourceIndex == 0);
    assert(! dims.empty());
    int len = dims[0];
    for (size_t i = 1; i < dims.size(); ++i) {
        len *= dims[i];
    }
    assert(len > 0);
    std::unique_ptr<TTO[]> todata(new TTO[len]);
    GenericCopy<TFROM, TTO>::Copy(data.get(), data.get() + len, todata.get());
    this->GetAsSinkType()->ConsumeMatrix(nullptr, std::move(todata), dims, startTime);
}

namespace sigblocks {
    template
    class BasicTypeConverter<int, float>;
}
