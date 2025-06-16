#include "heuristic/destroy_operator_l1.h"

#include <algorithm>
#include <random>
#include <vector>

#include "heuristic/operator.h"
#include "heuristic/utils.h"
#include "problem/cvrpptpl.h"

DestroyOperatorL1::DestroyOperatorL1(int minNodesTR, int maxNodesToRemove, int seed)
  : minNodesToRemove(minNodesTR), maxNodesToRemove(maxNodesToRemove), rng(seed), dist(minNodesTR, maxNodesToRemove){}

OperatorStatus DestroyOperatorL1::apply(const Cvrpptpl& problem, Solution& solution) {
  std::vector<int> nodeIdxsToRemove = findNodesToRemove(problem, solution);
  for (int nodeIdx : nodeIdxsToRemove) {
    removeNodeFromRoute(nodeIdx, problem, solution);
  }
  return OperatorStatus::SUCCESS;
}

std::vector<int> RandomDestinationRemoval::findNodesToRemove(const Cvrpptpl& problem, Solution& solution)
{
  std::vector<int> nodeIdxsInRoutes;
  for (int nodeIdx = 1; nodeIdx < problem.numNodes; nodeIdx++) {
    if (solution.destinationVehicleAssignments[nodeIdx] != NO_VEHICLE) {
      nodeIdxsInRoutes.push_back(nodeIdx);
    }
  }
  int numNodesToRemove = dist(rng);
  numNodesToRemove = std::min(numNodesToRemove, int(nodeIdxsInRoutes.size()));
  std::vector<int> nodeIdxsToRemove;
  std::sample(nodeIdxsInRoutes.begin(), nodeIdxsInRoutes.end(), std::back_inserter(nodeIdxsToRemove), numNodesToRemove, rng);
  return nodeIdxsToRemove;
}
