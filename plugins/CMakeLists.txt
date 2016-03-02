# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(PLUGINS_SOURCES
        NumpyUtil.hh
        NumpyUtil.cc
        PythonPluginCommon.hh
        PythonResult.hh
        PythonPlugin.hh
        PythonPlugin.cc
        PythonPluginExceptions.hh
        PythonPluginExceptions.cc
        PythonUtil.cc
        PythonUtil.hh
        SafePythonObject.cc
        SafePythonObject.hh)

add_library(plugins STATIC ${PLUGINS_SOURCES})

target_include_directories(plugins PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})