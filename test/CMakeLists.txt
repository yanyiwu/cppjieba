SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

ENABLE_TESTING()
ADD_EXECUTABLE(load_test load_test.cpp)
ADD_SUBDIRECTORY(unittest)
ADD_TEST(NAME ./test/test.run COMMAND ./test/test.run)
ADD_TEST(NAME ./load_test COMMAND ./load_test)
