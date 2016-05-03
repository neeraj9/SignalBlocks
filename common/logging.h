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

#ifndef SIGBLOCKS_LOGGING_H
#define SIGBLOCKS_LOGGING_H

#include <cstdio>

namespace signalblocks {

#define VERBOSE

#define DISPLAY(...) printf("%s:%d  %s() - ", __FILE__, __LINE__, __func__); printf(__VA_ARGS__)

#ifdef VERBOSE
#define LOG_ERROR(...) DISPLAY("[ERROR] " __VA_ARGS__)
#define LOG_DEBUG(...) DISPLAY("[DEBUG] " __VA_ARGS__)
#define LOG_INFO(...) DISPLAY("[INFO] " __VA_ARGS__)
#define LOG_WARN(...) DISPLAY("[WARN] " __VA_ARGS__)
#else
#define LOG_ERROR(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#endif

}


#endif //SIGBLOCKS_LOGGING_H
