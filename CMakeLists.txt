cmake_minimum_required (VERSION 3.1)
project (QueuePtr)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../include")
include(compilerflags)

file(GLOB boost_dirs "${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/install/*")

find_file(HAVE_BOOST "boost/smart_ptr/intrusive_ptr.hpp" PATHS ${boost_dirs})
if (NOT HAVE_BOOST)
    MESSAGE(FATAL_ERROR "Need to run the makeboost.py script, could not find boost includes")
endif()

get_filename_component(HAVE_BOOST ${HAVE_BOOST} DIRECTORY)
get_filename_component(HAVE_BOOST "${HAVE_BOOST}/../../" ABSOLUTE)
message(STATUS "Found boost: ${HAVE_BOOST}")

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../install")

file(GLOB QueuePtr_INCLUDES "include/*.h")
install(FILES ${QueuePtr_INCLUDES} DESTINATION include/QueuePtr/)

add_subdirectory(src)
add_subdirectory(QueuePtrTester)
