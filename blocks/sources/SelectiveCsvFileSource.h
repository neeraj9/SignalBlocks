// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGNALBLOCKS_SELECTIVECSVFILESOURCE_H
#define SIGNALBLOCKS_SELECTIVECSVFILESOURCE_H

#include "../../common/Port.h"
#include "../../common/CsvParser.h"
#include "../../common/logging.h"

#include <assert.h>
#include <string>

namespace sigblocks {
    template <int M>
    class SelectiveCsvFileSource
            : public Port<0, M, std::string> {
    public:
        SelectiveCsvFileSource(const std::string& filename, std::vector<std::string> selectColumns)
                : mLastTick(),
                  mFilename(filename),
                  mParser(filename, true),
                  mColumns(std::move(selectColumns)),
                  mColumnIndices() {
            assert(M == selectColumns.size());
            if (! mParser.IsOpen()) {
                LOG_ERROR("Cannot read/parse csv file = %s\n", filename.c_str());
            } else {
                // determine column indices
                const std::vector<std::string>& header = mParser.GetHeader();
                mColumnIndices.reserve(mColumns.size());
                for (size_t i = 0; i < mColumns.size(); ++i) {
                    for (auto iter = std::begin(header); iter != std::end(header); ++iter) {
                        if (*iter == mColumns[i]) {
                            mColumnIndices.push_back(static_cast<int>(iter - header.begin()));
                        } else {  // not found
                            mColumnIndices.push_back(-1);
                        }
                    }
                }
            }
        }


    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            if (mParser.IsOpen()) {
                LOG_DEBUG("SelectiveCsvFileSource is done processing!\n");
                return; // dont do anything
            }

            if (mLastTick == timeTick) {
                return;  // already processed the event
            }
            mLastTick = timeTick;

            std::vector<std::string> fields(std::move(mParser.GetNext()));
            std::string dummy;
            if (! fields.empty()) {
                for (size_t i = 0; i < mColumnIndices.size(); ++i) {
                    if (mColumnIndices[i] >= 0) {
                        this->LeakData(i, fields[mColumnIndices[i]], timeTick);
                    } else {
                        // column do not exist, so pass empty string to maintain data point
                        this->LeakData(i, dummy, timeTick);
                    }
                }
            }
        }

    private:
        TimeTick mLastTick;
        const std::string mFilename;
        CsvParser mParser;
        std::vector<std::string> mColumns;
        std::vector<int> mColumnIndices;
    };
}

#endif //SIGNALBLOCKS_SELECTIVECSVFILESOURCE_H
