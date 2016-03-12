# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(PLUGINS_SOURCES
        NumpyUtil.h
        NumpyUtil.cpp
        PythonPluginCommon.h
        PythonResult.h
        PythonPlugin.h
        PythonPlugin.cpp
        PythonPluginExceptions.h
        PythonPluginExceptions.cpp
        PythonUtil.cpp
        PythonUtil.h
        SafePythonObject.cpp
        SafePythonObject.h)

add_library(plugins STATIC ${PLUGINS_SOURCES})

target_include_directories(plugins PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})