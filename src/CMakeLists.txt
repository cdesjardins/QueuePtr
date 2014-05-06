
file(GLOB LIBSRCS
    "../*.h"
    "*.h"
    "*.cpp"
)

add_library(QueuePtr STATIC ${LIBSRCS})

include_directories($ENV{BOOST_ROOT} ../include)
add_definitions(-DBOOST_ALL_NO_LIB)
include(compilerflags)
