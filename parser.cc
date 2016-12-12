#include <functional>
#include <parser.hh>

namespace jabz {

    template<typename T>
    static void ext_num(Parser& p, const json_t* obj, Stream& out) {
        if (json_typeof(obj) != JSON_INTEGER)
            throw Ex(Error::Code::TypeError, "Wrong type for u64: int required");
        p.xlate<T>((T)json_integer_value(obj), out);
    }

    const ExtensionPack std {
        {"u64", ext_num<uint64_t>},
        {"u16", ext_num<uint16_t>},
        {"u8", ext_num<uint8_t>}
    };

    class Lps {
        const TypeConf tcfg{};
        Stream& s;
        std::iostream::pos_type start{};
        std::iostream::pos_type data_off{};
        public:
        Lps(const TypeConf& tcfg_, Stream& s_): tcfg{tcfg_}, s{s_} {}
        Lps(const Lps& other) = delete;
        void open() {
            start = s.tellp();
            tcfg.write(Stream::pos_type(0), s);
            data_off = s.tellp();
        }
        void close() {
            const auto end = s.tellp();
            s.seekp(start);
            tcfg.write(Stream::pos_type(end - data_off), s);
            s.seekp(end);
        }

        using WrapCb = std::function<void(Stream& s)>;
        void wrap(WrapCb cb) {
            open();
            cb(s);
            close();
        }
    };

    void Parser::xlate(json_t* obj, Stream& out) {
        xlate((const json_t*) obj, out);
    }

    void Parser::xlate(const json_t* obj, Stream& out) {
        switch (json_typeof(obj)) {
            case JSON_OBJECT:   return xlate_obj(obj, out);
            case JSON_ARRAY:    return xlate_arr(obj, out);
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
            if (!key.size()) {
                xlate(obj, out);
                continue;
            }

            auto exti = ext.find(key);
            if (exti == ext.end())
                throw Ex(Error::Code::UnknownDirective,
                        std::string("Don't know how to process '") + std::string(key) + std::string("'"));
            exti->second(*this, val, out);
        }
    }

    void Parser::xlate_arr(const json_t* a, Stream& out) {
        Lps(tcfg, out).wrap(
                [this, &a] (Stream& s) {
                    size_t i;
                    json_t* e;
                    json_array_foreach((json_t*)a, i, e)
                        xlate(e, s);
                });
    }
}
