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

#ifndef SIGNALBLOCKS_PLUGINS_PYTHONPLUGIN_H
#define SIGNALBLOCKS_PLUGINS_PYTHONPLUGIN_H

#include "PythonPluginExceptions.h"
#include "PythonResult.h"
#include "SafePythonObject.h"

#include <mutex>
#include <memory>
#include <string>

namespace signalblocks {
    typedef SafePythonObject PythonRunnableCode;

    //
    // For regular users have a single instance of this class
    // and share it across multiple threads within the same
    // process if you need it. At best do not create multiple
    // instances of the class to exist at the same time.
    //
    // Also note that the python interpreter is initialized
    // upon the first instance of this class and destroyed
    // only at the exit of the process.
    //
    // Details:
    //
    // Note that python interpreter is not completely
    // thread safe (or at least was not at the time of
    // writing this module and I did not test again).
    // That is why a mutex is added to this class in case
    // a single instance of this class used across multiple
    // threads. This do not restrict the case where many
    // instances of such class could exist, but then it
    // is left to the anyone using this plugin worry about it.
    //
    // Although a single process may have multiple copies of
    // python plugin, but ultimately the initialization and
    // finalization step for the python interpreter only
    // happens once. This class ensures that a handler
    // is setup via a call to atexit() cstdlib call
    // so that python finalization happens at the
    // exit of the process.
    //
    class PythonPlugin {
    public:
        PythonPlugin();

        // This is better than passing modulePath all the time
        // in RunPythonCode.
        // So instead use this ctor and pass empty string as
        // modulePath in RunPythonCode wherever possible.
        // Also, this will be useful for any custom path of
        // user root module.
        // Note: You can specify multiple paths separated by
        // comma.
        PythonPlugin(const std::string& includePath);

        ~PythonPlugin();

        // return new reference
        std::unique_ptr<PythonBaseResult> RunPythonCode(
                const std::string& modulePath,
                const std::string& pyModuleName,
                const std::string& pyFuncName) throw(PyPluginTypeException);

        // return new reference
        std::unique_ptr<PythonRunnableCode> ParsePythonSource(
                const std::string& source) throw();

        // return new reference
        std::unique_ptr<PythonBaseResult> RunPythonRunnableCode(
                PythonRunnableCode* pCode) throw(PyPluginTypeException);

    private: // no copy allowed
        PythonPlugin(const PythonPlugin&) = delete;

        PythonPlugin& operator=(const PythonPlugin&) = delete;

    private:
        std::mutex mMutex;
    };
}

#endif // SIGNALBLOCKS_PLUGINS_PYTHONPLUGIN_H
