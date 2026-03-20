// RUN: %clang_cc1 -std=c++17 -fherbception -fsyntax-only -verify %s

// Basic throws specification
void f1() throws;

// Throws with return type
int f2() throws;

// Throws on lambda
auto l1 = []() throws {};

// Throws does not conflict with const/volatile qualifiers
struct S {
  void m1() throws;
  void m2() const throws;
};

// Throws cannot be combined with throw()
void f_bad1() throw() throws; // expected-error {{cannot have both}}

// Throws cannot be combined with noexcept
void f_bad2() noexcept throws; // expected-error {{cannot have both}}

// Basic usage compiles
void test_basic() {
  f1();
  int x = f2();
}
