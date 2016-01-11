INCLUDE_DIRECTORIES(./ include)
ADD_LIBRARY(gtest STATIC src/gtest-all.cc)
TARGET_LINK_LIBRARIES(gtest pthread)
