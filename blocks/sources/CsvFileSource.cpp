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

#include "CsvFileSource.h"
#include "../../common/logging.h"

#include <algorithm>

using namespace signalblocks;

#define DESCRIPTION "A source block which reads data from a CSV text file."

CsvFileSource::CsvFileSource(
        std::string name, const std::string& filename, bool hasHeader)
        : Port<0, 1, std::string>(std::move(name), DESCRIPTION),
          mLastTick(),
          mFilename(filename),
          mParser(filename, hasHeader) {
    if (! mParser.IsOpen()) {
        LOG_ERROR("Cannot read/parse csv file = %s\n", filename.c_str());
    }
}

const std::vector<std::string>& CsvFileSource::GetHeader() const {
    return mParser.GetHeader();
}

bool
CsvFileSource::IsValid() const {
    return mParser.IsOpen();
}

bool
CsvFileSource::IsDone() const {
    if (! IsValid()) {
        // when not valid then nothing else to do
        return true;
    }
    return mParser.IsEof();
}

void CsvFileSource::ClockCycle(const TimeTick& timeTick) {
    if (! mParser.IsOpen()) {
        LOG_DEBUG("CsvFileSource is done processing!\n");
        return; // dont do anything
    }

    if (mLastTick == timeTick) {
        return;  // already processed the event
    }
    mLastTick = timeTick;

    std::vector<std::string> fields = mParser.GetNext();
    if (! fields.empty()) {
        int num_fields = static_cast<int>(fields.size());
        std::unique_ptr<std::string[]> data(new std::string[num_fields]);
        for (size_t i = 0; i < fields.size(); ++i) {
            std::string* tmp = data.get();
            tmp[i].swap(fields[i]);
        }
        this->LeakData(0, std::move(data), num_fields, timeTick);
    }
}
