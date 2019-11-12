# using cplex lib
add_definitions(-DUSE_CPLEX)
find_package(CPLEX REQUIRED)
include_directories(${CPLEX_INCLUDE_DIRS})
set(LIBS ${LIBS} ${CPLEX_LIBRARIES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DNDEBUG -DIL_STD -pthread -W ${LINUX_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 14)