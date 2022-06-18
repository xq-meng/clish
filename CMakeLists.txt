cmake_minimum_required(VERSION 3.3)

project(cli)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
	src/main.cpp)

if(WIN32)
    add_definitions(-DWINDOWS_PLATFORM)
endif()

add_executable(cli ${SOURCES})

target_include_directories(cli PUBLIC ${CMAKE_SOURCE_DIR}/include)
