SET(LIBCPPJIEBA_SRC HMMSegment.cpp  KeyWordExt.cpp  MixSegment.cpp  MPSegment.cpp  Trie.cpp)
INCLUDE_DIRECTORIES(../limonp)
ADD_LIBRARY(cppjieba SHARED ${LIBCPPJIEBA_SRC})
