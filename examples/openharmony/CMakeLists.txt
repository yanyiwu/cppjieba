set(CPPJIEBA_OPENHARMONY_DICT_DIR "${PROJECT_SOURCE_DIR}/dict"
    CACHE PATH "Directory containing cppjieba dictionary assets for the OpenHarmony demo")

string(REPLACE "\\" "\\\\" CPPJIEBA_OPENHARMONY_DICT_DIR_ESCAPED "${CPPJIEBA_OPENHARMONY_DICT_DIR}")

add_executable(cppjieba_openharmony_demo openharmony_demo.cpp)
target_link_libraries(cppjieba_openharmony_demo PRIVATE cppjieba)
target_compile_definitions(cppjieba_openharmony_demo PRIVATE
  CPPJIEBA_OPENHARMONY_DICT_DIR="${CPPJIEBA_OPENHARMONY_DICT_DIR_ESCAPED}")

add_test(NAME ./cppjieba_openharmony_demo COMMAND ./cppjieba_openharmony_demo)
