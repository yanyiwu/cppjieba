ENABLE_TESTING()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# Configure test paths
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test_paths.h.in" "${CMAKE_BINARY_DIR}/test/test_paths.h")

# Add include directories
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/test
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/deps/limonp/include
    ${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include
)

# Add Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Add UTF-8 support for MSVC
if(MSVC)
  add_compile_options(/utf-8)
endif()

ADD_EXECUTABLE(load_test load_test.cpp)
TARGET_LINK_LIBRARIES(load_test gtest gtest_main)

ADD_SUBDIRECTORY(unittest)

# Add test configurations
ADD_TEST(NAME load_test COMMAND load_test)
SET_TESTS_PROPERTIES(load_test PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
