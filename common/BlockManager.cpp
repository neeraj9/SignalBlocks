// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#include "BlockManager.h"

using namespace sigblocks;

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
