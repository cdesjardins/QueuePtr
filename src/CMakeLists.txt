file(GLOB LIBSRCS
    "../*.h"
    "*.h"
    "*.cpp"
)

add_library(QueuePtr STATIC ${LIBSRCS})

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/../include)
set_property(TARGET QueuePtr PROPERTY CXX_STANDARD 11)
install(TARGETS QueuePtr DESTINATION lib)

