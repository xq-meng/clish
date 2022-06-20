cmake_minimum_required(VERSION 3.3)

project(clish)

option(BUILD_TEST_EXECUTABLE    "Build test executable."    ON)

if(WIN32)
    add_definitions(-DWINDOWS_PLATFORM)
endif()

add_library(clish INTERFACE)

add_library(clish::clish ALIAS clish)

target_include_directories(clish INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(clish INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/clish/clish.h)

if(${CMAKE_VERSION} VERSION_GREATER "3.7") 
    target_compile_features(clish INTERFACE cxx_std_11)
endif()

if(BUILD_TEST_EXECUTABLE)
    add_executable(clish_test ./test/clish_test.cpp)
    target_link_libraries(clish_test PRIVATE clish::clish)
endif()
