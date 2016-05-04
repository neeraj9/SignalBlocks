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

#ifndef SIGNALBLOCKS_CHARORDINALCONVERTER_H
#define SIGNALBLOCKS_CHARORDINALCONVERTER_H

#include "../../common/MixedPort.h"

#include <tuple>
#include <unordered_map>

namespace signalblocks {
    class CharOrdinalConverter
            : public MixedPort<1, 1, char, unsigned long> {

    public:
        CharOrdinalConverter(std::string name);

        CharOrdinalConverter(std::string name,
                         std::unordered_map<std::string, unsigned long> initDict,
                         unsigned long nextOrdinalValue);

        std::tuple<unsigned long, bool> GetOrdinalValue(const std::string& key) const;
        unsigned long GetNextOrdinalValue() const;

        std::unordered_map<std::string, unsigned long> ExtractDictionary();

    protected: // Override MixedPort interfaces for Sink
        virtual void Process(int sourceIndex, const char& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<char[]> data, int len, const TimeTick& startTime);

        /// Matrix data is not supported and an assertion failure will follow
        /// when this interface is used.
        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<char[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime);

    private:
        std::unordered_map<std::string, unsigned long> mDictionary;
        unsigned long mNextOrdinalValue;
    };
}


#endif //SIGNALBLOCKS_CHARORDINALCONVERTER_H
