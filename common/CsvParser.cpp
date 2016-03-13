// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#include "CsvParser.h"
#include "logging.h"

#include <regex>

using namespace sigblocks;

namespace {
    static std::regex csv_regex(",");
    static std::regex quote_regex("\"");
}


CsvParser::CsvParser(const std::string& filename, bool hasHeader)
        : mFilename(filename),
          mHasHeader(hasHeader),
          mInFile(filename),
          mHeader(),
          mLineNumber(0) {
    if (mHasHeader) {
        std::string line;
        if (!std::getline(mInFile, line)) {
            //LOG_ERROR("Cannot read anything from file=%s\n", filename.c_str());
            // probably the file do not exist
        } else {
            std::sregex_token_iterator first(line.begin(), line.end(), csv_regex, -1);
            std::sregex_token_iterator last;
            std::vector<std::string> fields(first, last);
            //LOG_DEBUG("There are %lu header fields in file=%s\n", fields.size(), filename.c_str());
            //LOG_DEBUG("header = %s\n", line.c_str());
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

    std::string line;
    if (!std::getline(mInFile, line)) {
        // nothing more to read
        return dummy;
    }
    // else

    ++mLineNumber;

    std::sregex_token_iterator first(line.begin(), line.end(), csv_regex, -1);
    std::sregex_token_iterator last;
    std::vector<std::string> fields(first, last);

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
