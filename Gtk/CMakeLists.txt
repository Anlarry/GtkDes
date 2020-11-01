link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

file(GLOB SRC ./src/*.cpp)

option(DEBUG OFF)

if (DEBUG) 
    add_definitions(-D DEBUG)
    SET(CMAKE_BUILD_TYPE "Debug")
    SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g2 -ggdb")

endif(DEBUG)

# add_executable(main 
#     main.cpp
#     ${SRC}
# )
add_library(Gtk ${SRC})

target_link_libraries(Gtk 
    ${GTKMM_LIBRARIES}  )