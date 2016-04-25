# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

# ##################################################################################
# Common Source Files
# ##################################################################################

set(SOURCE_FILES
        )

# ##################################################################################
# main sample application
# ##################################################################################

add_executable(histograms histograms.cpp ${SOURCE_FILES})

target_link_libraries(histograms blocks)
target_link_libraries(histograms plugins)
target_link_libraries(histograms http)
target_link_libraries(histograms socket)
target_link_libraries(histograms codec)
target_link_libraries(histograms common)

target_link_libraries(histograms ${LIBSNDFILE_LIBRARIES} ${PYTHON_LIBRARIES} ${GSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# define more applications if exists
