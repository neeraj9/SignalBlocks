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

#ifndef SIGNALBLOCKS_PLUGINS_PYTHONPLUGINCOMMON_H
#define SIGNALBLOCKS_PLUGINS_PYTHONPLUGINCOMMON_H

#include "SafePythonObject.h"

namespace signalblocks {
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

#endif // SIGNALBLOCKS_PLUGINS_PYTHONPLUGINCOMMON_H
