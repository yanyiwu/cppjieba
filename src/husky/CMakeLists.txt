SET(LIBHUSKY_SRC Daemon.cpp ServerFrame.cpp)
INCLUDE_DIRECTORIES(../limonp)
ADD_LIBRARY(husky SHARED ${LIBHUSKY_SRC})

INSTALL(TARGETS husky LIBRARY DESTINATION lib/CppJieba)
INSTALL(FILES Daemon.h  globals.h  HttpReqInfo.hpp  ServerFrame.h  ThreadManager.hpp DESTINATION include/CppJieba/Husky)
