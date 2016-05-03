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

#include "SafePythonObject.h"

using namespace sigblocks;

// Manage the reference counting automatically even when there
// are exceptions.
SafePythonObject::SafePythonObject(PyObject* ptr)
        : mpPtr(ptr) {
    // IMPORTANT:
    // We dont increment the ref because its a direct
    // pointer assignment and assumed to be auto ref inc
    // by the python interpreter.
}

SafePythonObject::~SafePythonObject() {
    if (mpPtr) {
        Py_DECREF(mpPtr);
    }
}

SafePythonObject::SafePythonObject(const SafePythonObject& rhs)
        : mpPtr(rhs.mpPtr) {
    if (mpPtr) {
        Py_INCREF(mpPtr);
    }
}

const SafePythonObject&
SafePythonObject::operator=(const SafePythonObject& rhs) {
    if ((this == &rhs) || (mpPtr == rhs.mpPtr)) {
        return *this;
    }
    // decrease self ref-count before owning the other
    if (mpPtr) {
        Py_DECREF(mpPtr);
    }
    // own a new PyObject and increment ref count
    mpPtr = rhs.mpPtr;
    if (mpPtr) {
        Py_INCREF(mpPtr);
    }
    return *this;
}

PyObject*
SafePythonObject::GetPyObject() {
    return mpPtr;
}
