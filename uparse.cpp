#include "uparse.h"

namespace uparse {
  void amendLengths(Result& r, int length) {
    for (int& len : r) len += length;
  }
  void test(Parser& p, const char* str) {
    std::cout << str << ": ";
    Result r = p.parse(str);
    for (int len : r)
      std::cout << len << ' ';
    std::cout << '\n';
  }
  PPtr term(int c) {
    return std::make_unique<Term>(c);
  }
  PPtr operator+(PPtr a, PPtr b) {
    return std::make_unique<Sequence>(std::move(a), std::move(b));
  }
  PPtr operator|(PPtr a, PPtr b) {
    return std::make_unique<Alternate>(std::move(a), std::move(b));
  }
  PPtr many(PPtr a) {
    return std::make_unique<Many>(std::move(a));
  }
  PPtr succeed() {
    return std::make_unique<Succeed>();
  }
}