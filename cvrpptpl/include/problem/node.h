#pragma once
#include <array>

struct Node {
  
  const int idx;
  std::array<float, 2> coord;
  
  Node(int index, float x, float y);
};