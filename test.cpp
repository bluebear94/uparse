#include <memory>

#include "uparse.h"

using uparse::term;
using uparse::many;
using uparse::PPtr;

int main() {
  PPtr p = term('h') + term('e') + term('l') + term('l') + term('o');
  test(p, "hello");
  test(p, "hell");
  test(p, "hello w");
  PPtr p2 = (term('H') | term('h')) + term('e') + term('l') + term('l') + term('o');
  test(p2, "Hello");
  test(p2, "hell");
  test(p2, "hello w");
  PPtr p3 = many(term('x'));
  test(p3, "xxx");
  test(p3, "xxxxx");
  test(p3, "xyzzy");
}