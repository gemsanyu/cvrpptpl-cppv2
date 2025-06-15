#include "heuristic/solution.h"

#include<iostream>

#include "problem/cvrpptpl.h"
#include "utils/math_utils.h"

Solution::Solution(const Cvrpptpl& problem) {
  routes.resize(problem.numVehicles, {0,0});
  vehicleLoads.resize(problem.numVehicles, 0);
  vehicleCharges.resize(problem.numVehicles, 0);
  packageDestinations.resize(problem.numNodes, NO_DESTINATION);
  destinationVehicleAssignments.resize(problem.numNodes, NO_VEHICLE);
  destinationTotalDemands.resize(problem.numNodes, 0);
  lockerLoads.resize(problem.numNodes, 0);
  mrtLoads.resize(problem.numMrtLines, 0);
  mrtCharges.resize(problem.numMrtLines, 0);
  isNodeVisited.resize(problem.numNodes, false);
  isMrtLineUsed.resize(problem.numMrtLines, false);
  totalCost = 0;
}

Solution::Solution(const Solution& other): routes(other.routes),
packageDestinations(other.packageDestinations),
destinationTotalDemands(other.destinationTotalDemands),
vehicleLoads(other.vehicleLoads),
lockerLoads(other.lockerLoads),
destinationVehicleAssignments(other.destinationVehicleAssignments),
mrtLoads(other.mrtLoads),
vehicleCharges(other.vehicleCharges),
mrtCharges(other.mrtCharges),
isNodeVisited(other.isNodeVisited),
isMrtLineUsed(other.isMrtLineUsed),
totalCost(other.totalCost) {}

bool Solution::isFeasible(const Cvrpptpl& problem)
{
  int totalDemands = 0;
  for (auto& customer : problem.customers) {
    totalDemands += customer.demand;
  }
  int totalVehicleLoads = 0;
  for (int vi = 0; vi < problem.numVehicles; vi++) {
    totalVehicleLoads += vehicleLoads[vi];
  }
  std::vector<int> nodeVisitationCounts(problem.numNodes, 0);
  for (int vi = 0; vi < routes.size(); vi++) {
    auto& route = routes[vi];
    int actualVehicleLoad = 0;
    for (auto i = 0; i < route.size(); i++) {
      int nodeIdx = route[i];
      actualVehicleLoad += destinationTotalDemands[nodeIdx];
      if (nodeIdx == 0) {
        if (!((i == 0) || (i == route.size() - 1))) {
          return false;
        }
      }
      if (nodeIdx != 0) {
        nodeVisitationCounts[nodeIdx]++;
      }
    }
    if (actualVehicleLoad != vehicleLoads[vi]) {
      std::cout << "Vehicle's load does not match its route " << vi <<" " << vehicleLoads[vi] << " " << actualVehicleLoad << "\n";
      return false;
    }
  }
  for (int nodeIdx = 1; nodeIdx < problem.numNodes; nodeIdx++) {
    if (nodeVisitationCounts[nodeIdx] > 1) {
      std::cout << "A node is visited more than once " << nodeIdx << " " << nodeVisitationCounts[nodeIdx] << "\n";
      return false;
    }
  }
  
  if (totalDemands != totalVehicleLoads) {
    std::cout << "total demands does not match with total vehicle loads" << " " << totalDemands << " " << totalVehicleLoads << "\n";
    return false;
  }

  for (int i = 0; i < problem.numNodes; i++) {
    if (!problem.isLocker[i]) continue;
    int actualLockerLoad = 0;
    for (auto& customer : problem.customers) {
      int custIdx = customer.idx;
      int demand = customer.demand;
      if (packageDestinations[custIdx] == i) {
        actualLockerLoad += demand;
      }
    }
    if (actualLockerLoad != lockerLoads[i]) {
      std::cout << "total locker load is wrong " << i << lockerLoads[i] << " " << actualLockerLoad << "\n";
      return false;
    }
  }

  auto& distanceMatrix = problem.distanceMatrix;
  for (int vi = 0; vi < routes.size(); vi++) {
    auto& route = routes[vi];
    double actualVehicleCharge = 0;
    for (auto i = 0; i < route.size() - 1; i++) {
      double charge = distanceMatrix[route[i]][route[i + 1]] * problem.vehicleCosts[vi];
      actualVehicleCharge += charge;
    }
    if (!isClose(actualVehicleCharge, vehicleCharges[vi])) {
      std::cout << "Vehicle charge computation is incorrect " << vi << " " << vehicleCharges[vi] << " " << actualVehicleCharge << " " << vehicleCharges[vi] - actualVehicleCharge<<"\n";
      return false;
    }
  }

  for (int mi = 0; mi < problem.numMrtLines; mi++) {
    double actualMrtLineCharge = 0;
    if (isMrtLineUsed[mi]) {
      auto& mrtLine = problem.mrtLines[mi];
      int endStationIdx = mrtLine.endStation.idx;
      actualMrtLineCharge = lockerLoads[endStationIdx] * mrtLine.cost;
      if (nodeVisitationCounts[endStationIdx]) {
        std::cout << "End station of used mrt line is in the routes" << mi << " " << endStationIdx << "\n";
        return false;
      }
    }
    if (!isClose(actualMrtLineCharge, mrtCharges[mi])) {
      std::cout << "Incorrect computation of mrt line charge " << mi << " " << mi << " " << mrtCharges[mi] << " " << actualMrtLineCharge << "\n";
      return false;
    }
  }
  
  //TODO: continue adding constraints to check if a solution is feasible.
  return true;
}
