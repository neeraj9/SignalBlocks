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

#ifndef SIGNALBLOCKS_GPIOSOURCE_H
#define SIGNALBLOCKS_GPIOSOURCE_H

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <tuple>
#include <vector>

namespace signalblocks {

    /// This must be called prior to accessing information from the IO ports
    /// otherwise error will occur.
    std::tuple<bool, int> GetLinuxIOPermissions(unsigned long startPort, unsigned long numPorts);

    /** LinuxIOSource Read from Linux IO port.
     * This source block has two specializations where
     * 1. T = unsigned char
     * 2. T = unsigned short
     *
     * The block reads from port and generates scalar data output.
     *
     * @important Client must call GetLinuxIOPermissions() prior to
     *            using this source block.
     *
     * @important The frequency with which the ClockCycle() is called
     *            is not under scrutiny since this is a read operation
     *            rather than a write operation.
     */
    template <class T>
    class LinuxIOSource
            : public Port<0, 1, T> {
    public:
        LinuxIOSource(std::string name, unsigned short gpioPort);

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        const unsigned short mGpioPort;
    };
}

#endif //SIGNALBLOCKS_GPIOSOURCE_H
