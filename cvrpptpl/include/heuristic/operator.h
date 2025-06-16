#pragma once

#include "heuristic/solution.h"
#include "problem/cvrpptpl.h"

enum struct OperatorStatus {
  SUCCESS,
  FAILED
};

struct Operator {
  int score = 0;
  int count = 0;

  virtual OperatorStatus apply(const Cvrpptpl& problem, Solution& solution) = 0;
  virtual ~Operator() = default;
};