// RUN: %clang_cc1 -std=c++17 -fherbception -emit-llvm -o - %s | FileCheck %s

#include <stderror>

// A simple throws function should be nounwind.
// CHECK: define {{.*}} @_Z2f1v() #[[ATTR:[0-9]+]]
void f1() throws {
}

// A throws function returning int.
// CHECK: define {{.*}} @_Z2f2v() #[[ATTR]]
int f2() throws {
  return 42;
}

// CHECK: attributes #[[ATTR]] = {{{.*}}nounwind{{.*}}}
