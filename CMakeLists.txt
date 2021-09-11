cmake_minimum_required(VERSION 3.17)

project(VirtualCamera)

set(CMAKE_CXX_STANDARD 17)

file(GLOB STRMBASE_SRC ${PROJECT_SOURCE_DIR}/baseclasses/*.cpp ${PROJECT_SOURCE_DIR}/baseclasses/*.h)

add_library(strmbase STATIC ${STRMBASE_SRC})

target_include_directories(strmbase SYSTEM BEFORE PRIVATE baseclasses)

target_link_libraries(strmbase PRIVATE Strmiids)

add_library(VirtualCamera SHARED VirtualCamera.def DllMain.cpp CVirtualCamera.cpp CVirtualCameraPin.cpp)

target_include_directories(VirtualCamera SYSTEM BEFORE PRIVATE baseclasses)

target_link_libraries(VirtualCamera PRIVATE strmbase winmm)

add_dependencies(VirtualCamera strmbase)

set(libdshowcapture_SOURCES
        libdshowcapture/source/capture-filter.cpp
        libdshowcapture/source/output-filter.cpp
        libdshowcapture/source/dshowcapture.cpp
        libdshowcapture/source/dshowencode.cpp
        libdshowcapture/source/device.cpp
        libdshowcapture/source/encoder.cpp
        libdshowcapture/source/dshow-base.cpp
        libdshowcapture/source/dshow-demux.cpp
        libdshowcapture/source/dshow-enum.cpp
        libdshowcapture/source/dshow-formats.cpp
        libdshowcapture/source/dshow-media-type.cpp
        libdshowcapture/source/dshow-encoded-device.cpp
        libdshowcapture/source/log.cpp)

set(libdshowcapture_HEADERS
        libdshowcapture/dshowcapture.hpp
        libdshowcapture/source/external/IVideoCaptureFilter.h
        libdshowcapture/source/capture-filter.hpp
        libdshowcapture/source/output-filter.hpp
        libdshowcapture/source/device.hpp
        libdshowcapture/source/encoder.hpp
        libdshowcapture/source/dshow-base.hpp
        libdshowcapture/source/dshow-demux.hpp
        libdshowcapture/source/dshow-device-defs.hpp
        libdshowcapture/source/dshow-enum.hpp
        libdshowcapture/source/dshow-formats.hpp
        libdshowcapture/source/dshow-media-type.hpp
        libdshowcapture/source/log.hpp)

add_library(libdshowcapture STATIC ${libdshowcapture_SOURCES} ${libdshowcapture_HEADERS})

target_compile_definitions(libdshowcapture PRIVATE UNICODE)

target_link_libraries(libdshowcapture strmiids ksuser wmcodecdspuuid)

add_library(VirtualCamera2 SHARED VirtualCamera.def DllMain2.cpp CVirtualCamera2.cpp)

target_link_libraries(VirtualCamera2 PRIVATE winmm libdshowcapture)

add_dependencies(VirtualCamera2 libdshowcapture)
