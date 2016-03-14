// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#include "CsvFileSource.h"
#include "../../common/logging.h"

#include <algorithm>

using namespace sigblocks;

CsvFileSource::CsvFileSource(const std::string& filename, bool hasHeader)
        : mLastTick(),
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

    std::vector<std::string> fields(std::move(mParser.GetNext()));
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
