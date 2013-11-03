SET(LIBCPPJIEBA_SRC HMMSegment.cpp  KeyWordExt.cpp  MixSegment.cpp  MPSegment.cpp  Trie.cpp)

INCLUDE_DIRECTORIES(../limonp)

ADD_LIBRARY(cppjieba SHARED ${LIBCPPJIEBA_SRC})

SET_TARGET_PROPERTIES(cppjieba PROPERTIES VERSION 1.2 SOVERSION 1)

INSTALL(TARGETS cppjieba LIBRARY DESTINATION lib/CppJieba)
INSTALL(FILES ChineseFilter.hpp HMMSegment.h KeyWordExt.h MPSegment.h structs.h Trie.h globals.h          ISegment.hpp  MixSegment.h  SegmentBase.hpp  TransCode.hpp  DESTINATION include/CppJieba)
