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

#ifndef SIGNALBLOCKS_SPLITWORDSBLOCK_H
#define SIGNALBLOCKS_SPLITWORDSBLOCK_H

#include "../../common/Port.h"

#include <memory>
#include <string>
#include <vector>

namespace signalblocks {

    /** SplitWordsBlock Splits a character stream into words (or vector or characters).
     *
     * The input is either a scalar (single character) or a vector (multiple characters),
     * while the output is always a vector (multiple characters forming a word.
     * The word delimiters are discarded. Additionally, matrix input is not allowed
     * and an assertion will follow if the source blocks feeds matrix input into
     * this block.
     *
     * This is a special block where words are sent as vector but they can be
     * sent with the same time tick. This suggests that time tick do not
     * hold much relevance in this respect. Take a look at the implementation
     * for further details. Additionally, note that the output of this
     * block can skip time ticks as well, which will be the case when
     * there is no word boundary in the input.
     *
     * @note the size of local storage (mArchive) is bounded (to 256), which
     *       works for the regular case.
     *       Having said that the archive is only used for incomplete
     *       words, which by definition must be bounded themselves, but
     *       this is not something which can be guaranteed for unknown
     *       data source. Any excees to the limit is treated as a reset
     *       signal and all the previous characters are discarded while
     *       new ones are archived.
     *
     * @note there is no validation for maximim length of a word.
     */
    class SplitWordsBlock
            : public Port<1, 1, char> {
    public:
        SplitWordsBlock(std::string name);

    protected: // Port interface
        virtual void Process(int sourceIndex, const char& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<char[]> data, int len, const TimeTick& startTime);

        /// this interface must not be used, so when it does
        /// an assertion will happen.
        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<char[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime);
    private:
        std::string mArchive;
    };
}

#endif //SIGNALBLOCKS_SPLITWORDSBLOCK_H