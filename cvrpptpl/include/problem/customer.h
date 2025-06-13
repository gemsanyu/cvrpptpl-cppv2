#pragma once

#include "node.h"
#include <vector>

struct Customer : Node {
  int serviceTime;
  int demand;
  bool isHomeDelivery;
  bool isSelfPickup;
  bool isFlexible;
  std::vector<int> lockerPreferenceIdxs;
  Customer(int index, float x, float y, int serviceTime, int demand, bool isSelfPickup, bool isFlexible, const std::vector<int> & lockerPreferenceIdxs);
};