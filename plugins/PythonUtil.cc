// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "PythonUtil.hh"

#include "PythonPluginExceptions.hh"

#include <iostream>
#include <numpy/ndarrayobject.h> // required for Py_ssize_t
#include <string>

using namespace sigblocks;

std::unique_ptr<PythonBaseResult>
PythonUtil::ExtractBasicType(PyObject* pObj)
{
  // see http://docs.python.org/release/2.5.2/api/noneObject.html
  if ((pObj == 0) || (pObj == Py_None))
  {
    return std::unique_ptr<PythonBaseResult>(nullptr); // no result
  }
  // bool is a subclass of int so check it first.
  if (PyBool_Check(pObj))
  {
    long res = PyInt_AS_LONG(pObj);
    std::unique_ptr<PyBooleanResult> pres(new PyBooleanResult());
    if (res == 0)
    {
      pres->mValue = false;
    }
    else
    {
      pres->mValue = true;
    }
    return std::unique_ptr<PythonBaseResult>(pres.release());
  }
  else if (PyInt_Check(pObj))
  {
    long res = PyInt_AsLong(pObj);
    if (PyErr_Occurred())
    {
      PyErr_Print();
      PyErr_Clear();
      throw PyPluginTypeException("PyInt_AsLong(pObj) failed!");
      // return 0;
    }
    std::unique_ptr<PyLongResult> pres(new PyLongResult());
    pres->mValue = res;
    return std::unique_ptr<PythonBaseResult>(pres.release());
  }
  else if (PyLong_Check(pObj))
  {
    long res = PyLong_AsLong(pObj);
    if (PyErr_Occurred())
    {
      PyErr_Clear(); // XXX is this needed?
      long long res = PyLong_AsLongLong(pObj);
      if (PyErr_Occurred())
      {
        PyErr_Print();
        PyErr_Clear();
        throw PyPluginTypeException("PyLong_AsLongLong(pObj) failed!");
        // return 0;
      }
      std::unique_ptr<PyLongLongResult> pres(new PyLongLongResult());
      pres->mValue = res;
      return std::unique_ptr<PythonBaseResult>(pres.release());
    }
    // else
    std::unique_ptr<PyLongResult> pres(new PyLongResult());
    pres->mValue = res;
    return std::unique_ptr<PythonBaseResult>(pres.release());
  }
  else if (PyFloat_Check(pObj))
  {
    double res = PyFloat_AsDouble(pObj);
    if (PyErr_Occurred())
    {
      PyErr_Print();
      PyErr_Clear();
      throw PyPluginTypeException("PyFloat_AsDouble(pObj) failed!");
      // return 0;
    }
    std::unique_ptr<PyDoubleResult> pres(new PyDoubleResult());
    pres->mValue = res;
    return std::unique_ptr<PythonBaseResult>(pres.release());
  }
  else if (PyComplex_Check(pObj))
  {
    Py_complex res = PyComplex_AsCComplex(pObj);
    if (PyErr_Occurred())
    {
      PyErr_Print();
      PyErr_Clear();
      throw PyPluginTypeException("PyComplex_AsCComplex(pObj) failed!");
      // return 0;
    }
    std::unique_ptr<PyComplexResult> pres(new PyComplexResult());
    pres->mValue.mRealValue = res.real;
    pres->mValue.mImagValue = res.imag;
    return std::unique_ptr<PythonBaseResult>(pres.release());
  }
  else if (PyString_Check(pObj))
  {
    Py_ssize_t len = PyString_GET_SIZE(pObj);
    const char* str = PyString_AS_STRING(pObj);
    // copy the string and dont modify because its an internal
    // buffer of the pObj
    std::unique_ptr<PyStringResult> pres(new PyStringResult());
    pres->mValue = std::string(str, len);
    return std::unique_ptr<PythonBaseResult>(pres.release());
  }
  // XXX else if (PyUnicodeObject(pObj))
  throw PyPluginTypeException("error: no basic type matched");
  // return 0;
}

std::unique_ptr<PythonBaseResult>
PythonUtil::ExtractSequenceType(PyObject* pObj)
{
  // XXXX if (PyBuffer_Check(pObj))
  if (PyTuple_Check(pObj))
  {
    int len = PyTuple_GET_SIZE(pObj);
    std::unique_ptr<PyTupleResult> pres(new PyTupleResult());
    for (int i = 0; i < len; ++i)
    {
      // PyTuple_GetItem() was aborrowed reference so no need to
      // worry about the reference counting.
      PyObject* pobj = PyTuple_GET_ITEM(pObj, i);
      // XXX find the type and populate a list of that type.
      std::unique_ptr<PythonBaseResult> new_element(ExtractBasicType(pobj));
      if (new_element.get() != 0)
      {
        pres->mValue.push_back(new_element.release());
      }
    }
    return std::unique_ptr<PythonBaseResult>(pres.release());
  }
  else if (PyList_Check(pObj))
  {
    Py_ssize_t len = PyList_GET_SIZE(pObj);
    std::unique_ptr<PyListResult> pres(new PyListResult());
    for (int i = 0; i < len; ++i)
    {
      PyObject* pobj = PyList_GET_ITEM(pObj, i);
      std::unique_ptr<PythonBaseResult> new_element(ExtractBasicType(pobj));
      if (new_element.get() != 0)
      {
        pres->mValue.push_back(new_element.release());
      }
    }
    return std::unique_ptr<PythonBaseResult>(pres.release());
  }
  else if (PyDict_Check(pObj))
  {
    PyObject* key;
    PyObject* value;
    Py_ssize_t pos = 0;
    std::unique_ptr<PyDictResult> pres(new PyDictResult());
    while (PyDict_Next(pObj, &pos, &key, &value))
    {
      if (key !=0 && value != 0)
      {
        std::unique_ptr<PythonBaseResult> pkey(ExtractBasicType(key));
        std::unique_ptr<PythonBaseResult> pvalue(ExtractBasicType(value));
        if (pkey.get() != 0 && pvalue.get() != 0)
        {
          pres->mValue.insert(std::make_pair(pkey.release(), pvalue.release()));
        }
      }
    }
    return std::unique_ptr<PythonBaseResult>(pres.release());
  }
  // XXX do the other objects
  // see http://docs.python.org/release/2.5/api/concrete.html
  return std::unique_ptr<PythonBaseResult>(nullptr);
}

