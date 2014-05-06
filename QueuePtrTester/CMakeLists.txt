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

# custom find boost start
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(BOOSTDIR "$ENV{BOOST_ROOT}/stage/debug")
else()
    set(BOOSTDIR "$ENV{BOOST_ROOT}/stage/x86/lib")
endif()
find_library(BOOST_SYSTEM libboost_system${CMAKE_STATIC_LIBRARY_SUFFIX} ${BOOSTDIR})
find_library(BOOST_CHRONO libboost_chrono${CMAKE_STATIC_LIBRARY_SUFFIX} ${BOOSTDIR})
find_library(BOOST_THREAD libboost_thread${CMAKE_STATIC_LIBRARY_SUFFIX} ${BOOSTDIR})
add_definitions(-DBOOST_ALL_NO_LIB)

target_link_libraries(QueuePtrTester ${BOOST_SYSTEM} ${BOOST_CHRONO} ${BOOST_THREAD} QueuePtr)
include_directories($ENV{BOOST_ROOT}  ../include)
include(compilerflags)
