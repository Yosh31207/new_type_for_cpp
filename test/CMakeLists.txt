cmake_minimum_required(VERSION 3.0)
project(new_type_test CXX)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(new_type_test
    ./src/new_type_test.cpp
)
set_target_properties(new_type_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(new_type_test
    PUBLIC
    ../include/
)
target_compile_features(new_type_test
    PUBLIC
    cxx_std_17
)
target_compile_options(new_type_test
    PUBLIC
    -Wall
    -Wextra
)
target_link_libraries(
    new_type_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(new_type_test)
