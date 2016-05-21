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


#include "LinuxIOSource.h"

#include <cerrno>
#include <sys/io.h>

#define DESCRIPTION "A Linux IO source"

namespace signalblocks {

    std::tuple<bool, int> GetLinuxIOPermissions(unsigned long startPort, unsigned long numPorts) {
        int rc = ioperm(startPort, numPorts, 1);
        if (rc < 0) {
            int err = errno;
            return std::make_tuple(false, err);
        }
        return std::make_tuple(true, 0);
    }

    template<>
    LinuxIOSource<unsigned char>::LinuxIOSource(std::string name, unsigned short ioPort)
            : Port<0, 1, unsigned char>(name, DESCRIPTION),
              mIoPort(ioPort) {
    }

    template<>
    void LinuxIOSource<unsigned char>::ClockCycle(const TimeTick& timeTick) {
        unsigned char value = inb(mIoPort);
        this->LeakData(0, value, timeTick);
    }


    template<>
    LinuxIOSource<unsigned short>::LinuxIOSource(std::string name, unsigned short ioPort)
            : Port<0, 1, unsigned short>(name, DESCRIPTION),
              mIoPort(ioPort) {
    }

    template<>
    void LinuxIOSource<unsigned short>::ClockCycle(const TimeTick& timeTick) {
        unsigned short value = inw(mIoPort);
        this->LeakData(0, value, timeTick);
    }
}