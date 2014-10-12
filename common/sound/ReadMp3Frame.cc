// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "ReadMp3Frame.hh"

using namespace iblocks;
using namespace std;

ReadMp3Frame::ReadMp3Frame(const string& filename)
  : mFilename(filename)
{
}

string
ReadMp3Frame::GetFilename() const
{
  return mFilename;
}
