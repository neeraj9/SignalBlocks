// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGNALBLOCKS_CSVFILESOURCE_H
#define SIGNALBLOCKS_CSVFILESOURCE_H

#include "../../common/Port.h"
#include "../../common/CsvParser.h"

#include <string>

namespace sigblocks {
    class CsvFileSource
            : public Port<0, 1, std::string> {
    public:
        CsvFileSource(const std::string& filename, bool hasHeader);

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

#endif //SIGNALBLOCKS_CSVFILESOURCE_H
