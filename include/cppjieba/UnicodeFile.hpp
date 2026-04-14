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
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > converter;
    ifs.open(converter.from_bytes(path).c_str());
    return;
  } catch (const std::range_error&) {
  }
#endif
  ifs.open(path.c_str());
}

} // namespace cppjieba

#endif
