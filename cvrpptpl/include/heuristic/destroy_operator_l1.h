#pragma once

#include <random>

#include "heuristic/operator.h"


struct DestroyOperatorL1 : Operator {
  int minNodesToRemove;
  int maxNodesToRemove;

  std::mt19937 rng;
  std::uniform_int_distribution<int> dist;

  virtual std::vector<int> findNodesToRemove(const Cvrpptpl& problem, Solution& solution)=0;
  DestroyOperatorL1(int minNodesTR, int maxNodesTR, int seed = 42);

  OperatorStatus apply(const Cvrpptpl& problem, Solution& solution) override;
};

struct RandomDestinationRemoval: DestroyOperatorL1{
  using DestroyOperatorL1::DestroyOperatorL1;
  std::vector<int> findNodesToRemove(const Cvrpptpl& problem, Solution& solution) override;
};