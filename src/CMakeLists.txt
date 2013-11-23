SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(LIBCPPJIEBA_SRC HMMSegment.cpp MixSegment.cpp  MPSegment.cpp  Trie.cpp)
ADD_LIBRARY(cppjieba STATIC ${LIBCPPJIEBA_SRC})
ADD_EXECUTABLE(cjsegment segment.cpp)
ADD_EXECUTABLE(cjserver server.cpp)

LINK_DIRECTORIES(Husky)

TARGET_LINK_LIBRARIES(cjsegment cppjieba)
TARGET_LINK_LIBRARIES(cjserver cppjieba husky pthread)

SET_TARGET_PROPERTIES(cppjieba PROPERTIES VERSION 1.2 SOVERSION 1)

INSTALL(TARGETS cjsegment RUNTIME DESTINATION bin)
INSTALL(TARGETS cjserver RUNTIME DESTINATION bin)
INSTALL(TARGETS cppjieba ARCHIVE DESTINATION lib/CppJieba)
INSTALL(FILES ChineseFilter.hpp HMMSegment.h MPSegment.h structs.h Trie.h globals.h ISegment.hpp  MixSegment.h  SegmentBase.hpp  TransCode.hpp  DESTINATION include/CppJieba)


ADD_SUBDIRECTORY(Husky)
ADD_SUBDIRECTORY(Limonp)
