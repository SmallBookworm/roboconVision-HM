cmake_minimum_required(VERSION 3.4)
project(roboconVisionHM)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp serial.cpp Info.cpp Info.h
        lineTest.cpp lineInfo.cpp linesOption.cpp
        rtlFinder.cpp rtlFinder.h rtlInfo.cpp rtlInfo.h
        joystick.cc ballTake.cpp ballTake.h ballTakeOption.cpp ballTakeOption.h control.cpp control.h controlInfo.cpp controlInfo.h ThreadInfo.cpp ThreadInfo.h)

add_executable(roboconVisionHM ${SOURCE_FILES})

target_link_libraries(roboconVisionHM ${OpenCV_LIBS} pthread)