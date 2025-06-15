#pragma once
#include <array>

struct Node {
  
  const int idx;
  std::array<double, 2> coord;
  
  Node(int index, double x, double y);
};