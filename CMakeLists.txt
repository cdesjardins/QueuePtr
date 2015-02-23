cmake_minimum_required (VERSION 3.1)
project (QueuePtr)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../include")
include(compilerflags)
set(BOOST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../boost/boost_1_57_0)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../install")

file(GLOB QueuePtr_INCLUDES "include/*.h")
install(FILES ${QueuePtr_INCLUDES} DESTINATION include/QueuePtr/)

add_subdirectory(src)
add_subdirectory(QueuePtrTester)
