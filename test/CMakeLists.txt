SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

ADD_EXECUTABLE(segment.demo segment_demo.cpp)
ADD_EXECUTABLE(keyword.demo keyword_demo.cpp)
ADD_EXECUTABLE(tagging.demo tagging_demo.cpp)
ADD_EXECUTABLE(load_test load_test.cpp)
ADD_SUBDIRECTORY(unittest)
