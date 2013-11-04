SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(LIBCPPJIEBA_SRC HMMSegment.cpp MixSegment.cpp  MPSegment.cpp  Trie.cpp)
ADD_LIBRARY(cppjieba STATIC ${LIBCPPJIEBA_SRC})
ADD_EXECUTABLE(segment segment.cpp)
ADD_EXECUTABLE(server server.cpp)

LINK_DIRECTORIES(husky)

TARGET_LINK_LIBRARIES(segment cppjieba)
TARGET_LINK_LIBRARIES(server cppjieba husky pthread)

SET_TARGET_PROPERTIES(cppjieba PROPERTIES VERSION 1.2 SOVERSION 1)

INSTALL(TARGETS cppjieba ARCHIVE DESTINATION lib/CppJieba)
INSTALL(TARGETS segment RUNTIME DESTINATION bin/CppJieba)
INSTALL(TARGETS server RUNTIME DESTINATION bin/CppJieba)
INSTALL(FILES ChineseFilter.hpp HMMSegment.h MPSegment.h structs.h Trie.h globals.h ISegment.hpp  MixSegment.h  SegmentBase.hpp  TransCode.hpp  DESTINATION include/CppJieba)


ADD_SUBDIRECTORY(Husky)
ADD_SUBDIRECTORY(Limonp)
