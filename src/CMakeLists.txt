
file(GLOB LIBSRCS
    "../*.h"
    "*.h"
    "*.cpp"
)

add_library(QueuePtr STATIC ${LIBSRCS})
message(STATUS "src ${BOOST_ROOT}")

include_directories(${BOOST_ROOT} ../include)
add_definitions(-DBOOST_ALL_NO_LIB)
include(compilerflags)
set_property(TARGET QueuePtr PROPERTY CXX_STANDARD 11)

