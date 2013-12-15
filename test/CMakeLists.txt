SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)

ADD_EXECUTABLE(segment.demo segment.cpp)
ADD_EXECUTABLE(test_performance test_performance.cpp)
ADD_SUBDIRECTORY(unittest)
