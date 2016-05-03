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

#ifndef SIGBLOCKS_PLUGINS_SAFEPYTHONOBJECT_H
#define SIGBLOCKS_PLUGINS_SAFEPYTHONOBJECT_H

#include <Python.h> // Its present here only because of PyObject
// because its forward declaration is not possible.

namespace signalblocks {
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
