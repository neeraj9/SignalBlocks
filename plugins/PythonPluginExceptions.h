// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_PLUGINS_PYTHONPLUGINEXCEPTIONS_H
#define SIGBLOCKS_PLUGINS_PYTHONPLUGINEXCEPTIONS_H

#include <exception>
#include <string>

namespace sigblocks {
    class PyPluginTypeException
            : public std::exception {
    public:
        PyPluginTypeException(const std::string& error) throw();

        virtual ~PyPluginTypeException() throw();

        virtual const char* what() const throw();

    private:
        const std::string mError;
    };
}

#endif // SIGBLOCKS_PLUGINS_PYTHONPLUGINEXCEPTIONS_H
