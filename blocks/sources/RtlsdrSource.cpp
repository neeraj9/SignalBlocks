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

#include "RtlsdrSource.h"
#include "../../common/logging.h"

#include <cstring>
#include <rtl-sdr.h>

using namespace signalblocks;

namespace {
    constexpr int MAX_RTLSDR_DEVICE_INFO_BYTES = 256;
    constexpr int MAX_INTERNAL_BUFFER_BYTES = 512 * 512;
}

RtlsdrSourceException::RtlsdrSourceException(const std::string& msg)
        : mMsg(msg) {
}

const char* RtlsdrSourceException::what() const throw() {
    return mMsg.c_str();
}

RtlsdrSource::RtlsdrSource(std::string name,
                           std::string serialNumber,
                           uint32_t sampleRateHz)
        : Port<0, 1, std::complex<double> >(std::move(name), "A RTLSDR source block."),
          mSerialNumber(std::move(serialNumber)),
          mDeviceIndex(-1),
          mpRtlSdrDev(nullptr),
          mpBuffer(new uint8_t[MAX_INTERNAL_BUFFER_BYTES]) {
    mDeviceIndex = rtlsdr_get_index_by_serial(mSerialNumber.c_str());
    int max_num_rtlsdr_devices = rtlsdr_get_device_count();
    if ((mDeviceIndex >= 0) && (mDeviceIndex < max_num_rtlsdr_devices)) {
        char manufacturer[MAX_RTLSDR_DEVICE_INFO_BYTES];
        char product[MAX_RTLSDR_DEVICE_INFO_BYTES];
        char serial[MAX_RTLSDR_DEVICE_INFO_BYTES];

        ::memset(manufacturer, 0, MAX_RTLSDR_DEVICE_INFO_BYTES);
        ::memset(product, 0, MAX_RTLSDR_DEVICE_INFO_BYTES);
        ::memset(serial, 0, MAX_RTLSDR_DEVICE_INFO_BYTES);
        int rc = rtlsdr_get_device_usb_strings(static_cast<uint32_t>(mDeviceIndex),
                                               manufacturer,
                                               product,
                                               serial);
        if (rc >= 0) {
            LOG_DEBUG("Using device index=%d for rtlsdr device with serial=%s"
                              "manufacturer=%s, product=%s\n",
                      mDeviceIndex,
                      serial,
                      manufacturer,
                      product);
        } else {
            LOG_DEBUG("Using device index=%d for rtlsdr device with serial=%s\n",
                      mDeviceIndex,
                      mSerialNumber.c_str());
        }
        rc = rtlsdr_open(&mpRtlSdrDev, mDeviceIndex);
        if (rc >= 0) {
            // rtlsdr_set_direct_sampling
            // rtlsdr_set_offset_tuning
            // rtlsdr_set_xtal_freq
            // rtlsdr_set_tuner_if_gain (should we do more for RTLSDR E4000?)

            int manual_gain_mode = 0;  // set automatic gain mode
            rc = rtlsdr_set_tuner_gain_mode(mpRtlSdrDev, manual_gain_mode);
            if (rc < 0) {
                LOG_ERROR("rtlsdr_set_tuner_gain_mode() failed on rtlsdr with serial=%s\n",
                          mSerialNumber.c_str());
                throw RtlsdrSourceException(
                        "rtlsdr_set_tuner_gain_mode() failed on rtlsdr");
            }

            rc = rtlsdr_set_agc_mode(mpRtlSdrDev, !manual_gain_mode);
            if (rc < 0) {
                LOG_ERROR("rtlsdr_set_agc_mode() failed on rtlsdr with serial=%s\n",
                          mSerialNumber.c_str());
                throw RtlsdrSourceException(
                        "rtlsdr_set_agc_mode() failed on rtlsdr");
            }

            rc = rtlsdr_set_sample_rate(mpRtlSdrDev, sampleRateHz);
            if (rc < 0) {
                LOG_ERROR("rtlsdr_set_sample_rate() failed on rtlsdr with serial=%s\n",
                          mSerialNumber.c_str());
                throw RtlsdrSourceException(
                        "rtlsdr_set_sample_rate() failed on rtlsdr");
            }

            rc = rtlsdr_reset_buffer(mpRtlSdrDev);
            if (rc < 0) {
                LOG_ERROR("rtlsdr_reset_buffer() failed on rtlsdr with serial=%s\n",
                          mSerialNumber.c_str());
                throw RtlsdrSourceException(
                        "rtlsdr_reset_buffer() failed on rtlsdr");
            }
        } else {
            LOG_ERROR("rtlsdr_open() failed.\n");
            throw RtlsdrSourceException("rtlsdr_open() failed");
        }
    } else {
        if (mDeviceIndex < 0) {
            LOG_ERROR("Cannot find rtlsdr device with serial=%s\n",
                      mSerialNumber.c_str());
            throw RtlsdrSourceException("Cannot find rtlsdr device");
        } else {
            LOG_ERROR("Wrong index discovered for rtlsdr device with serial=%s\n",
                      mSerialNumber.c_str());
            throw RtlsdrSourceException("Wrong index found for rtlsdr device");
        }
    }
}

void RtlsdrSource::ClockCycle(const TimeTick& timeTick) {
    // TODO should we skip initial set of samples before
    // making sense of the data read from the device?
    int bytes_read = 0;
    uint32_t timeout_millisec = 1;  // this is the minimum possible
    int rc = rtlsdr_read_timed_sync(mpRtlSdrDev,
                                    mpBuffer.get(),
                                    MAX_INTERNAL_BUFFER_BYTES,
                                    &bytes_read,
                                    timeout_millisec);
    if ((rc < 0) && (bytes_read <= 0)) {
        // nothing read
        return;
    }
    // else read something or everything
    int num_complex_samples = bytes_read / 2;  // 8bit I,Q
    std::unique_ptr<std::complex<double>[]> data(new std::complex<double>[num_complex_samples]);
    // HINT: could use lookup table to convert {I,Q} pair to complex<double>
    // so that there is no computation each time samples are read from
    // the device. There is a memory trade-off though not that much (2^16 lookup).
    for (int i = 0; i < num_complex_samples; ++i) {
        double real = (mpBuffer[2*i] - 127.4) / 128.0;
        double imag = (mpBuffer[2*i + 1] - 127.4) / 128.0;
        data[i] = std::complex<double>(real, imag);
    }
    // always produce vector output
    this->LeakData(0, std::move(data), num_complex_samples, timeTick);
}
