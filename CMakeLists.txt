cmake_minimum_required(VERSION 3.17)

project(VirtualCamera)

set(CMAKE_CXX_STANDARD 17)

file(GLOB STRMBASE_SRC ${PROJECT_SOURCE_DIR}/baseclasses/*.cpp ${PROJECT_SOURCE_DIR}/baseclasses/*.h)

add_library(strmbase STATIC ${STRMBASE_SRC})

target_include_directories(strmbase SYSTEM BEFORE PRIVATE baseclasses)

target_link_libraries(strmbase PRIVATE Strmiids)

add_library(VirtualCamera SHARED VirtualCamera.def DllMain.cpp CVirtualCamera.cpp CVirtualCameraPin.cpp CVirtualCameraPin.h)

target_include_directories(VirtualCamera SYSTEM BEFORE PRIVATE baseclasses)

target_link_libraries(VirtualCamera PRIVATE strmbase winmm)

add_dependencies(VirtualCamera strmbase)
