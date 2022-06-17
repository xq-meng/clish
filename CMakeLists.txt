cmake_minimum_required(VERSION 3.3)

project(cli)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
	src/main.cpp
    src/command_list.cpp
    src/command_list.h
    src/command_shell.cpp
    src/command_shell.h)

set(READBUF_SRC
    src/read_buf.cpp
    src/read_buf.h)

if(WIN32)
    add_definitions(-DWINDOWS_PLATFORM)
    add_executable(cli ${SOURCES})
elseif(UNIX)
    add_executable(cli ${SOURCES} ${READBUF_SRC})
endif()

target_include_directories(cli PUBLIC ${CMAKE_SOURCE_DIR}/extern)
