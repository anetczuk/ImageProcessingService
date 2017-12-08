#
#
#


set( TARGET_NAME ias )


include_directories( "../include" )


set( EXT_LIBS ${OpenCV_LIBS} )


file(GLOB_RECURSE cpp_files *.cpp )


add_library( ${TARGET_NAME} SHARED ${cpp_files} )
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )
