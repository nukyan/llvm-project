// RUN: %clang_cc1 -std=c++17 -fherbception -fsyntax-only -verify %s
// RUN: %clang_cc1 -std=c++20 -fherbception -fsyntax-only -verify %s

#include <stderror>

// Basic throws declaration
void f1() throws;
int f2() throws;

// Conditional throws with except_t values
void f3() throws(std::except_t::no_except);     // same as noexcept
void f4() throws(std::except_t::static_except);  // same as throws
void f5() throws(std::except_t::dynamic_except); // may throw dynamically

// noexcept queries on throws functions
static_assert(!noexcept(f1()), "throws functions are not noexcept");
static_assert(noexcept(f3()), "throws(no_except) is noexcept");

// Virtual overrides
struct Base {
  virtual void vf() throws;
  virtual ~Base() = default;
};

struct Derived : Base {
  void vf() throws override;
};

// expected-no-diagnostics
