cmake_minimum_required(VERSION 3.0.2)
project(myproject)

find_package(PkgConfig) 
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

include_directories(./include)

file(GLOB SRC ./src/*.cpp)

option(DEBUG OFF)

if (DEBUG) 
    add_definitions(-D DEBUG)
    SET(CMAKE_BUILD_TYPE "Debug")
    SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g2 -ggdb")

endif(DEBUG)

add_executable(main 
    main.cpp
    ${SRC}
)

target_link_libraries(main 
    ${GTKMM_LIBRARIES}  )