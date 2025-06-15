#pragma once

#include "problem/node.h"
#include "problem/locker.h"
#include "problem/customer.h"
#include "problem/mrt_line.h"
#include "problem/vehicle.h"
#include <vector>
#include <variant>
#include <filesystem>

using AllNodeType = std::variant<Node, Customer, Locker, MrtLine>;
namespace fs = std::filesystem;

const int NO_MRT_LINE = -1;

struct Cvrpptpl {
  const Node depot;
  std::vector<Customer> customers;
  std::vector<Locker> lockers;
  std::vector<MrtLine> mrtLines;
  std::vector<AllNodeType> nodes;
  std::vector<Vehicle> vehicles;

  std::vector<std::vector<double>> distanceMatrix;
  std::vector<int> demands;
  std::vector<int> serviceTimes;
  std::vector<int> lockerCapacities;
  std::vector<double> mrtLineCosts;
  std::vector<int> mrtLineCapacities;
  std::vector<int> incomingMrtLineIdxs;
  std::vector<std::array<int, 2>> mrtStationIdxs;
  std::vector<std::vector<int>> destinationAlternatives;
  std::vector<int> vehicleCapacities;
  std::vector<double> vehicleCosts;


  int numNodes;
  int numVehicles;
  int numCustomers;
  int numLockers;
  int numMrtLines;

  std::vector<bool> isCustomer;
  std::vector<bool> isHdCustomer;
  std::vector<bool> isSpCustomer;
  std::vector<bool> isFxCustomer;
  std::vector<bool> isLocker;
  std::vector<bool> isMrtLine;

  Cvrpptpl(const Node& depot, const std::vector<Customer>& customers,
    const std::vector<Locker>& lockers, const std::vector<MrtLine>& mrtLines,
    const std::vector<Vehicle>& vehicles,
    std::vector<std::vector<double>>& distanceMatrixOrig);
};

Cvrpptpl readProblem(fs::path filepath);