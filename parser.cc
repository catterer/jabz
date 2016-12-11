#include <parser.hh>

namespace jabz {

void Parser::xlate(const json_t* obj, Stream& out) {
  switch (json_typeof(obj)) {
    case JSON_OBJECT:   return xlate_obj(obj, out);
    case JSON_ARRAY:    return xlate_TODO(obj, out);
    case JSON_STRING:   return xlate<std::string>({json_string_value(obj)}, out);
    case JSON_INTEGER:  return xlate<uint32_t>(json_integer_value(obj), out);
    case JSON_REAL:     return xlate<float>((float)json_real_value(obj), out);
    case JSON_TRUE:     return xlate_bool(true, out);
    case JSON_FALSE:    return xlate_bool(false, out);
    case JSON_NULL:     return;
  }
}

void Parser::xlate_u64(json_t* obj, Stream& out) {
  if (json_typeof(obj) != JSON_INTEGER)
    throw Ex(Error::Code::TypeError, "Wrong type for u64: int required");
  xlate<uint64_t>(json_integer_value(obj), out);
}

void Parser::xlate_obj(const json_t* obj, Stream& out) {
  const char* key_;
  json_t* val;
  json_object_foreach((json_t*)obj, key_, val) {
    const char* comment = strchr(key_, '#');
    auto key = std::string(key_, comment ? (comment - key_) : strlen(key_));

    if (key == "u64")   xlate_u64(val, out);
    else
        throw Ex(Error::Code::UnknownDirective,
            std::string("Don't know how to process '") + std::string(key) + std::string("'"));
  }
}
}
