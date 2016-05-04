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

#include "CharOrdinalConverter.h"

using namespace signalblocks;

#define DESCRIPTION "An ordinal block which converts from char to a number."

CharOrdinalConverter::CharOrdinalConverter(std::string name)
        : MixedPort<1, 1, char, unsigned long>(std::move(name), DESCRIPTION),
          mDictionary(),
          mNextOrdinalValue(0) {

}

CharOrdinalConverter::CharOrdinalConverter(std::string name,
                                   std::unordered_map<std::string, unsigned long> initDict,
                                   unsigned long nextOrdinalValue)
        : MixedPort<1, 1, char, unsigned long>(std::move(name), DESCRIPTION),
          mDictionary(std::move(initDict)),
          mNextOrdinalValue(nextOrdinalValue) {
}

std::tuple<unsigned long, bool> CharOrdinalConverter::GetOrdinalValue(const std::string& key) const {
    auto iter = mDictionary.find(key);
    if (iter != mDictionary.end()) {
        unsigned long value = iter->second;
        return std::tuple<unsigned long, bool>(value, true);
    } else {
        return std::tuple<unsigned long, bool>(0, false);
    }
}

unsigned long CharOrdinalConverter::GetNextOrdinalValue() const {
    return mNextOrdinalValue;
}

std::unordered_map<std::string, unsigned long> CharOrdinalConverter::ExtractDictionary() {
    std::unordered_map<std::string, unsigned long> dict(std::move(mDictionary));
    return dict;
}

void CharOrdinalConverter::Process(int sourceIndex, const char& data, const TimeTick& startTime) {
    assert(sourceIndex == 0);
    unsigned long todata = 0;
    std::string text;
    text.push_back(data);
    auto iter = mDictionary.find(text);
    if (iter != mDictionary.end()) {
        todata = iter->second;
    } else {
        mDictionary.insert({text, mNextOrdinalValue});
        todata = mNextOrdinalValue;
        ++mNextOrdinalValue;  // assuming no overflow
    }

    this->GetAsSinkType()->ConsumeScalar(nullptr, todata, startTime);
}

void CharOrdinalConverter::Process(int sourceIndex, std::unique_ptr<char[]> data, int len,
                               const TimeTick& startTime) {
    assert(sourceIndex == 0);
    unsigned long todata = 0;
    std::string text(&data.get()[0], &data.get()[len]);
    auto iter = mDictionary.find(text);
    if (iter != mDictionary.end()) {
        todata = iter->second;
    } else {
        mDictionary.insert({text, mNextOrdinalValue});
        todata = mNextOrdinalValue;
        ++mNextOrdinalValue;  // assuming no overflow
    }

    this->GetAsSinkType()->ConsumeScalar(nullptr, todata, startTime);
}

void CharOrdinalConverter::ProcessMatrix(int sourceIndex,
                                     std::unique_ptr<char[]> data,
                                     const std::vector<int>& dims,
                                     const TimeTick& startTime) {
    assert(0);  // not supported
}