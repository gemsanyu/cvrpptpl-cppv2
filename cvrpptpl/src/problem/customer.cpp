#include "problem/customer.h"
#include "problem/node.h"

Customer::Customer(int index, double x, double y, int serviceTime, int demand, 
                    bool isSelfPickup, bool isFlexible, const std::vector<int> & lockerPreferenceIdxs)
  : Node(index, x, y), 
  serviceTime(serviceTime), 
  demand(demand), 
  isSelfPickup(isSelfPickup), 
  isFlexible(isFlexible), 
  lockerPreferenceIdxs(lockerPreferenceIdxs) {
    isHomeDelivery = !(isFlexible || isSelfPickup);
}