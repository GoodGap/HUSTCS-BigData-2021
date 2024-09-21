/*
 * Below is the given application for lab1_challenge1_backtrace.
 * This app prints all functions before calling print_backtrace().
 */

#include "user_lib.h"
#include "util/types.h"

// void f8() { print_backtrace(100); }
// void f7() { f8(); }
// int f6(int a, int b) { f7(); return a + b;}
// void f5() { f6(1, 2); }
// void f4() { f5(); }
// void f3() { f4(); }
// void f2() { f3(); }
// uint64 f1(uint64 a) {f2(); return a;}

void f8() { print_backtrace(7); }
void f7() { f8(); }
void f6() { f7(); }
void f5() { f6(); }
void f4() { f5(); }
void f3() { f4(); }
void f2() { f3(); }
void f1() { f2(); }

int main()
{
  printu("back trace the user app in the following:\n");
  f1();
  exit(0);
  return 0;
}
