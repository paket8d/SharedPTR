//#include <iostream>
//
//class A {
//public:
//  inline static int x = 0;
//  int y;
//
//  void f() {
//    ++y;
//    ++x;
//  }
//
//  static void g() {
//    ++x;
//  }
//};
//
//int main() {
//  A a, b;
//  a.x = 2; a.y = 0;
//  b.x = 1; b.y = 2;
//  a.f();
//  A::g();
//  std::cout << a.x << ' ' << a.y << ' ' << b.x << ' ' << b.y;
//}