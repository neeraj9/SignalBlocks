# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

# ##################################################################################
# Common Source Files
# ##################################################################################

set(SOURCE_FILES
        catch.hpp
        test_blocks_math_gain.cpp
        test_blocks_math_sum.cpp
        test_blocks_math_cos.cpp
        test_blocks_math_arccosh.cpp
        test_blocks_math_arcsinh.cpp
        test_blocks_sources_linear.cpp
        test_blocks_sources_pulse.cpp
        test_blocks_sources_random.cpp
        test_blocks_sources_complex.cpp
        test_blocks_filters_delay.cpp
        test_blocks_sources_audio.cpp test_blocks_sources_constant.cpp test_blocks_sinks_json_data_extractable.cpp test_blocks_basicop_transpose.cpp test_plugins_numpy_basic.cpp)

# ##################################################################################
# main sample application
# ##################################################################################

add_executable(testsuite testsuite.cpp ${SOURCE_FILES})


target_link_libraries(testsuite blocks)
target_link_libraries(testsuite plugins)
target_link_libraries(testsuite http)
target_link_libraries(testsuite socket)
target_link_libraries(testsuite codec)
target_link_libraries(testsuite common)

target_link_libraries(testsuite ${LIBSNDFILE_LIBRARIES} ${PYTHON_LIBRARIES} ${GSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# unit test via CTest
enable_testing()
add_test(NAME RunTests COMMAND testsuite)
