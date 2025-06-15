#include "problem/locker.h"
#include "problem/node.h"

Locker::Locker(int index, double x, double y,
  int serviceTime, int capacity)
  : Node(index, x, y),
  serviceTime(serviceTime),
  capacity(capacity) { }
