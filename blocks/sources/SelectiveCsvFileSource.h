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

#ifndef SIGBLOCKS_SELECTIVECSVFILESOURCE_H
#define SIGBLOCKS_SELECTIVECSVFILESOURCE_H

#include "../../common/Port.h"
#include "../../common/CsvParser.h"
#include "../../common/logging.h"

#include <assert.h>
#include <string>

namespace signalblocks {
    template <int M>
    class SelectiveCsvFileSource
            : public Port<0, M, std::string> {
    public:
        SelectiveCsvFileSource(std::string name,
                               const std::string& filename,
                               std::vector<std::string> selectColumns)
                : Port<0, M, std::string>(std::move(name), "A selective csv file source."),
                  mLastTick(),
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

#endif //SIGBLOCKS_SELECTIVECSVFILESOURCE_H
