#include "heuristic/utils.h"

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