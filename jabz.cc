#include <jabz.hh>
#include <parser.hh>

namespace jabz {

    constexpr auto json_flags = JSON_REJECT_DUPLICATES;

    Error pack(const std::string& in, std::string& out) {
        json_error_t e;
        const json_t* j = json_loads(in.c_str(), json_flags, &e);
        if (!j)
            return {e};
        return pack(j, out);
    }

    Error pack(FILE* f, std::string& out) {
        json_error_t e;
        const json_t* j = json_loadf(f, json_flags, &e);
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
