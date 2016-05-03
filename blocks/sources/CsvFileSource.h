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

#ifndef SIGBLOCKS_CSVFILESOURCE_H
#define SIGBLOCKS_CSVFILESOURCE_H

#include "../../common/Port.h"
#include "../../common/CsvParser.h"

#include <string>

namespace signalblocks {
    class CsvFileSource
            : public Port<0, 1, std::string> {
    public:
        CsvFileSource(std::string name, const std::string& filename, bool hasHeader);

        const std::vector<std::string>& GetHeader() const;
        bool IsValid() const;
        bool IsDone() const;

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        TimeTick mLastTick;
        const std::string mFilename;
        CsvParser mParser;
    };
}

#endif //SIGBLOCKS_CSVFILESOURCE_H
