// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "PythonPluginExceptions.hh"

using namespace sigblocks;

PyPluginTypeException::PyPluginTypeException(const std::string& error) throw()
        : mError(error) {
}

PyPluginTypeException::~PyPluginTypeException() throw() {
}

const char*
PyPluginTypeException::what() const throw() {
    return mError.c_str();
}
