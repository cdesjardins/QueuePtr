cmake_minimum_required (VERSION 3.1)
project (QueuePtr)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../include")
include(compilerflags)

file(GLOB boost_dirs ${CMAKE_CURRENT_SOURCE_DIR}/../boost/boost_1_*_0)

find_library(HAVE_BOOST "boost_system" PATHS ${boost_dirs} PATH_SUFFIXES "stage/lib")
if (NOT HAVE_BOOST)
    MESSAGE(FATAL_ERROR "Need to run the makeboost.py script, could not find boost includes")
endif()
get_filename_component(HAVE_BOOST ${HAVE_BOOST} DIRECTORY)
set(HAVE_BOOST ${HAVE_BOOST}/../..)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../install")

file(GLOB QueuePtr_INCLUDES "include/*.h")
install(FILES ${QueuePtr_INCLUDES} DESTINATION include/QueuePtr/)

add_subdirectory(src)
add_subdirectory(QueuePtrTester)
