// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGNALBLOCKS_SOUNDINFO_H
#define SIGNALBLOCKS_SOUNDINFO_H

#include <memory>
#include <sndfile.h>
#include <stdint.h>
#include <string>

namespace sigblocks {
    class SoundInfo {
    public:
        enum OpenMode {
            OPEN_READ_ONLY = 0,
            OPEN_WRITE_ONLY,
            OPEN_READ_AND_WRITE,

            OPEN_NONE
        };

        SoundInfo();

        virtual ~SoundInfo();

        /// Open an audio file with the given fullpath and mode.
        bool Open(const std::string& path, OpenMode mode);
        /// Open an audio stream in read-only mode.
        bool OpenStream(std::unique_ptr<std::istream> pIns);
        /// Close an audio source previously opened via Open() or OpenStream()
        /// interface.
        void Close();

        int GetNumChannels() const;
        int GetSampleRate() const;
        int64_t GetNumSamples() const;  // get total number of frames or samples
        int GetSeekable() const;
        int GetFormat() const;

        bool IsEof() const;

        /// The maxElements must be a multiple of total number of channels
        /// or value returned by GetNumChannels() error occurs and nothing
        /// is read.
        ///
        /// The samples read are one for each channel and then next frame (or sample for
        /// all channels), and so on.
        ///
        /// @return number of elements read.
        int64_t Read(char* pData, int maxElements);
        int64_t Read(unsigned char* pData, int maxElements);
        int64_t Read(short* pData, int maxElements);
        int64_t Read(int* pData, int maxElements);
        int64_t Read(long* pData, int maxElements);
        int64_t Read(float* pData, int maxElements);
        int64_t Read(double* pData, int maxElements);

        /// Reset back to the first data frame or sample.
        bool Rewind();

    private:
        int64_t ReadShort(short* pData, int maxElements);
        int64_t ReadInt(int* pData, int maxElements);
        int64_t ReadFloat(float* pData, int maxElements);
        int64_t ReadDouble(double* pData, int maxElements);

    private:
        OpenMode mMode;
        SF_INFO mSfInfo;
        SNDFILE* mpSndFile;  //< c-style file representation by libsndfile
        int mFd;  //< low level file descriptor
        std::unique_ptr<std::istream> mpIns;
    };

}

#endif //SIGNALBLOCKS_SOUNDINFO_H
