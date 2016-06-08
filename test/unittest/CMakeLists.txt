SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/deps/gtest/include)

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

if(MSVC)
	TARGET_LINK_LIBRARIES(test.run gtest)
else()
	TARGET_LINK_LIBRARIES(test.run gtest pthread)
endif()
