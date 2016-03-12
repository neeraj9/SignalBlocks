// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_PLUGINS_PYTHONUTIL_H
#define SIGBLOCKS_PLUGINS_PYTHONUTIL_H

#include "PythonResult.h"

#include <memory>
#include <Python.h>

namespace sigblocks {
    class PythonUtil {
    public:
        static std::unique_ptr<PythonBaseResult> ExtractBasicType(PyObject* pObj);

        static std::unique_ptr<PythonBaseResult> ExtractSequenceType(PyObject* pObj);
    };
}

#endif // SIGBLOCKS_PLUGINS_PYTHONUTIL_H
