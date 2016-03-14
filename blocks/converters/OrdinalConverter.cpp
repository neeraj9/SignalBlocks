// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "OrdinalConverter.h"

using namespace sigblocks;


OrdinalConverter::OrdinalConverter()
        : mDictionary(),
          mNextOrdinalValue(0) {

}

OrdinalConverter::OrdinalConverter(std::unordered_map<std::string, unsigned long> initDict,
                                   unsigned long nextOrdinalValue)
        : mDictionary(std::move(initDict)),
          mNextOrdinalValue(nextOrdinalValue) {
}

std::tuple<unsigned long, bool> OrdinalConverter::GetOrdinalValue(const std::string& key) const {
    auto iter = mDictionary.find(key);
    if (iter != mDictionary.end()) {
        unsigned long value = iter->second;
        return std::tuple<unsigned long, bool>(value, true);
    } else {
        return std::tuple<unsigned long, bool>(0, false);
    }
}

unsigned long OrdinalConverter::GetNextOrdinalValue() const {
    return mNextOrdinalValue;
}

std::unordered_map<std::string, unsigned long> OrdinalConverter::ExtractDictionary() {
    std::unordered_map<std::string, unsigned long> dict(std::move(mDictionary));
    return dict;
}

void OrdinalConverter::Process(int sourceIndex, const std::string& data, const TimeTick& startTime) {
    assert(sourceIndex == 0);
    unsigned long todata = 0;
    auto iter = mDictionary.find(data);
    if (iter != mDictionary.end()) {
        todata = iter->second;
    } else {
        mDictionary.insert({data, mNextOrdinalValue});
        todata = mNextOrdinalValue;
        ++mNextOrdinalValue;  // assuming no overflow
    }

    this->GetAsSinkType()->ConsumeScalar(nullptr, todata, startTime);
}

void OrdinalConverter::Process(int sourceIndex, std::unique_ptr<std::string[]> data, int len,
                               const TimeTick& startTime) {
    assert(sourceIndex == 0);
    std::unique_ptr<unsigned long[]> todata(new unsigned long[len]);
    for (int i = 0; i < len; ++i) {
        auto iter = mDictionary.find(data[i]);
        if (iter != mDictionary.end()) {
            todata[i] = iter->second;
        } else {
            mDictionary.insert({data[i], mNextOrdinalValue});
            todata[i] = mNextOrdinalValue;
            ++mNextOrdinalValue;  // assuming no overflow
        }
    }
    this->GetAsSinkType()->ConsumeVector(nullptr, std::move(todata), len, startTime);
}
