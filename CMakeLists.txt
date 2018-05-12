cmake_minimum_required(VERSION 3.5)
project(roboconVisionHM)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp serial.cpp Info.cpp Info.h
        lineTest.cpp lineInfo.cpp linesOption.cpp
        rtlFinder.cpp rtlFinder.h rtlInfo.cpp rtlInfo.h)

add_executable(roboconVisionHM ${SOURCE_FILES})

target_link_libraries(roboconVisionHM ${OpenCV_LIBS} pthread)