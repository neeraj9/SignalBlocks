# (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
# see LICENSE for license
#
LDADD = $(LOCAL_LIBS)
LOCAL_LIBS = \
 libplugincommon.la \
 ../libcommon.la

noinst_LTLIBRARIES = libplugincommon.la

if OS_DARWIN

libplugincommon_la_CPPFLAGS = -I/opt/local/Library//Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages/numpy/core/include
libplugincommon_la_LDFLAGS = -L/opt/local/Library//Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages/numpy/core/lib

endif

libplugincommon_la_SOURCES = \
 NumpyUtil.cc \
 PythonPlugin.cc \
 PythonPluginExceptions.cc \
 PythonUtil.cc \
 SafePythonObject.cc

TEST_EXE = \
 PythonPluginTest

check_PROGRAMS = $(TEST_EXE)

TESTS = $(TEST_EXE)

# FIXME
valgrind:
	$(MAKE) CHECKER='valgrind --tool=memcheck --suppressions=/usr/lib/valgrind/python.supp --quiet --leak-check=full --trace-children=yes' $(TESTS)

PythonPluginTest_SOURCES = PythonPluginTest.cc
