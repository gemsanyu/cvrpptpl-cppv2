#pragma once

#include "problem/cvrpptpl.h"

#include <vector>

const int NO_DESTINATION = -1;
const int NO_VEHICLE = -2;

class Solution {
public:
  std::vector<std::vector<int>> routes;
  std::vector<int> packageDestinations;
  std::vector<int> destinationTotalDemands;
  std::vector<int> vehicleLoads;
  std::vector<int> lockerLoads;
  std::vector<int> destinationVehicleAssignments;
  std::vector<int> mrtLoads;

  std::vector<float> lockerCharges;
  std::vector<float> vehicleCharges;
  std::vector<float> mrtCharges;
  float totalCost;

  Solution(const Cvrpptpl& problem);
  Solution(const Solution& other);
};