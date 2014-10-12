# copyright (c) 2010-12 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
# see LICENSE.txt for license information

# http://stackoverflow.com/questions/11438613/conditional-subdir-objects
AUTOMAKE_OPTIONS = subdir-objects

#SUBDIRS =

if OS_DARWIN
  # http://dennycd.me/google-test-xcode-mac-osx/
  # http://www.gnu.org/software/automake/manual/html_node/Program-Variables.html
  AM_CPPFLAGS=-DGTEST_USE_OWN_TR1_TUPLE=1
endif

libgmock_la_CXXFLAGS=-Igmock -Igmock/gtest $(CXXFLAGS)
LDADD = $(LOCAL_LIBS)
LOCAL_LIBS = \
 libgmock.la

noinst_LTLIBRARIES = libgmock.la

libgmock_la_SOURCES = \
 gmock/gtest/src/gtest-all.cc \
 gmock/src/gmock-all.cc

bin_PROGRAMS = gmock_test

gmock_test_SOURCES = gmock/test/gmock_test.cc \
										 gmock/src/gmock_main.cc
