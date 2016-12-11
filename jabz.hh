#pragma once
#include <jansson.h>
#include <string>
#include <iostream>
#include <sstream>
#include <error.hh>

namespace jabz {
Error pack(const std::string& in, std::string& out);
Error pack(FILE* f, std::string& out);
Error pack(const json_t* in, std::string& out);
}
