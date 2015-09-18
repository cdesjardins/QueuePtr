file(GLOB SRCS
    "*.h"
    "*.cpp"
)

add_executable(QueuePtrTester ${SRCS})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(QueuePtrTester rt pthread)
endif()

add_definitions(-DBOOST_ALL_NO_LIB)
target_link_libraries(QueuePtrTester QueuePtr)
include_directories(${HAVE_BOOST}  ../include)
set_property(TARGET QueuePtrTester PROPERTY CXX_STANDARD 11)
install(TARGETS QueuePtrTester DESTINATION bin)

