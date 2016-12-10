#pragma once
#include <string.h>
#include <jansson.h>
#include <sstream>

namespace jabz {

class Stream: public std::stringstream {
public:
  Stream(): std::stringstream() {}
};

class TypeConf {
  template<typename T>
  T reorder(T val) {
    return val; // TODO
  }

  template<typename T>
  void pushPrimitive(T val, Stream& out) {
    out.write(reinterpret_cast<const char*>(&val), sizeof(val));
  }
public:
  TypeConf() {}
  void write(double     val, Stream& out) { pushPrimitive(reorder(val), out); }
  void write(float      val, Stream& out) { pushPrimitive(reorder(val), out); }
  void write(uint64_t   val, Stream& out) { pushPrimitive(reorder(val), out); }
  void write(uint32_t   val, Stream& out) { pushPrimitive(reorder(val), out); }
  void write(const      std::string& s, Stream& out) { out.write(s.c_str(), s.size()); }
};

class Parser {
  TypeConf tcfg{};

  void   xlate_TODO(const json_t* obj, Stream& out) {
    throw Ex(Error::Code::UnsupportedType, "TODO");
  }
  template<typename T>
  void   xlate(const T& val, Stream& out) {
    tcfg.write(val, out);
  }
  void xlate_u64(json_t* obj, Stream& out) {
    if (json_typeof(obj) != JSON_INTEGER)
      throw Ex(Error::Code::TypeError, "Wrong type for u64: int required");
    xlate<uint64_t>(json_integer_value(obj), out);
  }
  void   xlate_obj(const json_t* obj, Stream& out) {
    const char* key_;
    json_t* val;
    json_object_foreach((json_t*)obj, key_, val) {
      const char* comment = strchr(key_, '#');
      auto key = std::string(key_, comment ? (comment - key_) : strlen(key_));

      if (key == "u64") xlate_u64(val, out);
      else
          throw Ex(Error::Code::UnknownDirective,
              std::string("Don't know how to process '") + std::string(key) + std::string("'"));
    }
  }
  void   xlate_bool(bool val, Stream& out) {
    throw Ex(Error::Code::UnsupportedType, "how to convert bool?");
  }
public:
  Parser() {}
  virtual ~Parser() {}

  void xlate(const json_t* obj, Stream& out) {
    switch (json_typeof(obj)) {
      case JSON_OBJECT:   return xlate_obj(obj, out);
      case JSON_ARRAY:    return xlate_TODO(obj, out);
      case JSON_STRING:   return xlate<std::string>({json_string_value(obj)}, out);
      case JSON_INTEGER:  return xlate<uint32_t>(json_integer_value(obj), out);
      case JSON_REAL:     return xlate<double>(json_real_value(obj), out);
      case JSON_TRUE:     return xlate_bool(true, out);
      case JSON_FALSE:    return xlate_bool(false, out);
      case JSON_NULL:     return;
    }
  }

};
}
