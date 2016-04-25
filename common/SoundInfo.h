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

        SoundInfo(bool enableNormalisation = true);

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
        /// @note Whenever integer data is moved from one sized container to another
        /// sized container, the most significant bit in the source container
        /// will become the most significant bit in the destination container.
        ///
        /// @note When converting between integer data and floating point data,
        /// different rules apply. The default behaviour when reading floating
        /// point data (sf_read_float() or sf_read_double ()) from a file with
        /// integer data is normalisation. Regardless of whether data in the
        /// file is 8, 16, 24 or 32 bit wide, the data will be read as floating
        /// point data in the range [-1.0, 1.0]. Similarly, data in the range
        /// [-1.0, 1.0] will be written to an integer PCM file so that a data
        /// value of 1.0 will be the largest allowable integer for the given
        /// bit width. This normalisation can be turned on or off using the
        /// sf_command interface.
        ///
        /// @note Reading a file containing floating point data (allowable
        /// with WAV, AIFF, AU and other file formats) using integer read
        /// methods can produce unexpected results. There is a work around
        /// this issue but it is currently not implemented to force
        /// the user to use correct data type while reading from such audio
        /// files.
        ///
        /// @return number of elements read.
        ///
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
        bool mEnableNormalisation;
        OpenMode mMode;
        SF_INFO mSfInfo;
        SNDFILE* mpSndFile;  //< c-style file representation by libsndfile
        int mFd;  //< low level file descriptor
        std::unique_ptr<std::istream> mpIns;
    };

}

#endif //SIGNALBLOCKS_SOUNDINFO_H
