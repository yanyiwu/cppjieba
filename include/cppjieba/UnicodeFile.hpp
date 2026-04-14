#ifndef CPPJIEBA_UNICODE_FILE_HPP
#define CPPJIEBA_UNICODE_FILE_HPP

#include <fstream>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <codecvt>
#include <locale>
#endif

namespace cppjieba {

inline void OpenInputFile(std::ifstream& ifs, const std::string& path) {
#ifdef _WIN32
  try {
    // Keep using wstring_convert here for C++11 compatibility.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > converter;
    ifs.open(converter.from_bytes(path).c_str());
    return;
  } catch (const std::range_error&) {
    // Fall back to the existing narrow-path behavior for non-UTF-8 inputs.
  }
#endif
  ifs.open(path.c_str());
}

} // namespace cppjieba

#endif
