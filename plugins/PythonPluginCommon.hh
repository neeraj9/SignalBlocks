// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_plugins_PythonPluginCommon_hh
#define sigblocks_common_plugins_PythonPluginCommon_hh

#include "SafePythonObject.hh"

namespace sigblocks
{
  class Object
    : public SafePythonObject
  {
  public:
    Object(PyObject* ptr)
      : SafePythonObject(ptr)
    {
    }

    PyObject* Get()
    {
      return GetPyObject();
    }
  };
}

#endif // sigblocks_common_plugins_PythonPluginCommon_hh
