/// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
/// All rights reserved.

#ifndef SIGBLOCKS_LOGGING_H
#define SIGBLOCKS_LOGGING_H

#include <cstdio>

namespace sigblocks {

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
