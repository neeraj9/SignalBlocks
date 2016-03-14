// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include <assert.h>
#include "BasicTypeConverter.h"

#include "../../common/GenericCopy.h"

using namespace sigblocks;

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

template
class BasicTypeConverter<int, float>;
