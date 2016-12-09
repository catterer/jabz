#pragma once
#include <jansson.h>
#include <sstream>

namespace jabz {

class Stream: public std::stringstream {
public:
  Stream(): std::stringstream() {}
};

namespace Inpn {
  enum class Real {
    Float,
    Double
  };

  enum class Int {
    U32,
    U64
  };
}

class TypeConf {
  Inpn::Real   inpn_real{Inpn::Real::Float};
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
  void write(double val, Stream& out) {
    switch (inpn_real) {
    case Inpn::Real::Float:   pushPrimitive(reorder((float)val), out);
    case Inpn::Real::Double:  pushPrimitive(reorder((double)val), out);
    }
  }
};

class Parser {
  TypeConf tcfg{};

  virtual void   xlate_TODO(const json_t* obj, Stream& out) {
    throw Error(Error::Code::UnknownDirective, "TODO");
  }
  virtual void   xlate_real(double val, Stream& out) {
    tcfg.write(val, out);
  }
  virtual void   xlate_bool(bool val, Stream& out) {
    throw Error(Error::Code::UnknownDirective, "how to convert bool?");
  }
  virtual void   xlate_null(Stream& out) {
    throw Error(Error::Code::UnknownDirective, "how to convert NULL?");
  }
public:
  Parser() {}
  virtual ~Parser() {}

  void xlate(const json_t* obj, Stream& out) {
    switch (json_typeof(obj)) {
      case JSON_OBJECT:   return xlate_TODO(obj, out);
      case JSON_ARRAY:    return xlate_TODO(obj, out);
      case JSON_STRING:   return xlate_TODO(obj, out);
      case JSON_INTEGER:  return xlate_TODO(obj, out);
      case JSON_REAL:     return xlate_real(json_real_value(obj), out);
      case JSON_TRUE:     return xlate_bool(true, out);
      case JSON_FALSE:    return xlate_bool(false, out);
      case JSON_NULL:     return xlate_null(out);
    }
  }

};
}
