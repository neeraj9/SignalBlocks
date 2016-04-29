// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "OrdinalConverter.h"

using namespace sigblocks;

#define DESCRIPTION "An ordinal block which converts from string to a number."

OrdinalConverter::OrdinalConverter(std::string name)
        : MixedPort<1, 1, std::string, unsigned long>(std::move(name), DESCRIPTION),
          mDictionary(),
          mNextOrdinalValue(0) {

}

OrdinalConverter::OrdinalConverter(std::string name,
                                   std::unordered_map<std::string, unsigned long> initDict,
                                   unsigned long nextOrdinalValue)
        : MixedPort<1, 1, std::string, unsigned long>(std::move(name), DESCRIPTION),
          mDictionary(std::move(initDict)),
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

void OrdinalConverter::ProcessMatrix(int sourceIndex,
                                     std::unique_ptr<std::string[]> data,
                                     const std::vector<int>& dims,
                                     const TimeTick& startTime) {
    assert(sourceIndex == 0);
    assert(! dims.empty());
    int len = dims[0];
    for (size_t i = 1; i < dims.size(); ++i) {
        len *= dims[i];
    }
    assert(len > 0);
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
    this->GetAsSinkType()->ConsumeMatrix(nullptr, std::move(todata), dims, startTime);
}