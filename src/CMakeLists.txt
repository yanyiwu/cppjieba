ADD_EXECUTABLE(segment.demo segment.cpp)
ADD_EXECUTABLE(server.demo server.cpp)
ADD_EXECUTABLE(keywordext.demo keywordext.cpp)
INCLUDE_DIRECTORIES(../limonp ../cppjieba ../husky)
LINK_DIRECTORIES(../cppjieba ../husky)
TARGET_LINK_LIBRARIES(segment.demo cppjieba)
TARGET_LINK_LIBRARIES(server.demo cppjieba husky pthread)

SET(LIBCPPJIEBA_SRC HMMSegment.cpp MixSegment.cpp  MPSegment.cpp  Trie.cpp)

INCLUDE_DIRECTORIES(../limonp)

ADD_LIBRARY(cppjieba SHARED ${LIBCPPJIEBA_SRC})

SET_TARGET_PROPERTIES(cppjieba PROPERTIES VERSION 1.2 SOVERSION 1)

INSTALL(TARGETS cppjieba LIBRARY DESTINATION lib/CppJieba)
INSTALL(FILES ChineseFilter.hpp HMMSegment.h MPSegment.h structs.h Trie.h globals.h ISegment.hpp  MixSegment.h  SegmentBase.hpp  TransCode.hpp  DESTINATION include/CppJieba)
