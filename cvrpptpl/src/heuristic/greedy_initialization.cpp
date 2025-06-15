#include "heuristic/greedy_initialization.h"

#include <random>
#include <tuple>

#include "problem/cvrpptpl.h"
#include "heuristic/solution.h"
#include "heuristic/utils.h"

static bool tryLockerAssignment(int nodeIdx, int destIdx, const Cvrpptpl& problem, Solution& solution);
static bool tryHomeDeliveryAssignment(int nodeIdx, const Cvrpptpl& problem, Solution& solution);
static bool firstFitDestinationAssignment(int nodeIdx, const Cvrpptpl& problem, Solution& solution);
static bool greedyNodeInsertion(const Cvrpptpl& problem, Solution& solution);
static std::tuple<int, int, double> findBestInsertionPosition(int nodeIdx, const Cvrpptpl& problem, Solution& solution);

Solution greedyInitialization(const Cvrpptpl& problem) {
  Solution solution = Solution(problem);
  bool isAssignmentFound = firstFitDestinationAssignment(1, problem, solution);
  if (!isAssignmentFound) {
    throw std::runtime_error("no feasible locker assignment found");
  }
  bool isRouteFound = greedyNodeInsertion(problem, solution);
  if (!isRouteFound) {
    throw std::runtime_error("no feasible route found, suspected capacity problems");
  }
  return solution;
}

static std::tuple<int, int, double> findBestInsertionPosition(int nodeIdx, const Cvrpptpl& problem, Solution& solution) {
  double bestCost = INFINITY;
  int bestVehicleIdx = -1;
  int bestPosition = -1;
  int demand = solution.destinationTotalDemands[nodeIdx];
  auto& distanceMatrix = problem.distanceMatrix;
  for (int vi = 0; vi < problem.numVehicles; vi++) {
    if (solution.vehicleLoads[vi] + demand > problem.vehicleCapacities[vi]) {
      continue;
    }
    int routeLength = int(solution.routes[vi].size());
    for (int pos = 1; pos < routeLength; pos++) {
      // insertion at pos means,, insert between element
      // at pos-1 and pos -> route[pos-1] + nodeIdx + route[pos]
      int prevNodeIdx = solution.routes[vi][pos - 1];
      int nextNodeIdx = solution.routes[vi][pos];
      double cost = distanceMatrix[prevNodeIdx][nodeIdx] + distanceMatrix[nodeIdx][nextNodeIdx] - distanceMatrix[prevNodeIdx][nextNodeIdx];
      if (cost < bestCost) {
        bestCost = cost;
        bestVehicleIdx = vi;
        bestPosition = pos;
      }
    }
  }
  return std::make_tuple(bestVehicleIdx, bestPosition, bestCost);
}

static bool greedyNodeInsertion(const Cvrpptpl& problem, Solution& solution) {
  std::vector<int> nodeToVisitsIdxs;
  for (int i = 0; i < problem.numNodes; i++) {
    if (solution.destinationTotalDemands[i] > 0) {
      nodeToVisitsIdxs.push_back(i);
    }
  }
  auto rng = std::default_random_engine{42};
  std::shuffle(nodeToVisitsIdxs.begin(), nodeToVisitsIdxs.end(), rng);

  auto& distanceMatrix = problem.distanceMatrix;
  for (int nodeIdx : nodeToVisitsIdxs) {
    if (solution.isNodeVisited[nodeIdx]) {
      continue;
    }
    auto [bestVi, bestPos, bestCost] = findBestInsertionPosition(nodeIdx, problem, solution);
    if (problem.incomingMrtLineIdxs[nodeIdx] != NO_MRT_LINE) {
      // check if this is mrt station, what if we use mrt line,, is the cost better?
      int mrtLineIdx = problem.incomingMrtLineIdxs[nodeIdx];
      int startStationIdx = problem.mrtLines[mrtLineIdx].startStation.idx;
      int demand = solution.destinationTotalDemands[nodeIdx];
      bool isStartStationLockerCapSuffice = demand + solution.lockerLoads[startStationIdx] <= problem.lockerCapacities[startStationIdx];
      if (isStartStationLockerCapSuffice) {
        if (solution.isNodeVisited[startStationIdx]) {
          int vi = solution.destinationVehicleAssignments[startStationIdx];
          bool isVehicleCapSufficient = demand + solution.vehicleLoads[vi] <= problem.vehicleCapacities[vi];
          bool isMrtLineCapSufficient = demand + solution.mrtLoads[mrtLineIdx] <= problem.mrtLineCapacities[vi];
          double cost = demand * problem.mrtLineCosts[mrtLineIdx];
          if (cost < bestCost and isVehicleCapSufficient) {
            solution.destinationTotalDemands[startStationIdx] += demand;
            solution.lockerLoads[startStationIdx] += demand;
            solution.vehicleLoads[vi] += demand;
            solution.isMrtLineUsed[mrtLineIdx] = true;
            solution.mrtCharges[mrtLineIdx] += demand * problem.mrtLineCosts[mrtLineIdx];
            solution.mrtLoads[mrtLineIdx] += demand;
            solution.totalCost += demand * problem.mrtLineCosts[mrtLineIdx];
            solution.destinationTotalDemands[nodeIdx] = 0;
            continue;
          }
        }
        else {
          //temporarily move demand to startStation
          solution.destinationTotalDemands[startStationIdx] += demand;
          auto [bestVi2, bestPos2, bestCost2] = findBestInsertionPosition(startStationIdx, problem, solution);
          if (bestCost2 < bestCost) {
            solution.isMrtLineUsed[mrtLineIdx] = true;
            solution.mrtCharges[mrtLineIdx] += demand * problem.mrtLineCosts[mrtLineIdx];
            solution.mrtLoads[mrtLineIdx] += demand;
            solution.totalCost += demand * problem.mrtLineCosts[mrtLineIdx];
            solution.destinationTotalDemands[nodeIdx] = 0;
            insertNodeToRoute(startStationIdx, bestVi2, bestPos2, problem, solution);
            continue;
          }
          solution.destinationTotalDemands[startStationIdx] -= demand;
        }
      }
    }
    // insert
    if (bestVi == -1) {
      return false;
    }
    insertNodeToRoute(nodeIdx, bestVi, bestPos, problem, solution);
  }
  return true;
}

static bool firstFitDestinationAssignment(int nodeIdx, const Cvrpptpl& problem, Solution& solution) {
  if (nodeIdx > problem.numCustomers) {
    return true;
  }
  auto& destAlternatives = problem.destinationAlternatives[nodeIdx];
  int demand = problem.demands[nodeIdx];
  for (int destIdx : destAlternatives) {
    bool feasible;
    if (destIdx == nodeIdx) {
      feasible = tryHomeDeliveryAssignment(nodeIdx, problem, solution);
    }
    else {
      feasible = tryLockerAssignment(nodeIdx, destIdx, problem, solution);
    }
    if (feasible) {
      return true;
    }
  }
  return false;
}

static bool tryHomeDeliveryAssignment(int nodeIdx, const Cvrpptpl& problem, Solution& solution) {
  bool isFeasible = firstFitDestinationAssignment(nodeIdx + 1, problem, solution);
  if (isFeasible) {
    int demand = problem.demands[nodeIdx];
    solution.packageDestinations[nodeIdx] = nodeIdx;
    solution.destinationTotalDemands[nodeIdx] = demand;
    return true;
  }
  return false;
}


static bool tryLockerAssignment(int nodeIdx, int destIdx, const Cvrpptpl& problem, Solution& solution) {
  //if assign to locker
  int demand = problem.demands[nodeIdx];
  bool isOverloadLockerCap = solution.lockerLoads[destIdx] + demand > problem.lockerCapacities[destIdx];
  if (isOverloadLockerCap) {
    return false;
  }
  solution.lockerLoads[destIdx] += demand;
  bool isFeasible = firstFitDestinationAssignment(nodeIdx + 1, problem, solution);
  if (!isFeasible) {
    //revert changes
    solution.lockerLoads[destIdx] -= demand;
    return false;
  }
  //commit
  solution.packageDestinations[nodeIdx] = destIdx;
  solution.destinationTotalDemands[destIdx] += demand;
  return true;
}