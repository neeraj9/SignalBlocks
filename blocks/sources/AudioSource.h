// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGNALBLOCKS_AUDIOSOURCE_H
#define SIGNALBLOCKS_AUDIOSOURCE_H

#include "../../common/Port.h"
#include "../../common/SoundInfo.h"
#include "../../common/logging.h"

#include <assert.h>
#include <istream>
#include <string>
#include <stdint.h>

namespace sigblocks {

    /// This block generates matrix (2-D) output only, where the first
    /// dimension is the block size and the second dimension is the
    /// number of channels.
    ///
    /// The number of data types supported is limited to the following:
    /// * char
    /// * unsigned char
    /// * short
    /// * int
    /// * long
    /// * float
    /// * double
    ///
    /// If you want to support more types then enhance SoundInfo::Read()
    /// interface and overload for more data types.
    template <class T>
    class AudioSource
            : public Port<0, 1, T> {
    public:
        /// set enableNormalisation to false when you dont want
        /// integer audio samples to be normalised to [-1.0, 1.0]
        /// when read as float or double.
        AudioSource(std::string name,
                    int blockSize,
                    std::unique_ptr<std::istream> pIns,
                    bool enableNormalisation = true)
                : Port<0, 1, T>(std::move(name), "An audio source."),
                  mLastTick(),
                  mLoopOver(true),
                  mBlockSize(blockSize),
                  mSoundInfo(enableNormalisation) {

            bool success = mSoundInfo.OpenStream(std::move(pIns));
            if (!success) {
                assert(0);  // is there anything better which we can do?
            }
        }

        void Loop(bool loopOver) {
            mLoopOver = loopOver;
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            if (mSoundInfo.IsEof() && !mLoopOver) {
                LOG_ERROR("AudioSource is done processing!\n");
                return; // dont do anything
            }
            if (mLastTick == timeTick) {
                return;  // already processed the event
            }
            mLastTick = timeTick;

            // always generate matrix data, where first dimension is the data and
            // second dimension is the channel
            const int max_elements = mBlockSize * mSoundInfo.GetNumChannels();
            std::unique_ptr<T[]> data(new T[max_elements]);

            int elements_read = static_cast<int>(mSoundInfo.Read(data.get(), max_elements));
            if (elements_read <= 0) {
                // lets try again if looping is enabled
                if (mSoundInfo.IsEof() && mLoopOver) {
                    mSoundInfo.Rewind();
                    // lets try again
                    elements_read = static_cast<int>(mSoundInfo.Read(data.get(), max_elements));
                    if (elements_read <= 0) {
                        // still doesnt work, so give up
                        return;
                    }
                } else {
                    return;
                }
            }
            // The samples read are one for each channel and then next frame (or sample for
            // all channels), and so on. That is [{ch1, ch2}, {ch1, ch2}, ...] or
            // alternatively as C/C++ 2-d array:
            // T samples[num_samples][num_channels]
            std::vector<int> dims = {elements_read / mSoundInfo.GetNumChannels(),
                                     mSoundInfo.GetNumChannels()};
            this->LeakMatrix(0, std::move(data), dims, timeTick);
            if (mSoundInfo.IsEof() && mLoopOver) {
                mSoundInfo.Rewind();
            }
        }

    private:
        TimeTick mLastTick;
        bool mLoopOver;
        int mBlockSize;

        SoundInfo mSoundInfo;
    };
}

#endif //SIGNALBLOCKS_AUDIOSOURCE_H
