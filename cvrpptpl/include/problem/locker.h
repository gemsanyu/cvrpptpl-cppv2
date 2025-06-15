#pragma once

#include "problem/node.h"

struct Locker : Node {
  int serviceTime;
  int capacity;
  Locker(int index, double x, double y, int serviceTime, int capacity);
};