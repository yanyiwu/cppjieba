SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(GTEST_ROOT_DIR gtest)

ADD_DEFINITIONS(-DLOGGER_LEVEL=LL_WARN)
INCLUDE_DIRECTORIES(${GTEST_ROOT_DIR} ${GTEST_ROOT_DIR}/include ${PROJECT_SOURCE_DIR})
ADD_LIBRARY(gtest STATIC ${GTEST_ROOT_DIR}/src/gtest-all.cc)

ADD_EXECUTABLE(test.run 
    gtest_main.cpp 
    keyword_extractor_test.cpp 
    trie_test.cpp 
    segments_test.cpp 
    pos_tagger_test.cpp
    application_test.cpp
    pre_filter_test.cpp
)
TARGET_LINK_LIBRARIES(gtest pthread)
TARGET_LINK_LIBRARIES(test.run gtest pthread)

