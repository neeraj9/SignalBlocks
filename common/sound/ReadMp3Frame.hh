// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_sound_ReadMp3Frame_hh
#define iblocks_common_sound_ReadMp3Frame_hh

#include <string>

namespace iblocks
{
  /**
   * ReadMp3Frame extracts mp3 frame from a mp3 file.
   * It hides the detail of reading the mp3 frame structure from the clients.
   */
  class ReadMp3Frame
  {
  public:
    /// filename name of the file to read into
    ReadMp3Frame(const std::string& filename);
    ~ReadMp3Frame();

    std::string GetFilename() const;

  private:
    const std::string& mFilename;
  };
}

#endif // iblocks_common_sound_ReadMp3Frame_hh
