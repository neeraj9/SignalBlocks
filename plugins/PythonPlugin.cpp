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

#include "PythonPlugin.h"
#include "PythonPluginCommon.h"

#include "NumpyUtil.h"
#include "PythonUtil.h"
#include "../common/logging.h"

#include <atomic>
#include <cstdlib>
#include <mutex>
#include <memory>
#include <Python.h>
#include <iostream>

using namespace signalblocks;

namespace {

    static std::atomic_flag py_initialized = ATOMIC_FLAG_INIT;

    // Initialize and Deinitialize the python interpreter only
    // once for the whole process. This is required because modules
    // like numpy do not play nice with repeated Py_Initialize() and
    // Py_Finalize() calls.
    // see https://mail.scipy.org/pipermail/numpy-discussion/2009-March/040849.html

    static void PythonDeinitOnlyOnce() {
        // Finish the Python Interpreter
        LOG_DEBUG("Finish the python interpreter\n");
        Py_Finalize();
    }

    static void PythonInitOnlyOnce() {
        bool was_init = py_initialized.test_and_set(std::memory_order_seq_cst);
        if (! was_init) {
            Py_Initialize();
            // Dont do it all the time because its expensive!
            // So have only one instance of the PythonPlugin.
            PyRun_SimpleString("import sys, os, re, struct");
            PyRun_SimpleString("import numpy");

            assert(atexit(&PythonDeinitOnlyOnce) == 0);
            LOG_DEBUG("Initialized the python interpreter.\n");
        }
    }
}

PythonPlugin::PythonPlugin() {
    PythonInitOnlyOnce();
}

PythonPlugin::PythonPlugin(const std::string& includePath) {
    PythonInitOnlyOnce();

    // This is better than passing modulePath all the time.
    // So instead use this ctor and pass empty string as
    // modulePath in RunPythonCode wherever possible.
    if (includePath.size() > 0) {
        // PyRun_SimpleString("import sys"); done already
        std::string sys_path_append_cmd =
                "sys.path.append(\"" + includePath + "\")";
        PyRun_SimpleString(sys_path_append_cmd.c_str());
    }
}

std::unique_ptr<PythonBaseResult>
PythonPlugin::RunPythonCode(
        const std::string& modulePath,
        const std::string& pyModuleName,
        const std::string& pyFuncName) throw(PyPluginTypeException) {
    // Python interpreter is not fully thread safe. So for multi-threaded
    // programs we need to either use the Python GIL (global interpreter lock)
    // or have our own because its simple and independent of python version.
    std::lock_guard<std::mutex> guard(mMutex);

    // XXX There is a catch: Locking at the top will lock out other threads
    // trying to use the interpreter till the execution of one is going on.
    // This is an issue for big python code and giving a simple warning is
    // not sufficient. Looks like there is no way out there, so lets just live
    // with it but remember to warn users to write smaller functions which
    // need to be executed.

    // Fix an error as suggested by
    // site: http://old.nabble.com/failed-in-PyImport_Import()-td13401283.html
    if (modulePath.size() > 0) {
        PyRun_SimpleString("import sys");
        // PyRun_SimpleString("print sys.path");
        std::string sys_path_append_cmd = "sys.path.append(\"" + modulePath + "\")";
        PyRun_SimpleString(sys_path_append_cmd.c_str());
    }

#if PY_MAJOR_VERSION == 2 // long way of doing short things
    // Build the name object
    Object name(PyString_FromString(pyModuleName.c_str()));
    if (name.Get() == NULL) {
        PyErr_Print();
        PyErr_Clear();
        throw PyPluginTypeException(
                pyModuleName + std::string(" module not found."));
        // return std::unique_ptr<PythonBaseResult>(0);
    }
    // Load the module object
    // module is a new reference
    Object module(PyImport_Import(name.Get()));
#elif PY_MAJOR_VERSION >= 3
    // This gives error with stricter compiler in CentOS 5.5
    // module is a new reference for Python ver < 3, but
    // for Python 3+ this is not an issue with newer compiler.
    Object module(PyImport_ImportModule(pyModuleName.c_str()));
#endif
    if (module.Get() == 0) {
        PyErr_Print();
        PyErr_Clear();
        throw PyPluginTypeException(
                pyModuleName + std::string(" module failed to load."));
        // return std::unique_ptr<PythonBaseResult>(0);
    }
    // pDict is a borrowed reference so dont do ref-counting
    PyObject * dict = PyModule_GetDict(module.Get());
    if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
        throw PyPluginTypeException("Failed to get Dict");
    }
    // pFunc is also a borrowed reference so dont do ref-counting
    PyObject * func = PyDict_GetItemString(dict, pyFuncName.c_str());
    if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
        throw PyPluginTypeException(
                pyFuncName + std::string(" function failed to load."));
        // return std::unique_ptr<PythonBaseResult>(0);
    }
    if (PyCallable_Check(func) <= 0) {
        PyErr_Print();
        PyErr_Clear();
        throw PyPluginTypeException(
                pyFuncName + std::string(" is not callable."));
        // return std::unique_ptr<PythonBaseResult>(0);
    }
    // else success
    Object func_result(PyObject_CallObject(func, NULL));
    if (func_result.Get() != 0) {
        if (func_result.Get() == Py_None) {
            return std::unique_ptr<PythonBaseResult>(nullptr);
        }
        std::unique_ptr<PythonBaseResult> res;

        try {
            res = PythonUtil::ExtractBasicType(func_result.Get());
        }
        catch (PyPluginTypeException& e) {
            // we'll catch here to try other types.
        }
        if (res.get() == 0) {
            res = PythonUtil::ExtractSequenceType(func_result.Get());
        }
        if (res.get() == 0) {
            res = NumpyUtil::ExtractNumpyType(func_result.Get());
        }
        return res;
    }
    // else function call failed
    PyErr_Print();
    PyErr_Clear();
    throw PyPluginTypeException(
            pyFuncName + std::string(" call returned failure."));
    // return std::unique_ptr<PythonBaseResult>(0);
}

// Avoid object slicing
std::unique_ptr<PythonRunnableCode>
PythonPlugin::ParsePythonSource(const std::string& source) throw() {
    // Python interpreter is not fully thread safe. So for multi-threaded
    // programs we need to either use the Python GIL (global interpreter lock)
    // or have our own because its simple and independent of python version.
    std::lock_guard<std::mutex> guard(mMutex);

    if (source.size() == 0) {
        std::cerr << "empty source supplied to ParsePythonSource" << std::endl;
        return std::unique_ptr<PythonRunnableCode>(nullptr);
    }
    // Py_file_input:
    //   This is the symbol to use when compiling arbitrarily long Python source code.
    std::unique_ptr<PythonRunnableCode> ret(
            new Object(Py_CompileString(source.c_str(), "<inline-source>", Py_file_input)));
    if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
        return std::unique_ptr<PythonRunnableCode>(nullptr);
    }

    return ret;
}

// ref: http://www.python.org/doc/faq/extending/
std::unique_ptr<PythonBaseResult>
PythonPlugin::RunPythonRunnableCode(
        PythonRunnableCode* pCode) throw(PyPluginTypeException) {
    // Python interpreter is not fully thread safe. So for multi-threaded
    // programs we need to either use the Python GIL (global interpreter lock)
    // or have our own because its simple and independent of python version.
    std::lock_guard<std::mutex> guard(mMutex);

    Object loc(PyDict_New());
    Object glb(PyDict_New());
    PyDict_SetItemString(glb.Get(), "__builtins__", PyEval_GetBuiltins());

    // XXX the dynamic_cast is bound to succeed.
#if PY_MAJOR_VERSION == 2
    PyObject * pyobject = dynamic_cast<Object*>(pCode)->Get();
    Object result(PyEval_EvalCode((PyCodeObject*) pyobject, glb.Get(), loc.Get()));
#elif PY_MAJOR_VERSION >= 3
    // PythonRunnableCode may not be required for python 3+, but lets
    // keep it simple and retain that logic for supporting older python
    // interpreter as well.
    PyObject * pyobject = dynamic_cast<Object*>(pCode)->Get();
    Object result(PyEval_EvalCode(pyobject, glb.Get(), loc.Get()));
#endif
    // XXX look at /usr/include/python2.5/eval.h
    // and alternatively use PyEval_EvalCodeEx
    if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
        return std::unique_ptr<PythonBaseResult>(nullptr);
    }
    if (result.Get() != 0) {
        std::unique_ptr<PythonBaseResult> res;
        try {
            res = PythonUtil::ExtractBasicType(result.Get());
        }
        catch (PyPluginTypeException& e) {
            // we'll catch here to try other types.
        }
        if (res.get() == 0) {
            res = PythonUtil::ExtractSequenceType(result.Get());
        }
        if (res.get() == 0) {
            res = NumpyUtil::ExtractNumpyType(result.Get());
        }
        return res;
    }
    // else no result returned
    // Note: Errors are reported by exceptions.
    return std::unique_ptr<PythonBaseResult>(nullptr);
}

PythonPlugin::~PythonPlugin() {
    // do not finalize the python interpreter
    // here because a new instance of python plugin
    // will cause an issue with loading of numpy module
    // see PythonDeinitOnlyOnce() for details.
}
