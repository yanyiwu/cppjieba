SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_EXECUTABLE(cjsegment segment.cpp)
ADD_EXECUTABLE(cjserver server.cpp)
TARGET_LINK_LIBRARIES(cjserver pthread)

INSTALL(TARGETS cjsegment RUNTIME DESTINATION bin)
INSTALL(TARGETS cjserver RUNTIME DESTINATION bin)
INSTALL(FILES ChineseFilter.hpp HMMSegment.hpp MPSegment.hpp Trie.hpp ISegment.hpp  MixSegment.hpp  SegmentBase.hpp  TransCode.hpp  DESTINATION include/CppJieba)

ADD_SUBDIRECTORY(Husky)
ADD_SUBDIRECTORY(Limonp)
