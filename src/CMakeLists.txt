file(GLOB LIBSRCS
    "../*.h"
    "*.h"
    "*.cpp"
)

add_library(QueuePtr STATIC ${LIBSRCS})

include_directories(${BOOST_ROOT} ../include)
add_definitions(-DBOOST_ALL_NO_LIB)
set_property(TARGET QueuePtr PROPERTY CXX_STANDARD 11)
install(TARGETS QueuePtr DESTINATION lib)

