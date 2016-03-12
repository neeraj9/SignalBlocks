// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "NumpyUtil.h"

#include "PythonPluginCommon.h"
#include "PythonPluginExceptions.h"

#include <iostream>
#include <Python.h>

#ifdef PY_MAJOR_VERSION

#include <numpy/arrayobject.h>
#include <numpy/arrayscalars.h>

#define I_HAVE_NUMPY
#endif

#include <string>

using namespace sigblocks;

// see http://www.scipy.org/Cookbook/C_Extensions/NumPy_arrays
std::unique_ptr<PythonBaseResult>
NumpyUtil::ExtractNumpyType(PyObject* pObj) {
#ifdef I_HAVE_NUMPY
    if ((pObj == 0) || (pObj == Py_None)) {
        return std::unique_ptr<PythonBaseResult>(nullptr);
    }

    // have if numpy is installed in the build system
    //XXX Doest work
    //because numpy do not expose PyArray_Type to the outside world
    //in version-1.1.0
    //if (PyArray_Check(pObj))
    //Apply a great HACK for now and try newer python-numpy later.
    if (std::string(pObj->ob_type->tp_name) == "numpy.ndarray") {
        // Check if aligned.
        // Dont know what to do if unaligned?? XXX
        // usually it'll be aligned.
        if (!PyArray_ISALIGNED(pObj)) {
            throw PyPluginTypeException("PyArray is unaligned!");
            // return std::unique_ptr<PythonBaseResult>(0);
        }
        if (!PyArray_ISCARRAY(pObj)) {
            throw PyPluginTypeException("PyArray is not c-type aligned array!");
            // return std::unique_ptr<PythonBaseResult>(0);
        }
        // see http://docs.scipy.org/doc/numpy/reference/arrays.scalars.html
        // for hierarchy or array-scalars
        std::cout << "PyArray_ISBOOL: " << PyArray_ISBOOL(pObj) << std::endl;
        std::cout << "PyArray_ISINTEGER: " << PyArray_ISINTEGER(pObj) << std::endl;
        std::cout << "PyArray_ISFLOAT: " << PyArray_ISFLOAT(pObj) << std::endl;
        std::cout << "PyArray_ISSTRING: " << PyArray_ISSTRING(pObj) << std::endl;
        std::cout << "PyArray_ISCOMPLEX: " << PyArray_ISCOMPLEX(pObj) << std::endl;
        std::cout << "PyArray_ISPYTHON: " << PyArray_ISPYTHON(pObj) << std::endl;
        const int num_dimmensions = PyArray_NDIM(pObj);
        std::cout << "PyArray_NDIM(pObj) = " << PyArray_NDIM(pObj) << std::endl;
        npy_intp* dims = PyArray_DIMS(pObj); // XXX Change to get dims some other way.
        std::cout << "dims = " << dims << std::endl;
        // assert(dims != 0) for an array
        if (dims != 0) {
            for (int i = 0; i < num_dimmensions; ++i) {
                std::cout << "dims[" << i << "] = " << dims[i] << std::endl;
                // alternatively
                std::cout << "PyArray_DIM(pObj, " << i << ") = " << PyArray_DIM(pObj, i) << std::endl;

                // space between two elements at ((i+1) - i) dimmension
                // ex. for a MxN = 5x2 matrix (double=8bytes), i=0 stride[0] = 16 (N*8bytes),
                // i=1 stride[1] = 8 (1*8bytes).
                // so for a 2-d matrix the stride[1] is always equal to size of each element
                // of the matrix.
                std::cout << "PyArray_STRIDE(pObj, " << i << ") = " << PyArray_STRIDE(pObj, i) << std::endl;
                // Note: for a 2-d array you can access the element by
                // PyArray_GETPTR2(a, i, j) shortcut.
                // or do
                // (void *) a->data + i*a->strides[0] + j*a->strides[1],
                //    where a is of type (PyArrayObject*).
                // alternatively you could use
                // PyArray_BYTES(a) to get (char*)(a->data) and PyArray_STRIDE(a,i) for strides.
                // Note: PyArray_DATA(a) gives (void*)(a->data)
                // Note: I am not sure if aligned storage will be indicated else
                // we cannot simply do -> "double result = *(double*)PyArray_GETPTR2(a, i, j);"
                // we need to use memcopy to get the result, assuming the endianess/encoding is matched
                // with c-type code (which is usually the case but need to use macro's to get that info).
                //
                //
                // Note regarding contiguas matrix/vector
                // see http://web.archive.org/web/20021218064807/http://starship.python.net/crew/hinsen/NumPyExtensions.html
                // An important special case of a NumPy array is the contiguous array.
                // This is an array whose elements occupy a single contiguous block of
                // memory and have the same order as a standard C array. In a contiguous
                // array, the value of array->strides[i] is equal to the size of a single
                // array element times the product of array->dimensions[j] for j up to i-1.
                // Arrays that are created from scratch are always contiguous;
                // non-contiguous arrays are the result of indexing and other structural
                // array operations. The main advantage of contiguous arrays is easier handling
                // in C; the pointer array->data is cast to the required type and then used like
                // a C array, without any reference to the stride values. This is particularly
                // important when interfacing to existing libraries in C or Fortran, which typically
                // require this standard data layout. A function that requires input arrays to be
                // contiguous must call the conversion function PyArray_ContiguousFromObject().
                //
                // Note regarding some of the PyArray* calls:
                //   We know that the numpy shared objects are not linked with our code
                //   and also a lot of the numpy functions are static (numpy v1.1.0), so
                //   they are inaccessible. Hence use the PyObject_* variants.
                //{ will NOT work!
                //
                //  PyArrayObject *a; int new_dims[2];
                //  new_dims[0] = 10; new_dims[1] = 21;
                //  a = (PyArrayObject *) PyArray_FromDims(2, new_dims, PyArray_DOUBLE);
                //
                //}
                // Ex. To create a 2x2 numpy matrix do the following.
                //   PyObject *numpy = PyImport_ImportModule("numpy");
                //   PyObject *numpy_matrix = PyObject_GetAttrString(numpy, "matrix");
                //   PyObject *my_matrix = PyObject_CallFunction(numpy_matrix, "(s)", "0 0; 0 0");
                // see http://stackoverflow.com/questions/573487/any-way-to-create-a-numpy-matrix-with-c-api
                //
            }
        }
        // New reference needs to be protected.
        Object numpy(PyImport_ImportModule("numpy"));
        /* XXX
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
        */
    }
    else if (std::string(pObj->ob_type->tp_name) == "numpy.bool_") {
        // see /usr/include/python2.5/numpy/arrayscalars.h
        PyArray_TYPE(pObj);
        std::cout << NPY_BOOL << std::endl;
        std::cout << ((PyArrayObject*) pObj)->descr << std::endl;
        //std::cout << "ISBOOL = " << PyDataType_ISBOOL(pObj) << std::endl;
        std::cout << (((PyBoolScalarObject*) pObj)->obval == NPY_TRUE) << std::endl;
        std::cout << (PyArrayScalar_VAL(pObj, Bool) == NPY_TRUE) << std::endl;
        std::cout << "numpy.bool_" << std::endl;
    }
    // for sizes see http://docs.scipy.org/doc/numpy/reference/c-api.dtype.html#enumerated-types
    // /usr/include/python2.5/numpy/arrayscalars.h
    // numpy.int8
    // numpy.uint8
    // numpy.int16
    // numpy.uint16
    // numpy.int32 --> type casts to int but not on all systems
    // numpy.int_ --> type casts to int but not on all systems
    // numpy.uint32
    // numpy.int64
    // numpy.uint64
    // numpy.float_ --> on current system maps to float64 but not all sys
    // numpy.float32
    // numpy.float64 --> same as numpy.double but subclass of python double
    // numpy.complex --> same as python complex
    // numpy.complex_ --> same as numpy.complex128 but subclass of python float
    // numpy.complex64
    std::cout << "pObj->ob_type->tp_name = " << pObj->ob_type->tp_name << std::endl;
#endif
    // XXX do the other objects
    // see http://docs.python.org/release/2.5/api/concrete.html
    return std::unique_ptr<PythonBaseResult>(nullptr);
}

