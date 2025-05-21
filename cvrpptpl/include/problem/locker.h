#pragma once

#include "problem/node.h"

struct Locker : Node {
  int serviceTime;
  int capacity;
  float cost;
  Locker(int index, float x, float y, int serviceTime, int capacity, float cost);
};