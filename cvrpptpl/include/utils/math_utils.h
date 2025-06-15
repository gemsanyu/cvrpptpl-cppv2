#pragma once

#include <cmath>

bool isClose(double a, double b, double epsilon = 1e-9) {
  return std::fabs(a - b) < epsilon;
}