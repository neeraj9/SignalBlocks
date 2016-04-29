// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
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

template
class BasicTypeConverter<int, float>;
