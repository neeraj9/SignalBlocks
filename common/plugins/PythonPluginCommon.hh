// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_plugins_PythonPluginCommon_hh
#define iblocks_common_plugins_PythonPluginCommon_hh

#include "SafePythonObject.hh"

namespace iblocks
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

#endif // iblocks_common_plugins_PythonPluginCommon_hh
