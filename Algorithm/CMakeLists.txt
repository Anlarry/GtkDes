include_directories(./include)

file(GLOB SRC ./src/*.cpp)

option(DEBUG off)

if (DEBUG)
    add_definitions(-D DEBUG)
    SET(CMAKE_BUILD_TYPE "Debug")
    SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g2 -ggdb")
endif(DEBUG)

add_library(Algorithm ${SRC})


