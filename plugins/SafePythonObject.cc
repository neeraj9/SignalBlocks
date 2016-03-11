// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "SafePythonObject.hh"

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
