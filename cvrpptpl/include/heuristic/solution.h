#pragma once

#include "problem/cvrpptpl.h"

#include <vector>

const int NO_DESTINATION = -1;

class Solution {
public:
  std::vector<std::vector<int>> routes;
  std::vector<int> packageDestinations;
  std::vector<int> destinationTotalDemands;
  std::vector<int> vehicleLoads;

  Solution(Cvrpptpl problem);
  Solution(Solution& other);
};