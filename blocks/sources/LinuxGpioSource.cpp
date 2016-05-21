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


#include "LinuxGpioSource.h"
#include "../../common/logging.h"

#include <cerrno>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>


#define DESCRIPTION "A GPIO source"

using namespace signalblocks;

namespace {

    constexpr int MAX_BUF = 16;
    constexpr int PATH_MAX_LEN = 64;

    /// This must be called prior to accessing information from the GPIO ports
    /// otherwise error will occur.
    std::tuple<bool, int> ReserveLinuxGpio(int gpio) {
        int err = 0;

        int fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd > 0) {
            char buf[MAX_BUF];
            sprintf(buf, "%d", gpio);
            ssize_t rc = write(fd, buf, strlen(buf));
            if (rc < 0) {
                err = errno;
            }
            close(fd);
        }
        else {
            err = errno;
        }

        if (err == 0) {
            return std::make_tuple(true, 0);
        }
        return std::make_tuple(false, err);
    }

    /// Unreserve the GPIO ports after done, which is typically at the end.
    std::tuple<bool, int> UnreserveLinuxGpio(int gpio) {
        int err = 0;

        int fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd > 0) {
            char buf[MAX_BUF];
            sprintf(buf, "%d", gpio);
            ssize_t rc = write(fd, buf, strlen(buf));
            if (rc < 0) {
                err = errno;
            }
            close(fd);
        }
        else {
            err = errno;
        }

        if (err == 0) {
            return std::make_tuple(true, 0);
        }
        return std::make_tuple(false, err);
    }

    int SetInputLinuxGpioDirection(int gpio)
    {
        char path[PATH_MAX_LEN];

        snprintf(path, PATH_MAX_LEN, "/sys/class/gpio/gpio%d/direction", gpio);
        int fd = open(path, O_WRONLY);
        if (fd < 0) {
            LOG_ERROR("Failed to open gpio=%d IN direction for writing!\n", gpio);
            return -1;
        }
        ssize_t rc = write(fd, "in", 2);
        if (rc < 0) {
            LOG_ERROR("Failed to set direction on gpio=%d!\n", gpio);
            close(fd);
            return -1;
        }
        close(fd);
        return 0;
    }

    int ReadLinuxGpioValue(int gpio)
    {
        char path[PATH_MAX_LEN];

        snprintf(path, PATH_MAX_LEN, "/sys/class/gpio/gpio%d/value", gpio);
        int fd = open(path, O_RDONLY);
        if (fd < 0) {
            LOG_ERROR("Failed to open gpio=%d for reading!\n", gpio);
            return -1;
        }
        char value = 0;
        ssize_t rc = read(fd, &value, 1);
        if (rc < 0) {
            LOG_ERROR("Failed to read from gpio=%d!\n", gpio);
            close(fd);
            return -1;
        }
        close(fd);
        // GPIO gives only 0 or 1 (as ascii char) since we are reading from sysfs
        return (value == '1' ? 1 : 0);
    }
}

LinuxGpioException::LinuxGpioException(int errorNumber, const std::string& msg)
        : mErrorNumber(errorNumber),
          mMsg(msg) {
}

const char* LinuxGpioException::what() const throw() {
    return mMsg.c_str();
}

int LinuxGpioException::GetErrorNumber() const {
    return mErrorNumber;
}

LinuxGpioSource::LinuxGpioSource(std::string name, int gpioPort) throw(LinuxGpioException)
        : Port<0, 1, char>(name, DESCRIPTION),
          mGpioPort(gpioPort) {
    bool success;
    int err;
    std::tie(success, err) = ReserveLinuxGpio(mGpioPort);
    if (!success) {
        throw LinuxGpioException(err, "Failed to reseve GPIO");
    }
    err = SetInputLinuxGpioDirection(mGpioPort);
    if (err != 0) {
        throw LinuxGpioException(errno, "Failed to set direction for GPIO");
    }
}

LinuxGpioSource::~LinuxGpioSource() {
    UnreserveLinuxGpio(mGpioPort);
}

void LinuxGpioSource::ClockCycle(const TimeTick& timeTick) {
    int result = ReadLinuxGpioValue(mGpioPort);
    if (result >= 0) {
        char value = static_cast<char>(result);
        this->LeakData(0, value, timeTick);
    }
    // else cannot read from gpio
}