// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_plugins_PythonPluginExceptions_hh
#define iblocks_common_plugins_PythonPluginExceptions_hh

#include <exception>
#include <string>

namespace iblocks
{
  class PyPluginTypeException
    : public std::exception
  {
  public:
    PyPluginTypeException(const std::string& error) throw();
    virtual ~PyPluginTypeException() throw();

    virtual const char* what() const throw();

  private:
    const std::string mError;
  };
}

#endif // iblocks_common_plugins_PythonPluginExceptions_hh
