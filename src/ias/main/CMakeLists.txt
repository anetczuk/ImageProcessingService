#
#
#


set( TARGET_NAME iascli )


ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)


find_package(Threads)
find_package(Boost 1.54 COMPONENTS log log_setup thread system REQUIRED)


include_directories( ${Boost_INCLUDE_DIR} )
include_directories( "../include" )

set( EXT_LIBS ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ias )


add_executable( ${TARGET_NAME} ias.cpp )
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )


## copy run script
file(GLOB_RECURSE test_files test/* )
file(COPY ${test_files} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test )