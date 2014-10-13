// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_plugins_PythonUtil_hh
#define sigblocks_common_plugins_PythonUtil_hh

#include "PythonResult.hh"

#include <memory>
#include <Python.h>

namespace sigblocks
{
  class PythonUtil
  {
  public:
    static std::auto_ptr<PythonBaseResult> ExtractBasicType(PyObject* pObj);
    static std::auto_ptr<PythonBaseResult> ExtractSequenceType(PyObject* pObj);
  };
}

#endif // sigblocks_common_plugins_PythonUtil_hh
