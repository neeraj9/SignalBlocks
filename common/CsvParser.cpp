// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#include "CsvParser.h"
#include "logging.h"

#include <regex>

using namespace sigblocks;

namespace {
    static std::regex csv_regex(",");
    static std::regex quote_regex("\"");


    // split the line within CSV file, which can be quoted with double quotes (")
    // and fields are separated by comma (,).
    inline std::vector<std::string> CsvSplit(std::ifstream& instream,
                                             const std::string& filename,
                                             char quote = '"',
                                             char delim = ',') {
        std::string line;
        if (! std::getline(instream, line)) {
            return std::vector<std::string>();
        }
        std::vector<std::string> result;
        std::string leftover;
        bool done = false;
        do {
            unsigned long start_position = 0;
            unsigned long pos = 0;
            bool has_quote = false;
            for (const char* ptr = line.c_str(); *ptr != '\0'; ++ptr) {
                if (*ptr == quote) {
                    has_quote = (!has_quote);  // toggle quotes
                } else if (*ptr == delim) {
                    if (!has_quote) {
                        if (! leftover.empty()) {
                            leftover.append(std::move(line.substr(start_position, pos - start_position)));
                            if ((leftover[0] == quote) && (leftover[leftover.size() - 1] == quote)) {
                                result.push_back(leftover.substr(1, line.size() - 2));
                            } else {
                                result.push_back(leftover);
                            }
                            leftover.clear();
                        } else {
                            if ((line[start_position] == quote) && (line[pos - 1] == quote)) {
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
                if (!std::getline(instream, line)) {
                    LOG_ERROR("quoted field ends at the last line in file=%s\n", filename.c_str());
                    return std::vector<std::string>();
                }
                std::string tmp;
                if ((line[start_position] == quote) && (line[pos - 1] == quote)) {
                    std::string tmp(std::move(line.substr(start_position + 1, pos - start_position - 2)));
                    leftover.swap(tmp);
                } else {
                    std::string tmp(std::move(line.substr(start_position, pos - start_position)));
                    leftover.swap(tmp);
                }
                //std::string tmp(std::move(line.substr(start_position, pos - start_position)));
                //leftover.swap(tmp);
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
                        if ((line[start_position] == quote) && (line[pos - 1] == quote)) {
                            result.push_back(std::move(line.substr(start_position + 1, pos - start_position - 2)));
                        } else {
                            result.push_back(std::move(line.substr(start_position, pos - start_position)));
                        }
                        //result.push_back(std::move(line.substr(start_position, pos - start_position)));
                    }
                }
                // else nothing to add
                done = true;
            }
        } while(! done);

        return result;
    }
}


CsvParser::CsvParser(const std::string& filename, bool hasHeader)
        : mFilename(filename),
          mHasHeader(hasHeader),
          mInFile(filename),
          mHeader(),
          mLineNumber(0) {
    if (mHasHeader) {
        std::vector<std::string> fields(std::move(CsvSplit(mInFile, mFilename)));
        if (fields.empty()) {
            //LOG_ERROR("Cannot read anything from file=%s\n", filename.c_str());
            // probably the file do not exist
        } else {
            LOG_DEBUG("There are %lu header fields in file=%s\n", fields.size(), filename.c_str());
            mHeader.swap(fields);
            ++mLineNumber;
        }
    }
}

CsvParser::~CsvParser() {
    // we probably dont need it but still lets just do it
    if (mInFile.is_open()) {
        mInFile.close();
    }
}

bool CsvParser::IsOpen() const {
    return mInFile.is_open();
}

bool CsvParser::IsEof() const {
    return mInFile.eof();
}

const std::vector<std::string>& CsvParser::GetHeader() const {
    return mHeader;
}

std::vector<std::string> CsvParser::GetNext() {
    std::vector<std::string> dummy;
    if (mHasHeader && mHeader.empty()) {
        return dummy;
    }

    std::vector<std::string> fields(std::move(CsvSplit(mInFile, mFilename)));
    if (fields.empty()) {
        // nothing more to read
        LOG_DEBUG("read %d lines from file=%s\n", mLineNumber, mFilename.c_str());
        return dummy;
    }
    // else
    ++mLineNumber;

    // max fields is same as that of header
    if (mHasHeader && (fields.size() != mHeader.size())) {
        // ERROR because the size should match
        LOG_ERROR("line=%d has %lu fields while header has %lu\n",
                  mLineNumber, fields.size(), mHeader.size());
        return dummy;
    }
    // else all good

    return fields;
}

int CsvParser::GetLineNumber() const {
    return mLineNumber;
}
