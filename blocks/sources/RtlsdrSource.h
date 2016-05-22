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

#ifndef SIGNALBLOCKS_RTLSDRSOURCE_H
#define SIGNALBLOCKS_RTLSDRSOURCE_H

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <complex>
#include <memory>
#include <stdint.h>
#include <stdexcept>

// struct rtlsdr_dev is internal to rtlsdr implementation
// and is not directly exposed via public header files.
typedef struct rtlsdr_dev rtlsdr_dev_t;

namespace signalblocks {

    class RtlsdrSourceException : public std::exception {
    public:
        RtlsdrSourceException(const std::string& msg);

        // std::exception interface
        virtual const char* what() const throw();

    private:
        const std::string mMsg;
    };

    /** RtlsdrSource A source block which reads from a RTLSDR device.
     * This source block depends on the rtlsdr library to read from
     * a compatible device. The output is complex<double> while the
     * rtlsdr device generates 8bit I,Q data samples. The conversion
     * to complex<double> is done as follows:
     *
     * Real Value = ((I - 127.4) / 128.0)
     * Imaginary Value = ((Q - 127.4) / 128.0)
     *
     * The weird (-127.4) is copied as-is from the Gnu Radio project,
     * but the intent is to convert from integer values to floating
     * point number in (-1, 1) range.
     *
     * Note that the range of I and Q is [0, 255].
     */
    class RtlsdrSource
            : Port<0, 1, std::complex<double> > {
    public:
        RtlsdrSource(std::string name,
                     std::string serialNumber,
                     uint32_t sampleRateHz);

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        std::string mSerialNumber;
        int mDeviceIndex;
        rtlsdr_dev_t* mpRtlSdrDev;
        std::unique_ptr<uint8_t[]> mpBuffer;
    };
}

#endif //SIGNALBLOCKS_RTLSDRSOURCE_H
