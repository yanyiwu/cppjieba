if (MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-DLOGGING_LEVEL=LL_WARNING)

ADD_EXECUTABLE(test.run 
    gtest_main.cpp 
    keyword_extractor_test.cpp 
    trie_test.cpp 
    segments_test.cpp 
    pos_tagger_test.cpp
    jieba_test.cpp
    pre_filter_test.cpp
    unicode_test.cpp
    textrank_test.cpp
)

TARGET_LINK_LIBRARIES(test.run gtest)
