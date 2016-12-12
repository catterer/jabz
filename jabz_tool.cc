#include <iostream>
#include <jabz.hh>

int main() {
    std::string res{};
    jabz::ExtensionPack foo_ext {
        {"foo",
            [](jabz::Parser& p, const json_t* obj, jabz::Stream& out) {
                out << "eeeehaaaaaa";
            }}
    };
    auto e = jabz::pack(stdin, res, foo_ext);
    if (e)
        std::cerr << e << std::endl;
    else
        std::cout << res;
    return 0;
}
