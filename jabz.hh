#pragma once
#include <jansson.h>
#include <string>
#include <sstream>
#include <functional>
#include <error.hh>
#include <parser.hh>

namespace jabz {
    Error pack(const std::string& in, std::string& out, const ExtensionPack& ext={});
    Error pack(FILE* f, std::string& out, const ExtensionPack& ext={});
    Error pack(const json_t* in, std::string& out, const ExtensionPack& ext={});
}
