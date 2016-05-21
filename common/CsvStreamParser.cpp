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

#include "CsvStreamParser.h"
#include "logging.h"

using namespace signalblocks;

CsvStreamParser::CsvStreamParser()
        : mQuote('"'),
          mDelim(','),
          mLeftover(),
          mPartialFields() {
}

CsvStreamParser::CsvStreamParser(char quote, char delim)
        : mQuote(quote),
          mDelim(delim),
          mLeftover(),
          mPartialFields() {
}

// Needless to say the implementation looks a lot complex than
// it probably is supposed to be.
// Think about refactoring and cleaning up the complexities a bit.
std::vector<std::string> CsvStreamParser::Split(const std::string& line) {
    std::vector<std::string> result(std::move(mPartialFields));
    std::string leftover(std::move(mLeftover));
    unsigned long start_position = 0;
    unsigned long pos = 0;
    bool has_quote = !(leftover.empty());
    for (const char* ptr = line.c_str(); *ptr != '\0'; ++ptr) {
        if (*ptr == mQuote) {
            has_quote = (!has_quote);  // toggle quotes
        } else if (*ptr == mDelim) {
            if (!has_quote) {
                if (! leftover.empty()) {
                    leftover.append(" "); // separate with space for multi-line spanning quoted fields
                    leftover.append(std::move(line.substr(start_position, pos - start_position)));
                    if ((leftover[0] == mQuote) && (leftover[leftover.size() - 1] == mQuote)) {
                        result.push_back(leftover.substr(1, line.size() - 2));
                    } else {
                        result.push_back(leftover);
                    }
                    leftover.clear();
                } else {
                    if ((line[start_position] == mQuote) && (line[pos - 1] == mQuote)) {
                        result.push_back(std::move(line.substr(start_position + 1, pos - start_position - 2)));
                    } else {
                        result.push_back(std::move(line.substr(start_position, pos - start_position)));
                    }
                }
                start_position = pos + 1;  // skip over the delimiter character
            } else {
                // ignore the delimiter because its quoted
            }
        }
        ++pos;
    }
    // if open quote then it appears that a field has a newline char.
    if (has_quote) {
        if ((line[start_position] == mQuote) && (line[pos - 1] == mQuote)) {
            std::string tmp(std::move(line.substr(start_position + 1, pos - start_position - 2)));
            if (leftover.empty()) {
                mLeftover.swap(tmp);
            } else {
                mLeftover.swap(leftover);
                mLeftover.append(tmp);
            }
        } else {
            std::string tmp(std::move(line.substr(start_position, pos - start_position)));
            if (leftover.empty()) {
                mLeftover.swap(tmp);
            } else {
                mLeftover.swap(leftover);
                mLeftover.append(tmp);
            }
        }
        //std::string tmp(std::move(line.substr(start_position, pos - start_position)));
        //leftover.swap(tmp);
        mPartialFields.swap(result);
        return std::vector<std::string>();
    } else {
        if (start_position < pos) {
            // validate the last field because there are cases
            // where these are just newline characters
            const char *tmp = line.c_str();
            bool valid_field = false;
            for (unsigned long i = start_position; i < pos; ++i) {
                if ((tmp[i] != '\r') && (tmp[i] != '\n')) {
                    valid_field = true;
                    break;
                }
            }
            if (valid_field) {
                if ((line[start_position] == mQuote) && (line[pos - 1] == mQuote)) {
                    result.push_back(std::move(line.substr(start_position + 1, pos - start_position - 2)));
                } else {
                    result.push_back(std::move(line.substr(start_position, pos - start_position)));
                }
                //result.push_back(std::move(line.substr(start_position, pos - start_position)));
            }
        }
        // else nothing to add
    }
    return result;
}