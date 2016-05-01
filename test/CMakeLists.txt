# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

# ##################################################################################
# Common Source Files
# ##################################################################################

set(SOURCE_FILES
        catch.hpp
        test_blocks_math_gain.cpp
        test_blocks_math_sum.cpp
        test_blocks_math_sin.cpp
        test_blocks_math_cos.cpp
        test_blocks_math_tan.cpp
        test_blocks_math_arccosh.cpp
        test_blocks_math_arcsinh.cpp
        test_blocks_math_constpow.cpp
        test_blocks_sources_linear.cpp
        test_blocks_sources_step.cpp
        test_blocks_sources_pulse.cpp
        test_blocks_sources_random.cpp
        test_blocks_sources_complex.cpp
        test_blocks_filters_delay.cpp
        test_blocks_sources_audio.cpp
        test_blocks_sources_constant.cpp
        test_blocks_sinks_json_data_extractable.cpp
        test_blocks_sinks_stdout.cpp
        test_blocks_basicop_transpose.cpp
        test_blocks_basicop_duplicator.cpp
        test_plugins_numpy_basic.cpp
        test_blocks_sources_constant.cpp)


# ##################################################################################
# main test application
# ##################################################################################

set(PROJECT_TEST_NAME testsuite)

add_executable(testsuite testsuite.cpp ${SOURCE_FILES})


target_link_libraries(testsuite blocks)
target_link_libraries(testsuite plugins)
target_link_libraries(testsuite http)
target_link_libraries(testsuite socket)
target_link_libraries(testsuite codec)
target_link_libraries(testsuite common)

target_link_libraries(testsuite ${LIBSNDFILE_LIBRARIES} ${PYTHON_LIBRARIES} ${GSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if(NOT CMAKE_COMPILER_IS_GNUCXX)
  if( (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage"))
    target_link_libraries(${PROJECT_TEST_NAME} gcov)
  endif()
endif() # NOT CMAKE_COMPILER_IS_GNUCXX

add_test(NAME RunTests COMMAND testsuite)
