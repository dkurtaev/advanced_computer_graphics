#ifndef INCLUDE_FPS_HPP_
#define INCLUDE_FPS_HPP_

#include <sys/time.h>

class FPS {
 public:
  static void Stack();

  static int Get();

  static int TimeFrom(const timeval& t);

 private:
  static const int kDepth = 5000;
};

#endif  // INCLUDE_FPS_HPP_
