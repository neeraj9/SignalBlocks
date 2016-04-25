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

add_executable(signalblocks main.cpp ${SOURCE_FILES})


target_link_libraries(signalblocks blocks)
target_link_libraries(signalblocks plugins)
target_link_libraries(signalblocks http)
target_link_libraries(signalblocks socket)
target_link_libraries(signalblocks codec)
target_link_libraries(signalblocks common)

target_link_libraries(signalblocks ${LIBSNDFILE_LIBRARIES} ${PYTHON_LIBRARIES} ${GSL_LIBRARIES} ${SCTP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# define more applications if exists