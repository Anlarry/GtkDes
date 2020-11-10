cmake_minimum_required(VERSION 3.0.2)
project(myproject)

find_package(PkgConfig)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools REQUIRED)

include_directories(${QT_INCLUDE_DIRS})

pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

include_directories(
    ./Gtk/include 
    ./Algorithm/include
)

include_directories(Algorithm )
include_directories(Gtk )
include_directories(StringProcess)

add_subdirectory(Gtk)
add_subdirectory(Algorithm)
add_subdirectory(StringProcess)
add_subdirectory(Qt)

option(DEBUG off)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread ")

if (DEBUG)
    add_definitions(-D DEBUG)
    SET(CMAKE_BUILD_TYPE "Debug")
    SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g2 -ggdb")
endif(DEBUG)

add_executable(main main.cpp)
target_link_libraries(main Gtk Algorithm)