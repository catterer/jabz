#pragma once
#include <jansson.h>
#include <string>
#include <iostream>
#include <sstream>

namespace jabz {
class Error {
public:
  enum class Code: unsigned {
    OK = 0,
    InvalidJson       = 100,
    UnknownDirective  = 200,
  };

  Code          code{Code::OK};
  std::string   reason{};

  explicit operator bool() const {
    return code != Code::OK;
  }

  Error() {}

  Error(Code c, std::string&& r):
    code{c}, reason{r} {}

  Error(const json_error_t& e):
    code{Code::InvalidJson}
  {
    std::stringstream r{};
    r << "Invalid json: " << e.text << " in " << e.source << " line " << e.line;
    reason = r.str();
  }

  friend std::ostream& operator<<(std::ostream& out, const Error& e) {
    out << (unsigned)e.code << "[" << e.reason << "]";
    return out;
  }
};

Error pack(const std::string& in, std::string& out);
Error pack(FILE* f, std::string& out);
Error pack(const json_t* in, std::string& out);
}
