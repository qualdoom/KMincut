cmake_minimum_required(VERSION 3.15)

project(KMinCut
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(algo_lib
    src/minkcut_2_factor.cpp
    src/stupid.cpp
)

add_executable(solution src/main.cpp)

target_include_directories(algo_lib PUBLIC src)
target_link_libraries(solution PRIVATE algo_lib)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests tests/test_main.cpp)
target_link_libraries(unit_tests 
    PRIVATE 
        algo_lib 
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)