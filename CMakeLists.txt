cmake_minimum_required(VERSION 3.27)
project(Kivi)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(Kivi main.cpp
        DoubleThreeStepPhaseShifting.cpp
        DoubleThreeStepPhaseShifting.h
        CorrespondenceMatching.cpp
        CorrespondenceMatching.h
        BilateralFilter.cpp
        BilateralFilter.h)

target_link_libraries(Kivi ${OpenCV_LIBS})
