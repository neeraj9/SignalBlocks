// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_plugins_NumpyUtil_hh
#define iblocks_common_plugins_NumpyUtil_hh

#include "PythonResult.hh"

#include <memory>
#include <Python.h> // Needed for PyObject

namespace iblocks
{
  class NumpyUtil
  {
  public:
    static std::auto_ptr<PythonBaseResult> ExtractNumpyType(PyObject* pObj);
  };
}

#endif // iblocks_common_plugins_NumpyUtil_hh
