#include <jabz.hh>

int main() {
  std::string res{};
  auto e = jabz::pack(stdin, res);
  if (e)
    std::cerr << e << std::endl;
  else
    std::cout << res << std::endl;
  return 0;
}
