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

#include "BlockManager.h"

using namespace signalblocks;

namespace {
    thread_local BlockManager* pBlockManager = nullptr;
}

BlockManager::BlockInfo BlockManager::BlockInfo::INVALID_BLOCK_INFO(
        "", "", BLOCK_TYPE_INVALID, 0, 0);


BlockManager* BlockManager::Getinstance() {
    if (pBlockManager == nullptr) {
        pBlockManager = new BlockManager();
    }
    return pBlockManager;
}


BlockManager::BlockInfo::BlockInfo(
        std::string name, std::string description, BlockType blockType, size_t numSources, size_t numSinks)
        : mName(std::move(name)),
          mDescription(std::move(description)),
          mBlockType(blockType),
          mSourceId(numSources, -1),
          mSinkId(numSinks, -1) {
}



BlockManager::BlockManager()
        : mBlocks(),
          mNextAvailableId(0) {

}

uint64_t
BlockManager::Add(BlockManager::BlockInfoFuncType infoFunc) {
    uint64_t id = mNextAvailableId;

    mBlocks.insert(std::make_pair(id, std::move(infoFunc)));
    ++mNextAvailableId;
    return id;
}

bool BlockManager::Remove(uint64_t id) {
    return (mBlocks.erase(id) > 0);
}

const BlockManager::BlockInfo&
BlockManager::GetBlockInfo(uint64_t id) const {
    auto iter = mBlocks.find(id);
    if (iter != mBlocks.end()) {
        return iter->second();
    }
    return BlockInfo::INVALID_BLOCK_INFO;
}
