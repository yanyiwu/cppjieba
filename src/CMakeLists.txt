SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

ADD_EXECUTABLE(cjsegment segment.cpp)
ADD_EXECUTABLE(cjserver server.cpp)
TARGET_LINK_LIBRARIES(cjserver pthread)

INSTALL(TARGETS cjsegment RUNTIME DESTINATION bin)
INSTALL(TARGETS cjserver RUNTIME DESTINATION bin)
INSTALL(FILES HMMSegment.hpp MPSegment.hpp Trie.hpp TrieManager.hpp ISegment.hpp  MixSegment.hpp  SegmentBase.hpp  TransCode.hpp KeywordExtractor.hpp DESTINATION include/CppJieba)

ADD_SUBDIRECTORY(Husky)
ADD_SUBDIRECTORY(Limonp)
