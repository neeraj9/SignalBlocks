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

#include "TextCsvParser.h"

#include "../../common/logging.h"

#include <algorithm>

using namespace signalblocks;

#define DESCRIPTION "A block which reads CSV data as input and splits into multiple fields"

TextCsvParser::TextCsvParser(
        std::string name, bool hasHeader, char quote, char delim)
        : Port<1, 1, std::string>(std::move(name), DESCRIPTION),
          mParser(quote, delim),
          mHasHeader(hasHeader),
          mHeader() {
}

const std::vector<std::string>& TextCsvParser::GetHeader() const {
    return mHeader;
}

void TextCsvParser::Process(int sourceIndex, const std::string& data, const TimeTick& startTime) {
    std::vector<std::string> fields = mParser.Split(data);
    if (!fields.empty()) {
        // even pass the header downstream
        if (mHasHeader && mHeader.empty()) {
            mHeader = fields;
        }
        int num_fields = static_cast<int>(fields.size());
        std::unique_ptr<std::string[]> dest(new std::string[num_fields]);
        for (int i = 0; i < num_fields; ++i) {
            (dest.get())[i].swap(fields[i]);
        }
        this->LeakData(sourceIndex, std::move(dest), num_fields, startTime);
    }
}

void TextCsvParser::Process(
        int sourceIndex, std::unique_ptr<std::string[]> data, int len, const TimeTick& startTime) {
    assert(0);  // not supported
}

void TextCsvParser::ProcessMatrix(
        int sourceIndex, std::unique_ptr<std::string[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
    assert(0);  // not supported
}