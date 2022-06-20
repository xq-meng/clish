cmake_minimum_required(VERSION 3.3)

project(clish_test)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
	test/main.cpp)

if(WIN32)
    add_definitions(-DWINDOWS_PLATFORM)
endif()

add_executable(clish_test ${SOURCES})

target_include_directories(clish_test PUBLIC ${CMAKE_SOURCE_DIR}/include)
