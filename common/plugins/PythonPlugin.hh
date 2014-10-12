// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_plugins_PythonPlugin_hh
#define iblocks_common_plugins_PythonPlugin_hh

#include "PythonPluginExceptions.hh"
#include "PythonResult.hh"
#include "SafePythonObject.hh"

#include "../Mutex.hxx"

#include <memory>
#include <string>

namespace iblocks
{
  typedef SafePythonObject PythonRunnableCode;

  // I dont want to have singleton pattern because of multi-threaded env
  // so lets be content with no-copy and let application have as many
  // plugin's required. But have only one.
  class PythonPlugin
  {
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
    std::auto_ptr<PythonBaseResult> RunPythonCode(
      const std::string& modulePath,
      const std::string& pyModuleName,
      const std::string& pyFuncName) throw(PyPluginTypeException);

    // return new reference
    std::auto_ptr<PythonRunnableCode> ParsePythonSource(
      const std::string& source) throw();
    // return new reference
    std::auto_ptr<PythonBaseResult> RunPythonRunnableCode(
      PythonRunnableCode* pCode) throw(PyPluginTypeException);

  private: // no copy allowed
    PythonPlugin(const PythonPlugin&);
    PythonPlugin& operator=(const PythonPlugin&);

  private:
    mutable BoostPort::Mutex mMutex;
  };
}

#endif // iblocks_common_plugins_PythonPlugin_hh
