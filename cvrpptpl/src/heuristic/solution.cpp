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
  double totalActualVehicleCharge = 0;
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
    totalActualVehicleCharge += actualVehicleCharge;
  }

  double totalActualMrtLineCharge = 0;
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
    totalActualMrtLineCharge += actualMrtLineCharge;
  }

  for (auto& customer : problem.customers) {
    int custIdx = customer.idx;
    if ((packageDestinations[custIdx] == custIdx) && (nodeVisitationCounts[custIdx] != 1)) {
      std::cout << "Customer set to be home-delivered but not visited " << custIdx << "\n";
      return false;
    }
    if ((packageDestinations[custIdx] != custIdx) && (nodeVisitationCounts[custIdx] > 0)) {
      std::cout << "Customer set to be self-pickup but is visited " << custIdx << "\n";
      return false;
    }
  }

  if (!isClose(totalCost, totalActualVehicleCharge + totalActualMrtLineCharge)) {
    std::cout << "Total cost computation is incorrect " << totalCost << " " << totalActualVehicleCharge + totalActualMrtLineCharge << "\n";
    return false;
  }

  
  //Added constraint check:
  // 1. check total demands == total vehicle loads
  // 2. check all nodes visited max once
  // 3. check depot only at the beginning or end of route
  // 4. locker load == all customer assigned to it
  // 5. correct computation of per vehicle charge
  // 6. correct computation of mrt line charge
  // 7. customers who chose to be home-delivery must be visited once.
  // 8. customers who chose to self-pickup must not be visited.
  // 9. total cost valid
  // TODO add more when you think of something..
  return true;
}



bool Solution::isValid(const Cvrpptpl& problem)
{
  //TODO: add this, this is a more relaxed verision of isFeasible, useful for checking operators.
  return true;
}
