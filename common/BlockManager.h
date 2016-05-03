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

#ifndef SIGNALBLOCKS_BLOCKMANAGER_H
#define SIGNALBLOCKS_BLOCKMANAGER_H

#include "BlockTypes.h"

#include <functional>
#include <unordered_map>
#include <memory>
#include <string>
#include <stdint.h>
#include <vector>

namespace sigblocks {
    class BlockManager {
    public:
        struct BlockInfo {
            static BlockInfo INVALID_BLOCK_INFO;

            BlockInfo(std::string name,
                      std::string description,
                      BlockType blockType,
                      size_t numSources,
                      size_t numSinks);

            uint64_t mId;
            std::string mName;
            std::string mDescription;
            BlockType mBlockType;
            std::vector<int> mSourceId;
            std::vector<int> mSinkId;
        };

        using BlockInfoFuncType = std::function<const BlockInfo&()>;

    public:
        static BlockManager* Getinstance();

        uint64_t Add(BlockInfoFuncType infoFunc);
        bool Remove(uint64_t id);

        const BlockInfo& GetBlockInfo(uint64_t id) const;

    private:
        // cannot be instantiated directly
        BlockManager();

    private:
        std::unordered_map<int, BlockInfoFuncType> mBlocks;
        uint64_t mNextAvailableId;
    };
}

#endif //SIGNALBLOCKS_BLOCKMANAGER_H
