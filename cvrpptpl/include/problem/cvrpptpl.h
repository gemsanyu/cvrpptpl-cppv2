#pragma once

#include "problem/node.h"
#include "problem/locker.h"
#include "problem/customer.h"
#include "problem/mrt_line.h"
#include <vector>
#include <variant>

using AllNodeType = std::variant<Node, Customer, Locker, MrtLine>;

class Cvrpptpl {
  const Node depot;
  std::vector<const Customer> customers;
  std::vector<const Locker> lockers;
  std::vector<const MrtLine> mrtLines;
  std::vector<const AllNodeType> nodes;

  std::vector<std::vector<float>> distanceMatrix;
  std::vector<int> demands;
  std::vector<int> serviceTimes;
  int numNodes;


public:
  Cvrpptpl(const Node& depot, const std::vector<Customer>& customers,
    const std::vector<Locker>& lockers, const std::vector<MrtLine>& mrtLines,
    std::vector<std::vector<float>>& distanceMatrixOrig);
};