// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGBLOCKS_CSVPARSER_H
#define SIGBLOCKS_CSVPARSER_H

#include <fstream>
#include <string>
#include <vector>

namespace sigblocks {
    class CsvParser {
    public:
        CsvParser(const std::string& filename, bool hasHeader);
        ~CsvParser();

        bool IsOpen() const;
        bool IsEof() const;

        const std::vector<std::string>& GetHeader() const;

        std::vector<std::string> GetNext();
        int GetLineNumber() const;

    private:
        const std::string mFilename;
        bool mHasHeader;
        std::ifstream mInFile;
        std::vector<std::string> mHeader;
        int mLineNumber;
    };
}

#endif //SIGBLOCKS_CSVPARSER_H
