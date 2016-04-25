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

add_executable(websocket_demo websocket_demo.cpp ${SOURCE_FILES})

target_link_libraries(websocket_demo blocks)
target_link_libraries(websocket_demo plugins)
target_link_libraries(websocket_demo http)
target_link_libraries(websocket_demo socket)
target_link_libraries(websocket_demo codec)
target_link_libraries(websocket_demo common)

target_link_libraries(websocket_demo ${LIBSNDFILE_LIBRARIES} ${PYTHON_LIBRARIES} ${GSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# define more applications if exists
