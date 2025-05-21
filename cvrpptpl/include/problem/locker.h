#pragma once

#include "problem/node.h"

class Locker : public Node {
  int serviceTime;
  int capacity;
  float cost;

public:
  Locker(int index, float x, float y, int serviceTime, int capacity, float cost);
};