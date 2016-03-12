// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_PLUGINS_NUMPYUTIL_H
#define SIGBLOCKS_PLUGINS_NUMPYUTIL_H

#include "PythonResult.h"

#include <memory>
#include <Python.h> // Needed for PyObject

namespace sigblocks {
    class NumpyUtil {
    public:
        static std::unique_ptr<PythonBaseResult> ExtractNumpyType(PyObject* pObj);
    };
}

#endif // SIGBLOCKS_PLUGINS_NUMPYUTIL_H
