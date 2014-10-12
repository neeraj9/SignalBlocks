// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_plugins_PythonUtil_hh
#define iblocks_common_plugins_PythonUtil_hh

#include "PythonResult.hh"

#include <memory>
#include <Python.h>

namespace iblocks
{
  class PythonUtil
  {
  public:
    static std::auto_ptr<PythonBaseResult> ExtractBasicType(PyObject* pObj);
    static std::auto_ptr<PythonBaseResult> ExtractSequenceType(PyObject* pObj);
  };
}

#endif // iblocks_common_plugins_PythonUtil_hh
