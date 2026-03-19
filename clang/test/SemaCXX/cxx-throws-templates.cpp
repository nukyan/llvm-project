// RUN: %clang_cc1 -std=c++17 -fstatic-exceptions -fsyntax-only -verify %s

#include <stderror>

// Template function with throws
template<typename T>
T create() throws;

// Conditional throws in templates
template<typename T>
void process(T val) throws(std::except_t::static_except);

// Instantiation
void test() {
  int x = create<int>();
  process(42);
}

// expected-no-diagnostics
