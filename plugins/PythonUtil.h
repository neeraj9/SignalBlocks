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

#ifndef SIGBLOCKS_PLUGINS_PYTHONUTIL_H
#define SIGBLOCKS_PLUGINS_PYTHONUTIL_H

#include "PythonResult.h"

#include <memory>
#include <Python.h>

namespace signalblocks {
    class PythonUtil {
    public:
        static std::unique_ptr<PythonBaseResult> ExtractBasicType(PyObject* pObj);

        static std::unique_ptr<PythonBaseResult> ExtractSequenceType(PyObject* pObj);
    };
}

#endif // SIGBLOCKS_PLUGINS_PYTHONUTIL_H
