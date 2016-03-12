// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_PLUGINS_SAFEPYTHONOBJECT_H
#define SIGBLOCKS_PLUGINS_SAFEPYTHONOBJECT_H

#include <Python.h> // Its present here only because of PyObject
// because its forward declaration is not possible.

namespace sigblocks {
    class SafePythonObject {
    protected:
        // Dont pass pointer by using Get() of some other
        // SafePythonObject because that will not take care
        // of the reference counting and lead to issues later.
        // Ths constructor is for the first ownership only.
        // Use CopyCtor or Assignment operator instead if a copy
        // is required.
        explicit SafePythonObject(PyObject* ptr);

    public:
        virtual ~SafePythonObject();

        // Smart copy is required.
        SafePythonObject(const SafePythonObject& rhs);

        const SafePythonObject& operator=(const SafePythonObject& rhs);

    protected:
        PyObject* GetPyObject();

    private:
        PyObject* mpPtr;
    };
}

#endif // SIGBLOCKS_PLUGINS_SAFEPYTHONOBJECT_H
