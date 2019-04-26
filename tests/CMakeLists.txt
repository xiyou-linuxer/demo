include_directories (${PROJECT_SOURCE_DIR}/deps/gtest/include)
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    link_directories (${PROJECT_SOURCE_DIR}/deps/gtest/lib64)
ELSE()
    link_directories (${PROJECT_SOURCE_DIR}/deps/gtest/lib)
ENDIF()

set (GTEST_LIB gtest pthread)

add_executable(buffer_test buffer_test.cpp)
target_link_libraries(buffer_test ${GTEST_LIB})
add_test(NAME buffer_test COMMAND buffer_test)

add_executable(atomic_test atomic_test.cpp)
target_link_libraries(atomic_test ${GTEST_LIB})
add_test(NAME atomic_test COMMAND atomic_test)

add_executable(clock_time_test clock_time_test.cpp)
target_link_libraries(clock_time_test ${GTEST_LIB} demo-base)
add_test(NAME clock_time_test COMMAND clock_time_test)
