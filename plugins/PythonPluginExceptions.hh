// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_plugins_PythonPluginExceptions_hh
#define sigblocks_common_plugins_PythonPluginExceptions_hh

#include <exception>
#include <string>

namespace sigblocks
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

#endif // sigblocks_common_plugins_PythonPluginExceptions_hh
