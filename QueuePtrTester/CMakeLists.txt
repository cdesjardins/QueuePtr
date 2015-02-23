file(GLOB SRCS
    "*.h"
    "*.cpp"
)

add_executable(QueuePtrTester ${SRCS})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(QueuePtrTester rt)
endif()

# I typically use the "system" layout for boost, which cmake findboost 
# does not (yet) support. To use normal boost libs, just uncomment the next
# 3 lines, and delete everything between the custom find boost start and end
# comments.

#find_package(Boost COMPONENTS system thread chrono REQUIRED)
#target_link_libraries(QueuePtr ${Boost_LIBRARIES})
#include_directories(${Boost_INCLUDE_DIR})
message(STATUS ${BOOST_ROOT})
add_definitions(-DBOOST_ALL_NO_LIB)
target_link_libraries(QueuePtrTester QueuePtr)
include_directories(${BOOST_ROOT}  ../include)
include(compilerflags)
set_property(TARGET QueuePtrTester PROPERTY CXX_STANDARD 11)

