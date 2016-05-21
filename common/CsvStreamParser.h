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

#ifndef SIGNALBLOCKS_CSVSTREAMPARSER_H
#define SIGNALBLOCKS_CSVSTREAMPARSER_H

#include <vector>
#include <string>

namespace signalblocks {
    class CsvStreamParser {
    public:
        CsvStreamParser();
        CsvStreamParser(char quote, char delim);

        ///
        /// @param line contains a line worth of data, but it should
        ///        not contain '\r' or '\n'. This is important and
        ///        these characters must be removed before passing
        ///        it here.
        std::vector<std::string> Split(const std::string& line);

    private:
        char mQuote;
        char mDelim;
        std::string mLeftover;
        std::vector<std::string> mPartialFields;
    };
}

#endif //SIGNALBLOCKS_CSVSTREAMPARSER_H
