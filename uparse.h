#ifndef UPARSE_BLAH
#define UPARSE_BLAH

#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <memory>

namespace uparse {
  class Parser;
  using Result = std::list<int>;
  using PPtr = std::unique_ptr<Parser>;
  class Parser {
  public:
    virtual Result parse(const char* s) = 0;
    virtual ~Parser() {};
  };
  class Succeed : public Parser {
  public:
    Result parse(const char* s) override {
      return { 0 };
    }
  };
  class Fail : public Parser {
  public:
    Result parse(const char* s) override {
      return {};
    }
  };
  class Term : public Parser {
  public:
    Term(int c) : c(c) {}
    Result parse(const char* s) override {
      return s[0] == c ? std::list<int>{ 1 } : std::list<int>{};
    }
    int c;
  };
  class Alternate : public Parser {
  public:
    Alternate(std::unique_ptr<Parser> a, std::unique_ptr<Parser> b) :
        a(std::move(a)), b(std::move(b)) {}
    Result parse(const char* s) override {
      Result res1 = a->parse(s);
      Result res2 = b->parse(s);
      res1.splice(res1.end(), res2);
      return res1;
    }
    std::unique_ptr<Parser> a;
    std::unique_ptr<Parser> b;
  };
  void amendLengths(Result& r, int length);
  class Sequence : public Parser {
  public:
    Sequence(std::unique_ptr<Parser> a, std::unique_ptr<Parser> b) :
        a(std::move(a)), b(std::move(b)) {}
    Result parse(const char* s) override {
      Result res = a->parse(s);
      return parseNext(res, s);
    }
    std::unique_ptr<Parser> a;
    std::unique_ptr<Parser> b;
  private:
    Result parseNext(Result& r, const char* s) {
      Result result;
      for (int len : r) {
        Result res = b->parse(s + len);
        amendLengths(res, len);
        result.splice(result.end(), res); // append res to result
      }
      return result;
    }
  };
  class Many : public Parser {
  public:
    Many(std::unique_ptr<Parser> a) :
        a(std::move(a)) {}
    Result parse(const char* s) override {
      Result res = { 0 };
      Result result;
      while (!res.empty()) {
        result.insert(result.end(), res.begin(), res.end());
        res = parseNext(res, s);
      }
      return result;
    }
    std::unique_ptr<Parser> a;
  private:
    Result parseNext(Result& r, const char* s) {
      Result result;
      for (int len : r) {
        Result res = a->parse(s + len);
        amendLengths(res, len);
        result.splice(result.end(), res); // append res to result
      }
      return result;
    }
  };
  void test(Parser& p, const char* str);
  inline void test(PPtr& p, const char* str) {
    test(*p, str);
  }
  // Convenience functions
  PPtr term(int c);
  PPtr operator+(PPtr a, PPtr b);
  PPtr operator|(PPtr a, PPtr b);
  PPtr many(PPtr a);
  PPtr succeed();
}

#endif