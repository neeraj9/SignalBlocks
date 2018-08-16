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

#ifndef SIGNALBLOCKS_LINUXGPIOSOURCE_H
#define SIGNALBLOCKS_LINUXGPIOSOURCE_H

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <stdexcept>
#include <tuple>
#include <vector>

namespace signalblocks {

    class LinuxGpioException : public std::exception {
    public:
        LinuxGpioException(int errorNumber, const std::string& msg);

        // std::exception interface
        virtual const char* what() const noexcept(true);

        int GetErrorNumber() const;
    private:
        int mErrorNumber;
        const std::string mMsg;
    };

    /** LinuxGpioSource Read from Linux Gpio port.
     *
     * The block reads from port and generates scalar data output of
     * type char with either 0 or 1 value indicating LOW or HIGH on
     * the GPIO.
     *
     * Note that internal implementation uses the Linux sysfs interface,
     * that is reading/writing via /sys/class/gpio/  set of files.
     *
     */
    class LinuxGpioSource
            : public Port<0, 1, char> {
    public:
        // @throw LinuxGpioException
        LinuxGpioSource(std::string name, int gpioPort) noexcept(false);

        virtual ~LinuxGpioSource();

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        const int mGpioPort;
    };
}

#endif //SIGNALBLOCKS_LINUXGPIOSOURCE_H
