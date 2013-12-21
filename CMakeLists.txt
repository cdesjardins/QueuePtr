cmake_minimum_required (VERSION 2.6)
project (QueuePtr)
file(GLOB SRCS
    ${SRCS}
    "*.h"
    "*.cpp"
)

add_executable(QueuePtr ${SRCS})

# I typically use the "system" layout for boost, which cmake findboost 
# does not (yet) support. To use normal boost libs, just uncomment the next
# 3 lines, and delete everything between the custom find boost start and end
# comments.

#find_package(Boost COMPONENTS system thread chrono REQUIRED)
#target_link_libraries(QueuePtr ${Boost_LIBRARIES})
#include_directories(${Boost_INCLUDE_DIR})

# custom find boost start
set(BOOSTLIBS 
    "libboost_system${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "libboost_thread${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "libboost_chrono${CMAKE_STATIC_LIBRARY_SUFFIX}")
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(BOOSTDIR "$ENV{BOOST_ROOT}/stage/debug")
else()
    set(BOOSTDIR "$ENV{BOOST_ROOT}/stage/x86/lib")
endif()

if(MSVC)
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MD")
    endforeach(flag_var)
endif()

message(STATUS ${BOOSTDIR})
include_directories($ENV{BOOST_ROOT})
add_definitions(-DBOOST_ALL_NO_LIB)

foreach(BL ${BOOSTLIBS})
    target_link_libraries(QueuePtr ${BOOSTDIR}/${BL})
endforeach()
# custom find boost end

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(QueuePtr rt)
endif()
