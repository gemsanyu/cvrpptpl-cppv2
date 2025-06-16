#pragma once

#include "problem/cvrpptpl.h"

#include <vector>

const int NO_DESTINATION = -1;
const int NO_VEHICLE = -2;

struct Solution {
public:
  std::vector<std::vector<int>> routes;
  std::vector<int> packageDestinations;
  std::vector<int> destinationTotalDemands;
  std::vector<int> vehicleLoads;
  std::vector<int> lockerLoads;
  std::vector<int> destinationVehicleAssignments;
  std::vector<int> mrtLoads;
  std::vector<bool> isNodeVisited;
  std::vector<bool> isMrtLineUsed;

  std::vector<double> vehicleCharges;
  std::vector<double> mrtCharges;
  double totalCost;

  Solution(const Cvrpptpl& problem);
  Solution(const Solution& other);
  bool isFeasible(const Cvrpptpl& problem);
  bool isValid(const Cvrpptpl& problem);
};
