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

#ifndef SIGNALBLOCKS_TEXTCSVPARSER_H
#define SIGNALBLOCKS_TEXTCSVPARSER_H

#include "../../common/Port.h"
#include "../../common/CsvStreamParser.h"

#include <string>

namespace signalblocks {
    /** TextCsvParser Splits an input line into multiple parts.
     *
     * Split an input line (without '\r' and '\n') into multiple
     * parts, while generating vector output for scalar input.
     * This block do not accept vector input and neither it processes
     * matrix input. Additionally, this block only generates vector
     * output because it will always split a line into multiple parts.
     *
     * @important always connect the SplitLineBlock and
     *            BasicTypeConverter<char, std::string>
     *            to this block or something which behaves similar because
     *            there is a strict requirement for no '\r' and '\n' and data
     *            being content for a single line only.
     */
    class TextCsvParser
            : public Port<1, 1, std::string> {
    public:
        TextCsvParser(std::string name,
                      bool hasHeader,
                      char quote = '"',
                      char delim = ',');

        const std::vector<std::string>& GetHeader() const;

    protected: // Port interface

        /// IMPORTANT: data must not contain '\r' pr '\n' and it
        ///            is the responsibility of the previous block
        ///            to remove those characters while framing a
        ///            line and passing it downstream.
        virtual void Process(int sourceIndex,
                             const std::string& data,
                             const TimeTick& startTime);

        // not supported
        virtual void Process(int sourceIndex,
                             std::unique_ptr<std::string[]> data,
                             int len,
                             const TimeTick& startTime);

        // not supported
        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<std::string[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime);

    private:
        TimeTick mLastTick;
        CsvStreamParser mParser;
        bool mHasHeader;
        std::vector<std::string> mHeader;
    };
}

#endif //SIGNALBLOCKS_TEXTCSVPARSER_H
