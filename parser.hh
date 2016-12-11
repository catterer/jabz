#pragma once
#include <string.h>
#include <jansson.h>
#include <sstream>
#include <error.hh>

namespace jabz {

    class Stream: public std::stringstream {
        public:
            using pos_type = std::stringstream::pos_type;
            Stream(): std::stringstream() {}
    };

    class TypeConf {
        template<typename T>
            T reorder(T val) const {
                return val; // TODO
            }

        template<typename T>
            void pushPrimitive(T val, Stream& out) const {
                out.write(reinterpret_cast<const char*>(&val), sizeof(val));
            }
        public:
        TypeConf() {}
        void write(double             val, Stream& out) const { pushPrimitive(reorder(val), out); }
        void write(float              val, Stream& out) const { pushPrimitive(reorder(val), out); }
        void write(uint64_t           val, Stream& out) const { pushPrimitive(reorder(val), out); }
        void write(uint32_t           val, Stream& out) const { pushPrimitive(reorder(val), out); }
        void write(Stream::pos_type   val, Stream& out) const { pushPrimitive(reorder((uint32_t)val), out); }
        void write(const std::string& val, Stream& out) const { out.write(val.c_str(), val.size()); }
    };

    class Parser {
        public:
            Parser() {}
            virtual ~Parser() {}
            void xlate(const json_t* obj, Stream& out);
            void xlate(json_t* obj, Stream& out);


        private:
            TypeConf tcfg{};

            void   xlate_TODO(const json_t* obj, Stream& out) {
                throw Ex(Error::Code::UnsupportedType, "TODO");
            }
            template<typename T>
                void   xlate(const T& val, Stream& out) {
                    tcfg.write(val, out);
                }
            void   xlate_u64(json_t* obj, Stream& out);
            void   xlate_obj(const json_t* obj, Stream& out);
            void   xlate_arr(const json_t* obj, Stream& out);
            void   xlate_bool(bool val, Stream& out) {
                throw Ex(Error::Code::UnsupportedType, "how to convert bool?");
            }
    };
}
