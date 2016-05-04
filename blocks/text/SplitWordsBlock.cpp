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

#include "SplitWordsBlock.h"
#include "../../common/logging.h"

#include <cassert>
#include <ctype.h>

using namespace signalblocks;

namespace {
    constexpr int MAX_LOCAL_STORAGE_BYTES = 256;
}

SplitWordsBlock::SplitWordsBlock(std::string name)
        : Port<1, 1, char>(std::move(name), "A split words block"),
          mArchive() {
}

void SplitWordsBlock::Process(int sourceIndex, const char& data, const TimeTick& startTime) {
    if (isalnum(data)) {
        if (mArchive.size() >= MAX_LOCAL_STORAGE_BYTES) {
            LOG_WARN("Word limit exceeded, dropping previous archive (half-word)\n");
            mArchive.clear();  // limit reached
        }
        mArchive.push_back(data);
    } else if (!mArchive.empty()) {
        // word terminator
        std::unique_ptr<char[]> tmp_data(new char[mArchive.size()]);
        std::copy(mArchive.begin(), mArchive.end(), tmp_data.get());
        int len = static_cast<int>(mArchive.size());
        mArchive.clear();
        this->LeakData(sourceIndex, std::move(tmp_data), len, startTime);
    }
    // else nothing to send
}

void SplitWordsBlock::Process(
        int sourceIndex, std::unique_ptr<char[]> data, int len, const TimeTick& startTime) {
    bool is_terminator = false;
    for (int i = 0; i < len; ++i) {
        if (!isalnum(data[i])) {
            is_terminator = true;
            break;
        }
    }
    if (!is_terminator) {
        // expand existing archive and buffer new text because
        // the word continues
        // Note: There is no max-width of a word checking here.
        if (mArchive.size() + len >= MAX_LOCAL_STORAGE_BYTES) {
            LOG_WARN("Word limit exceeded for vector input, dropping previous archive (half-word)\n");
            mArchive.clear();
        }
        size_t old_len = mArchive.size();
        mArchive.resize(old_len + len, ' ');
        for (int i = 0; i < len; ++i) {
            mArchive[old_len + i] = data[i];
        }
    } else {
        // word terminator exists within the new text
        int i = 0;
        int offset = 0;
        while (i < len) {
            if (!isalnum(data[i])) {
                // this can be pulled out of the loop, since its usefulness
                // is only for the first time (TODO)
                int archive_len = static_cast<int>(mArchive.size());
                // dont include data[i] (or the delimiter itself)
                int tmp_len = static_cast<int>(archive_len + (i - offset));
                std::unique_ptr<char[]> tmp_data(new char[tmp_len]);
                if (!mArchive.empty()) {
                    std::copy(mArchive.begin(), mArchive.end(), tmp_data.get());
                    mArchive.clear();
                }
                // dont include delimiter or data[i]
                std::copy(&data.get()[offset], &data.get()[i], &tmp_data.get()[archive_len]);
                // This is a special case where words are sent as vector but with the
                // same time tick, so time tick do not hold much relevance in this respect.
                this->LeakData(sourceIndex, std::move(tmp_data), tmp_len, startTime);
                while (!isalnum(data[i]) && (i < len)) {
                    ++i;
                }
                offset = i;
            } else {
                ++i;
            }
        }
        if (offset < len) {
            if ((len - offset) < MAX_LOCAL_STORAGE_BYTES) {
                // there is still some characters which remains, so
                // archive those
                std::string tmp(&data.get()[offset], &data.get()[len]);
                mArchive.swap(tmp);
            } else {
                LOG_WARN("Word limit exceeded for leftover input, dropping it completely.\n");
            }
        }
    }
}

void SplitWordsBlock::ProcessMatrix(int sourceIndex,
                                    std::unique_ptr<char[]> data,
                                    const std::vector<int>& dims,
                                    const TimeTick& startTime) {
    assert(0);  // not supported
}