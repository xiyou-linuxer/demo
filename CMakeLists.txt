cmake_minimum_required(VERSION 2.8)

project(Demo)

set(CMAKE_CXX_STANDARD 11)

enable_testing()

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set (CMAKE_CXX_COMPILER "/usr/local/gcc-4.9.2/bin/g++")
    set(CXX_FLAGS
            -g
            -O0
            -std=c++11
            #-DNDEBUG
            -D_FILE_OFFSET_BITS=64
            -DELPP_THREAD_SAFE
            -DELPP_DISABLE_DEFAULT_CRASH_HANDLING
            -DELPP_DISABLE_PERFORMANCE_TRACKING
            -DELPP_NO_DEFAULT_LOG_FILE
            -static-libgcc -static-libstdc++
            -fno-omit-frame-pointer
            -fsanitize=address,leak,undefined,null
            )
ELSE()
    set (CMAKE_CXX_COMPILER "clang++")
    set(CXX_FLAGS
            -g
            -O2
            -std=c++11
            #-DNDEBUG
            -D_FILE_OFFSET_BITS=64
            -DELPP_THREAD_SAFE
            -DELPP_DISABLE_DEFAULT_CRASH_HANDLING
            -DELPP_DISABLE_PERFORMANCE_TRACKING
            -DELPP_NO_DEFAULT_LOG_FILE
            )
ENDIF()

string (REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories (${PROJECT_SOURCE_DIR}/src)
include_directories (${PROJECT_SOURCE_DIR}/deps/json/single_include/nlohmann/)
include_directories (${PROJECT_SOURCE_DIR}/deps/mongoose)
include_directories (${PROJECT_SOURCE_DIR}/deps/easyloggingpp/src/)
include_directories (${PROJECT_SOURCE_DIR}/deps/libuv/include)

file (COPY etc/demo.conf DESTINATION ${PROJECT_BINARY_DIR}/etc)

link_directories (/usr/local/lib)

add_subdirectory (src)
add_subdirectory (tests)

