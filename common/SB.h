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

#ifndef SIGNALBLOCKS_SB_H
#define SIGNALBLOCKS_SB_H

#include "IPort.h"

#include <memory>

namespace signalblocks {

    /// A alternative way to create blocks. See the example below for reference.
    ///
    /// auto blk = CreateBlock<Terminator<BaseDataType>, std::string>(std::string("block"));
    ///
    template<typename T, typename... Targs>
    inline std::shared_ptr<IPort<typename T::type> > CreateBlock(Targs&&... args) {
        std::shared_ptr<IPort<typename T::type> > block(new T(std::forward<Targs>(args)...));
        return block;
    }

}

#endif //SIGNALBLOCKS_SB_H
