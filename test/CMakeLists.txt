# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

# ##################################################################################
# Common Source Files
# ##################################################################################

set(SOURCE_FILES
        catch.hpp test_blocks_math_gain.cpp)

# ##################################################################################
# main sample application
# ##################################################################################

add_executable(testsuite testsuite.cpp ${SOURCE_FILES})


target_link_libraries(testsuite blocks)
target_link_libraries(testsuite plugins)
target_link_libraries(testsuite socket)
target_link_libraries(testsuite codec)
target_link_libraries(testsuite common)

target_link_libraries(testsuite ${PYTHON_LIBRARIES} ${GSL_LIBRARIES} ${SCTP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# unit test via CTest
enable_testing()
add_test(NAME RunTests COMMAND testsuite)
