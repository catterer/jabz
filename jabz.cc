#include <jabz.hh>

namespace jabz {
class Ex: public std::runtime_error {
public:
  Error err{};
  Ex(Error&& e): std::runtime_error(e.reason.c_str()), err{e} {}
};

Error pack(const std::string& in) {
  json_error_t e;
  json_t* j = json_loads(in.c_str(), JSON_REJECT_DUPLICATES, &e);
  if (!j)
    return {e};
  return {};
}

Error pack(json_t* obj) {
  try {
  } catch(Ex& ex) {
    return ex.err;
  }
  return {};
}
}
