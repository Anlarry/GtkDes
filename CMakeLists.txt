cmake_minimum_required(VERSION 3.0.2)
project(myproject)

find_package(PkgConfig) 
pkg_check_modules(GTKMM gtkmm-3.0)
include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

include_directories(
    ./Gtk/include 
    ./Algorithm/include
)

include_directories(Algorithm )
include_directories(Gtk )

add_subdirectory(Gtk)
add_subdirectory(Algorithm)

add_executable(main main.cpp)
target_link_libraries(main Gtk Algorithm)