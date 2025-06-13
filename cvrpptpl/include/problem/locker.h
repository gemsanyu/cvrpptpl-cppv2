#pragma once

#include "problem/node.h"

struct Locker : Node {
  int serviceTime;
  int capacity;
  Locker(int index, float x, float y, int serviceTime, int capacity);
};