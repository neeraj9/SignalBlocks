// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_plugins_NumpyUtil_hh
#define sigblocks_common_plugins_NumpyUtil_hh

#include "PythonResult.hh"

#include <memory>
#include <Python.h> // Needed for PyObject

namespace sigblocks
{
  class NumpyUtil
  {
  public:
    static std::auto_ptr<PythonBaseResult> ExtractNumpyType(PyObject* pObj);
  };
}

#endif // sigblocks_common_plugins_NumpyUtil_hh
