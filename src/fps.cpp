#include "include/fps.hpp"

#include <queue>

std::queue<timeval> times_;

void FPS::Stack() {
  timeval t;
  gettimeofday(&t, 0);
  times_.push(t);
}

int FPS::Get() {
  while (TimeFrom(times_.front()) > kDepth) {
    times_.pop();
  }
  return (int)(times_.size() * 1000.0f / kDepth);
}

int FPS::TimeFrom(const timeval& t) {
  timeval now;
  gettimeofday(&now, 0);
  return (now.tv_sec - t.tv_sec) * 1e+3 + (now.tv_usec - t.tv_usec) * 1e-3;
}
