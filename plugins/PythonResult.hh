// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_plugins_PythonResult_hh
#define sigblocks_common_plugins_PythonResult_hh

#include <list>
#include <map>
#include <ostream>
#include <string>
#include <sstream>

namespace sigblocks
{
  class PythonBaseResult
  {
  public:
    enum PythonResultType
    {
      BOOL_TYPE = 0,
      LONG_TYPE,
      LONG_LONG_TYPE,
      DOUBLE_TYPE,
      COMPLEX_TYPE,
      STRING_TYPE,
      TUPLE_TYPE,
      LIST_TYPE,
      DICT_TYPE,
    };
    virtual ~PythonBaseResult() {}

    virtual PythonResultType GetType() const = 0;

    virtual std::string ToString() = 0;
  };

  class PyBooleanResult : public PythonBaseResult
  {
  public:
    virtual PythonResultType GetType() const { return BOOL_TYPE; }
    virtual std::string ToString()
    {
      std::stringstream out;
      out << "(Type,Value) = " << GetType() << ", " << mValue;
      return out.str();
    }
    bool mValue;
  };

  class PyLongResult : public PythonBaseResult
  {
  public:
    virtual PythonResultType GetType() const { return LONG_TYPE; }
    virtual std::string ToString()
    {
      std::stringstream out;
      out << "(Type,Value) = " << GetType() << ", " << mValue;
      return out.str();
    }
    long mValue;
  };

  class PyLongLongResult : public PythonBaseResult
  {
  public:
    virtual PythonResultType GetType() const { return LONG_LONG_TYPE; }
    virtual std::string ToString()
    {
      std::stringstream out;
      out << "(Type,Value) = " << GetType() << ", " << mValue;
      return out.str();
    }
    long long mValue;
  };

  class PyDoubleResult : public PythonBaseResult
  {
  public:
    virtual PythonResultType GetType() const { return DOUBLE_TYPE; }
    virtual std::string ToString()
    {
      std::stringstream out;
      out << "(Type,Value) = " << GetType() << ", " << mValue;
      return out.str();
    }
    double mValue;
  };

  class PyComplexResult : public PythonBaseResult
  {
  public:
    virtual PythonResultType GetType() const { return COMPLEX_TYPE; }
    virtual std::string ToString()
    {
      std::stringstream out;
      out << "(Type,Value) = " << GetType() << ", " << mValue.ToString();
      return out.str();
    }

    struct DComplex
    {
      double mRealValue;
      double mImagValue;
      std::string ToString()
      {
        std::stringstream outStream;
        outStream << mRealValue << "+ j" << mImagValue;
        return outStream.str();
      }
    } mValue;
  };

  class PyStringResult : public PythonBaseResult
  {
  public:
    virtual PythonResultType GetType() const { return STRING_TYPE; }
    virtual std::string ToString()
    {
      std::stringstream out;
      out << "(Type,Value) = " << GetType() << ", " << mValue;
      return out.str();
    }
    std::string mValue;
  };

  class PyTupleResult : public PythonBaseResult
  {
  public:
    virtual ~PyTupleResult()
    {
      for (std::list<PythonBaseResult*>::iterator iter = mValue.begin();
           iter != mValue.end();
           ++iter)
      {
        delete *iter;
      }
    }
    virtual PythonResultType GetType() const { return TUPLE_TYPE; }
    virtual std::string ToString()
    {
      std::stringstream out;
      out << "(Type,TupleSize) = " << GetType() << ", " << mValue.size();
      return out.str();
    }

    std::list<PythonBaseResult*> mValue;
  };

  class PyListResult : public PythonBaseResult
  {
  public:
    virtual ~PyListResult()
    {
      for (std::list<PythonBaseResult*>::iterator iter = mValue.begin();
           iter != mValue.end();
           ++iter)
      {
        delete *iter;
      }
    }
    virtual PythonResultType GetType() const { return LIST_TYPE; }
    virtual std::string ToString()
    {
      std::stringstream out;
      out << "(Type,ListSize) = " << GetType() << ", " << mValue.size();
      return out.str();
    }

    std::list<PythonBaseResult*> mValue;
  };

  class PyDictResult : public PythonBaseResult
  {
  public:
    virtual ~PyDictResult()
    {
      for (std::map<PythonBaseResult*, PythonBaseResult*>::iterator
             iter = mValue.begin();
           iter != mValue.end();
           ++iter)
      {
        delete iter->second;
        delete iter->first;
      }
    }
    virtual PythonResultType GetType() const { return DICT_TYPE; }
    virtual std::string ToString()
    {
      std::stringstream out;
      out << "(Type,DictSize) = " << GetType() << ", " << mValue.size();
      return out.str();
    }

    std::map<PythonBaseResult*, PythonBaseResult*> mValue;
  };
}

#endif // sigblocks_common_plugins_PythonResult_hh
