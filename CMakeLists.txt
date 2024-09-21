CMAKE_MINIMUM_REQUIRED (VERSION 3.5)
PROJECT(CPPJIEBA)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/deps/limonp/include
  ${PROJECT_SOURCE_DIR}/include)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "/usr/local/cppjieba" CACHE PATH "default install path" FORCE )
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

ADD_DEFINITIONS(-O3 -g)

# Define a variable to check if this is the top-level project
if(NOT DEFINED CPPJIEBA_TOP_LEVEL_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(CPPJIEBA_TOP_LEVEL_PROJECT ON)
    else()
        set(CPPJIEBA_TOP_LEVEL_PROJECT OFF)
    endif()
endif()


option(CPPJIEBA_BUILD_TESTS "Build cppjieba tests" ${CPPJIEBA_TOP_LEVEL_PROJECT})

if(CPPJIEBA_BUILD_TESTS)
  ENABLE_TESTING()

  ADD_SUBDIRECTORY(test)
  if(NOT MSVC)
    ADD_TEST(NAME ./test/test.run COMMAND ./test/test.run)
    ADD_TEST(NAME ./load_test COMMAND ./load_test)
  endif()
endif()


# ... other CMake configurations ...
