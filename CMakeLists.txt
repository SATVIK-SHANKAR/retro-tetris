# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(RetroTetris)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/PDCurses)
    link_directories(${CMAKE_SOURCE_DIR}/PDCurses)
    set(CURSES_LIBRARIES pdcurses)
else()
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    set(CURSES_LIBRARIES ${CURSES_LIBRARIES})
endif()

add_executable(tetris tetris.cpp)
target_link_libraries(tetris ${CURSES_LIBRARIES})
