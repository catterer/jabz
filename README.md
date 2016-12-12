# jabz

Describe binary data using JSON format.
Use extensions interface to provide new data types.

###Basic syntax

[ ] stands for LPS with 32-bit length.

Json integer translated to 32-bit.

Json float translated to float.

###Examples

In:
```JSON
[
    ["reqid_string_100002"],
    [
        1231,
        122
    ],
    3, ["value1"],
    4, ["value2"],
    {"u64" : 123123123}
]
```

Out:
```
00000000  47 00 00 00 13 00 00 00  72 65 71 69 64 5f 73 74  |G.......reqid_st|
00000010  72 69 6e 67 5f 31 30 30  30 30 32 08 00 00 00 cf  |ring_100002.....|
00000020  04 00 00 7a 00 00 00 03  00 00 00 06 00 00 00 76  |...z...........v|
00000030  61 6c 75 65 31 04 00 00  00 06 00 00 00 76 61 6c  |alue1........val|
00000040  75 65 32 b3 b5 56 07 00  00 00 00                 |ue2..V.....|
```

### C++ interface
```C++
namespace jabz {
    using ExtCb = std::function<void(Parser& p, const json_t* obj, Stream& out)>;
    using ExtensionPack = std::unordered_map<std::string, ExtCb>;

    Error pack(const std::string& in, std::string& out, const ExtensionPack& ext={});
    Error pack(FILE* f, std::string& out, const ExtensionPack& ext={});
    Error pack(const json_t* in, std::string& out, const ExtensionPack& ext={});
}
```
