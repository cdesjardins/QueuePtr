cmake_minimum_required (VERSION 2.6)
project (QueuePtr)

#find_package(Boost COMPONENTS system thread chrono REQUIRED)
set(BOOSTLIBS 
    "libboost_system${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "libboost_thread${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "libboost_chrono${CMAKE_STATIC_LIBRARY_SUFFIX}")
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(BOOSTDIR "$ENV{BOOST_LIBRARYDIR}/stage/debug")
else()
    set(BOOSTDIR "$ENV{BOOST_LIBRARYDIR}/stage/x86/lib")
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
include_directories($ENV{BOOST_LIBRARYDIR})
add_definitions(-DBOOST_ALL_NO_LIB)
file(GLOB SRCS
    ${SRCS}
    "*.h"
    "*.cpp"
)

add_executable(QueuePtr ${SRCS})
#target_link_libraries(QueuePtr ${Boost_LIBRARIES})
foreach(BL ${BOOSTLIBS})
    target_link_libraries(QueuePtr ${BOOSTDIR}/${BL})
endforeach()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(QueuePtr rt)
endif()
