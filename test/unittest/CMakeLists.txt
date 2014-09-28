SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(GTEST_ROOT_DIR gtest-1.6.0)

ADD_DEFINITIONS(-DLOGGER_LEVEL=LL_WARN)
INCLUDE_DIRECTORIES(${GTEST_ROOT_DIR} ${GTEST_ROOT_DIR}/include ${PROJECT_SOURCE_DIR})
ADD_LIBRARY(gtest STATIC ${GTEST_ROOT_DIR}/src/gtest-all.cc)

#ADD_EXECUTABLE(segments.test gtest_main.cpp TSegments.cpp)
#TARGET_LINK_LIBRARIES(segments.test gtest pthread)
#
#ADD_EXECUTABLE(trie.test gtest_main.cpp TTrie.cpp)
#TARGET_LINK_LIBRARIES(trie.test gtest pthread)

#ADD_EXECUTABLE(keyword.test gtest_main.cpp TKeywordExtractor.cpp)
#TARGET_LINK_LIBRARIES(keyword.test gtest pthread)

ADD_EXECUTABLE(test.run 
    gtest_main.cpp 
    TKeywordExtractor.cpp 
    TTrie.cpp 
    TSegments.cpp 
    TPosTagger.cpp
)
TARGET_LINK_LIBRARIES(gtest pthread)
TARGET_LINK_LIBRARIES(test.run gtest pthread)

