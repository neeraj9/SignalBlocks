// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_PLUGINS_PYTHONPLUGINCOMMON_H
#define SIGBLOCKS_PLUGINS_PYTHONPLUGINCOMMON_H

#include "SafePythonObject.h"

namespace sigblocks {
    class Object
            : public SafePythonObject {
    public:
        Object(PyObject* ptr)
                : SafePythonObject(ptr) {
        }

        PyObject* Get() {
            return GetPyObject();
        }
    };
}

#endif // SIGBLOCKS_PLUGINS_PYTHONPLUGINCOMMON_H
