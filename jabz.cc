#include <jabz.hh>
#include <parser.hh>

namespace jabz {
class Ex: public std::runtime_error {
public:
  Error err{};
  Ex(Error&& e): std::runtime_error(e.reason.c_str()), err{e} {}
};

Error pack(const std::string& in, std::string& out) {
  json_error_t e;
  const json_t* j = json_loads(in.c_str(), JSON_REJECT_DUPLICATES, &e);
  if (!j)
    return {e};
  return pack(j, out);
}

Error pack(const json_t* obj, std::string& out) {
  try {
    Stream js{};
    Parser().xlate(obj, js);
    out = js.str();
  } catch(Ex& ex) {
    return ex.err;
  }
  return {};
}
}
