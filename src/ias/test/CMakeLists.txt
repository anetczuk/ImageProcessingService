#
#
#


set( TARGET_NAME ias_test )


include_directories( "../include" )


find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set( EXT_LIBS ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ias )


file(GLOB_RECURSE cpp_files *.cpp )


add_executable( ${TARGET_NAME} ${cpp_files} )
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )


#TODO: copying data and scripts can be optimised by creating script from template with defined data directry

## copy run script
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/runTests.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

## copy data files
file(GLOB_RECURSE data_files data/* )
file(COPY ${data_files} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data )
