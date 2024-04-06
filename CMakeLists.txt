cmake_minimum_required(VERSION 3.27)
project(Kivi)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(Kivi PhaseShifting.cpp
)
# linking
target_link_libraries(Kivi ${OpenCV_LIBS})
#find_package( OpenCV REQUIRED )
#set(SOURCE_FILES main.cpp)
#set( OpenCV_DIR "E:/OpenCV/opencv/build" )
#set( OpenCV_INCLUDE_DIR "E:/OpenCV/opencv/build/include" )
#add_executable(${Kivi} ${SOURCE_FILES})
#include_directories(${OpenCV_INCLUDE_DIRS})
#target_link_libraries( ${Kivi} ${OpenCV_LIBS} )