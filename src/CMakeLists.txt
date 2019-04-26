set (SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/deps/easyloggingpp/src/easylogging++.h
        ${PROJECT_SOURCE_DIR}/deps/easyloggingpp/src/easylogging++.cc
        ${PROJECT_SOURCE_DIR}/deps/mongoose/mongoose.h
        ${PROJECT_SOURCE_DIR}/deps/mongoose/mongoose.c
        ${PROJECT_SOURCE_DIR}/deps/json/single_include/nlohmann/json.hpp
        atomic.hpp clock_time.hpp config.hpp define.hpp
        logger.hpp logger.cpp
        http_server.cpp http_server.hpp http_service.cpp http_service.hpp
        noncopyable.hpp util.hpp logo.hpp
        statistics.hpp buffer.hpp
        demo_server.cpp demo_server.hpp
        demo_client.cpp demo_client.hpp)

add_library(demo-base ${SOURCE_FILES})
add_executable (demo main.cpp)
target_link_libraries(demo demo-base)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries (demo ${PROJECT_SOURCE_DIR}/deps/libuv/lib64/libuv_a.a pthread)
ELSE()
    target_link_libraries (demo ${PROJECT_SOURCE_DIR}/deps/libuv/lib/libuv_a.a pthread)
ENDIF()
