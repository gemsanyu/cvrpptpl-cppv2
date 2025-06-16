#include "heuristic/utils.h"

#include <algorithm>

void insertNodeToRoute(int nodeIdx, int vi, int pos, const Cvrpptpl& problem, Solution& solution) {
  auto& distanceMatrix = problem.distanceMatrix;
  int demand = solution.destinationTotalDemands[nodeIdx];
  solution.vehicleLoads[vi] += demand;
  solution.destinationVehicleAssignments[nodeIdx] = vi;
  int prevNodeIdx = solution.routes[vi][pos - 1];
  int nextNodeIdx = solution.routes[vi][pos];
  double oldDistance = distanceMatrix[prevNodeIdx][nextNodeIdx];
  double newDistance = distanceMatrix[prevNodeIdx][nodeIdx] + distanceMatrix[nodeIdx][nextNodeIdx];
  double dCost = (newDistance - oldDistance) * problem.vehicleCosts[vi];
  solution.totalCost += dCost;
  solution.vehicleCharges[vi] += dCost;
  solution.routes[vi].insert(solution.routes[vi].begin() + pos, nodeIdx);
  solution.isNodeVisited[nodeIdx] = true;
}

void removeNodeFromRoute(int nodeIdx, const Cvrpptpl& problem, Solution& solution) {
  int vehicleIdx = solution.destinationVehicleAssignments[nodeIdx];
  if (vehicleIdx == NO_VEHICLE) {
    throw std::runtime_error("A node not assigned to any vehicle is in a route");
  }
  auto it = std::find(solution.routes[vehicleIdx].begin(), solution.routes[vehicleIdx].end(), nodeIdx);
  if (it == solution.routes[vehicleIdx].end()) {
    throw std::runtime_error("Node is assigned to vehicle but is not in its route");
  }
  int pos = int(std::distance(solution.routes[vehicleIdx].begin(), it));

  auto& distanceMatrix = problem.distanceMatrix;
  //revise vehicle charge
  int prevNodeIdx = solution.routes[vehicleIdx][pos - 1];
  int nextNodeIdx = solution.routes[vehicleIdx][pos + 1];
  double oldDist = distanceMatrix[prevNodeIdx][nodeIdx] + distanceMatrix[nodeIdx][nextNodeIdx];
  double newDist = distanceMatrix[prevNodeIdx][nextNodeIdx];
  double dCharge = (newDist - oldDist) * problem.vehicleCosts[vehicleIdx];
  solution.vehicleCharges[vehicleIdx] += dCharge;
  solution.totalCost += dCharge;

  //revise vehicle assigment
  solution.destinationVehicleAssignments[nodeIdx] = NO_VEHICLE;
  solution.routes[vehicleIdx].erase(it);
}