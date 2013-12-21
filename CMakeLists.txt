cmake_minimum_required (VERSION 2.6)
project (QueuePtr)

find_package(Boost COMPONENTS system thread chrono REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
file(GLOB SRCS
    ${SRCS}
    "*.h"
    "*.cpp"
)

add_executable(QueuePtr ${SRCS})
target_link_libraries(QueuePtr ${Boost_LIBRARIES})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(QueuePtr rt)
endif()
