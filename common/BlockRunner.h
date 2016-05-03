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

#ifndef SIGNALBLOCKS_BLOCKRUNNER_H
#define SIGNALBLOCKS_BLOCKRUNNER_H

#include "BlockTypes.h"
#include "IPort.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace signalblocks {
    class BlockRunner {
    public:

        BlockRunner(const TimeTick& increment);
        
        bool Add(std::shared_ptr<IPort<char> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<unsigned char> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<short> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<unsigned short> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<int> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<unsigned int> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<long> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<unsigned long> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<float> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<double> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<long double> > source, const std::string& name);
        bool Add(std::shared_ptr<IPort<std::string> > source, const std::string& name);

        bool Remove(const std::string& name);

        // must be called repeatedly
        void Iterate();

    private:

        TimeTick mTimeTick;
        TimeTick mTimeTickIncrement;

        // Global database which archives all the source blocks and
        // their base data type. This is important to maintain uniqueness
        // across all the source blocks.
        std::unordered_map<std::string, BlockType> mBlocks;

        std::unordered_map<std::string, std::shared_ptr<IPort<char> > > mCharSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<unsigned char> > > mUcharSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<short> > > mShortSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<unsigned short> > > mUshortSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<int> > > mIntSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<unsigned int> > > mUintSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<long> > > mLongSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<unsigned long> > > mUlongSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<float> > > mFloatSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<double> > > mDoubleSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<long double> > > mLongDoubleSources;
        std::unordered_map<std::string, std::shared_ptr<IPort<std::string> > > mStringSources;
    };
}

#endif //SIGNALBLOCKS_BLOCKRUNNER_H
