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

#include "BlockRunner.h"

using namespace signalblocks;

BlockRunner::BlockRunner(const TimeTick& increment)
        : mTimeTick(),
          mTimeTickIncrement(increment) {
}

bool BlockRunner::Add(std::shared_ptr<IPort<char> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_CHAR}).second;
    if (success) {
        mCharSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<unsigned char> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_UCHAR}).second;
    if (success) {
        mUcharSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<short> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_SHORT}).second;
    if (success) {
        mShortSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<unsigned short> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_USHORT}).second;
    if (success) {
        mUshortSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<int> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_INT}).second;
    if (success) {
        mIntSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<unsigned int> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_UINT}).second;
    if (success) {
        mUintSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<long> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_LONG}).second;
    if (success) {
        mLongSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<unsigned long> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_ULONG}).second;
    if (success) {
        mUlongSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<float> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_FLOAT}).second;
    if (success) {
        mFloatSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<double> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_DOUBLE}).second;
    if (success) {
        mDoubleSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<long double> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_LONG_DOUBLE}).second;
    if (success) {
        mLongDoubleSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<std::string> > source, const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_STRING}).second;
    if (success) {
        mStringSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Add(std::shared_ptr<IPort<std::complex<double> > > source,
                      const std::string& name) {
    bool success = mBlocks.insert({name, BLOCK_COMPLEX_DOUBLE}).second;
    if (success) {
        mComplexDoubleSources.insert({name, source});
        return true;
    }
    return false;
}

bool BlockRunner::Remove(const std::string& name) {
    auto element = mBlocks.find(name);
    if (element != mBlocks.end()) {
        switch (element->second) {
            case BLOCK_CHAR:
            {
                mCharSources.erase(name);
                break;
            }
            case BLOCK_UCHAR:
            {
                mUcharSources.erase(name);
                break;
            }
            case BLOCK_SHORT:
            {
                mShortSources.erase(name);
                break;
            }
            case BLOCK_USHORT:
            {
                mUshortSources.erase(name);
                break;
            }
            case BLOCK_INT:
            {
                mIntSources.erase(name);
                break;
            }
            case BLOCK_UINT:
            {
                mUintSources.erase(name);
                break;
            }
            case BLOCK_LONG:
            {
                mLongSources.erase(name);
                break;
            }
            case BLOCK_ULONG:
            {
                mUlongSources.erase(name);
                break;
            }
            case BLOCK_FLOAT:
            {
                mFloatSources.erase(name);
                break;
            }
            case BLOCK_DOUBLE:
            {
                mDoubleSources.erase(name);
                break;
            }
            case BLOCK_LONG_DOUBLE:
            {
                mLongDoubleSources.erase(name);
                break;
            }
            case BLOCK_STRING:
            {
                mStringSources.erase(name);
                break;
            }
            case BLOCK_COMPLEX_DOUBLE:
            {
                mComplexDoubleSources.erase(name);
                break;
            }
            // no default
        }
        mBlocks.erase(name);
        return true;
    }
    return false;
}

void BlockRunner::Iterate() {
    for (auto&& iter : mCharSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mUcharSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mShortSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mUshortSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mIntSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mUintSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mLongSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mUlongSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mFloatSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mDoubleSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mLongDoubleSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mStringSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    for (auto&& iter : mComplexDoubleSources) {
        iter.second->ClockCycle(mTimeTick);
    }

    mTimeTick += mTimeTickIncrement;
}
