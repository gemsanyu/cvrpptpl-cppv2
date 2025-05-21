#pragma once

#include "node.h"
#include <vector>

class Customer : public Node {
  int serviceTime;
  int demand;
  bool isSelfPickup;
  bool isFlexible;
  std::vector<int> lockerPreferenceIdxs;

public:
  Customer(int index, float x, float y, int serviceTime, int demand, bool isSelfPickup, bool isFlexible, const std::vector<int> & lockerPreferenceIdxs);
};