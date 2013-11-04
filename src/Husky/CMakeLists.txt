
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(LIBHUSKY_SRC Daemon.cpp ServerFrame.cpp)
INCLUDE_DIRECTORIES(../limonp)
ADD_LIBRARY(husky STATIC ${LIBHUSKY_SRC})

INSTALL(TARGETS husky ARCHIVE DESTINATION lib/CppJieba)
INSTALL(FILES Daemon.h  globals.h  HttpReqInfo.hpp  ServerFrame.h  ThreadManager.hpp DESTINATION include/CppJieba/Husky)
