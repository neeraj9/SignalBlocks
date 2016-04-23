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
    /// dimension is the number of channels and second dimension is
    /// is the block size provided in the constructor.
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
        AudioSource(int blockSize, std::unique_ptr<std::istream> pIns)
                : mLastTick(),
                  mLoopOver(true),
                  mBlockSize(blockSize),
                  mSoundInfo() {

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
            // the samples read are one for each channel and then next frame (or sample for
            // all channels), and so on.
            std::vector<int> dims = {mSoundInfo.GetNumChannels(),
                                     elements_read / mSoundInfo.GetNumChannels()};
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
