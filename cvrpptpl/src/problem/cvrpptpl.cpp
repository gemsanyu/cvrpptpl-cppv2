#include "problem/cvrpptpl.h"
#include "problem/locker.h"
#include "problem/mrt_line.h"
#include "problem/node.h"

#include <fstream>
#include <iostream>
#include <ranges>
#include <span>

Cvrpptpl::Cvrpptpl(const Node& depot, const std::vector<Customer>& customers,
  const std::vector<Locker>& lockers, const std::vector<MrtLine>& mrtLines, 
  const std::vector<Vehicle>& vehicles,
  std::vector<std::vector<float>>& distanceMatrixOrig)
  : depot(depot), customers(customers), lockers(lockers), mrtLines(mrtLines), vehicles(vehicles) {
  nodes.push_back(depot);
  for (auto& customer : customers) {
    nodes.push_back(customer);
  }
  for (auto& locker: lockers) {
    nodes.push_back(locker);
  }
  numNodes = nodes.size()+mrtLines.size();

  for (std::size_t mi = 0; mi < mrtLines.size(); mi++) {
    std::array<int, 2> stationIdxs = {mrtLines[mi].startStation.idx,
                                      mrtLines[mi].endStation.idx};
    mrtStationIdxs.push_back(stationIdxs);
  }

  distanceMatrix = distanceMatrixOrig;
  numVehicles = vehicles.size();
  for (int vi = 0; vi < numVehicles; vi++) {
    vehicleCapacities.push_back(vehicles[vi].capacity);
  }

  for (int i = 0; i<numNodes; i++) {
    demands.push_back(0);
    serviceTimes.push_back(0);
    lockerCapacities.push_back(0);
    mrtLineCosts.push_back(0);
    mrtLineCapacities.push_back(0);
    destinationAlternatives.push_back({});
    incomingMrtLineIdxs.push_back(-1);
    isCustomer.push_back(false);
    isHdCustomer.push_back(false);
    isSpCustomer.push_back(false);
    isFxCustomer.push_back(false);
    isLocker.push_back(false);
    isMrtLine.push_back(false);
  }
  for (auto& customer : customers) {
    demands[customer.idx] = customer.demand;
    serviceTimes[customer.idx] = customer.serviceTime;
    isCustomer[customer.idx] = true;
    if (customer.isSelfPickup) {
      isSpCustomer[customer.idx] = true;
    } else if (customer.isFlexible) {
      isFxCustomer[customer.idx] = true;
    } else {
      isHdCustomer[customer.idx] = true;
    }
    destinationAlternatives[customer.idx].push_back(customer.idx);
    for (int lockerIdx : customer.lockerPreferenceIdxs) {
      destinationAlternatives[customer.idx].push_back(lockerIdx);
    }
  }
  for (auto& locker : lockers) {
    isLocker[locker.idx] = true;
    serviceTimes[locker.idx] = locker.serviceTime;
    lockerCapacities[locker.idx] = locker.capacity;
  }

  for (int mi = 0; mi < mrtLines.size(); mi++) {
    auto& mrtLine = mrtLines[mi];
    int i = mi + int(nodes.size());
    isMrtLine[i] = true;
    mrtLineCosts[i] = mrtLine.cost;
    mrtLineCapacities[i] = mrtLine.freightCapacity;
    int endStationIdx = mrtLine.endStation.idx;
    incomingMrtLineIdxs[endStationIdx] = mi;
  }
}

std::vector<std::string> splitString(std::string line, char sep) {
  std::vector<std::string> tokens;
  for (auto token : std::views::split(line, sep)) {
    tokens.push_back(std::string(token.begin(), token.end()));
  }
  return tokens;
}

Cvrpptpl readProblem(fs::path filepath) {
  std::ifstream file(filepath);
  std::cout << "Working dir: " << fs::current_path() << std::endl;
  if (!file) {
    std::cerr << "Could not open file: " << filepath << std::endl;
    throw std::runtime_error("Could not open file");
  }
  std::vector<Customer> customers;
  std::vector<Locker> lockers;
  std::vector<MrtLine> mrtLines;
  std::vector<Vehicle> vehicles;
  std::vector<std::vector<float>> distanceMatrix;


  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }
  std::size_t li = 2;
  line = lines[li];
  while (line != "depot") {
    auto tokens = splitString(line, ',');
    Vehicle new_vehicle = Vehicle(std::stoi(tokens[0]), std::stoi(tokens[1]), std::stof(tokens[2]));
    vehicles.push_back(new_vehicle);
    li++;
    line = lines[li];
  }
  li += 2;
  line = lines[li];
  auto tokens = splitString(line, ',');
  Node depot = Node(0, std::stof(tokens[1]), std::stof(tokens[2]));
  li += 3;
  line = lines[li];
  while (line != "self pickup customers") {
    auto tokens = splitString(line, ',');
    Customer customer = Customer(std::stoi(tokens[0]),
      std::stof(tokens[1]),
      std::stof(tokens[2]),
      std::stoi(tokens[3]),
      std::stoi(tokens[4]),
      false,
      false,
      std::vector<int>{}
    );
    customers.push_back(customer);
    li++;
    line = lines[li];
  }

  li += 2;
  line = lines[li];
  while (line != "flexible customers") {
    auto tokens = splitString(line, ',');
    auto lockerPreferredIdxStr = splitString(tokens[5], '-');
    std::vector<int> lockerPrerredIdxs;
    for (auto& lpiStr : lockerPreferredIdxStr) {
      lockerPrerredIdxs.push_back(std::stoi(lpiStr));
    }
    Customer customer = Customer(std::stoi(tokens[0]),
      std::stof(tokens[1]),
      std::stof(tokens[2]),
      std::stoi(tokens[3]),
      std::stoi(tokens[4]),
      true,
      false,
      lockerPrerredIdxs
    );
    customers.push_back(customer);
    li++;
    line = lines[li];
  }

  li += 2;
  line = lines[li];
  while (line != "lockers") {
    auto tokens = splitString(line, ',');
    auto lockerPreferredIdxStr = splitString(tokens[5], '-');
    std::vector<int> lockerPrerredIdxs;
    for (auto& lpiStr : lockerPreferredIdxStr) {
      lockerPrerredIdxs.push_back(std::stoi(lpiStr));
    }
    Customer customer = Customer(std::stoi(tokens[0]),
      std::stof(tokens[1]),
      std::stof(tokens[2]),
      std::stoi(tokens[3]),
      std::stoi(tokens[4]),
      false,
      true,
      lockerPrerredIdxs
    );
    customers.push_back(customer);
    li++;
    line = lines[li];
  }
  auto numCustoners = customers.size();
  li += 2;
  line = lines[li];
  while (line != "mrt lines") {
    auto tokens = splitString(line, ',');
    Locker locker = Locker(std::stoi(tokens[0]),
      std::stof(tokens[1]),
      std::stof(tokens[2]),
      std::stoi(tokens[3]),
      std::stoi(tokens[4]));
    lockers.push_back(locker);
    li++;
    line = lines[li];
  }

  li += 2;
  line = lines[li];
  while (line != "distance matrix") {
    auto tokens = splitString(line, ',');
    int startIdx = std::stoi(tokens[0]);
    int endIdx = std::stoi(tokens[1]);
    const auto startStation = lockers[startIdx - numCustoners - 1];
    const auto endStation = lockers[endIdx - numCustoners - 1];
    MrtLine mrtLine = MrtLine(
      startStation,
      endStation,
      10,
      std::stof(tokens[2]),
      std::stoi(tokens[3])
    );
    mrtLines.push_back(mrtLine);
    li++;
    line = lines[li];
  }
  
  li += 2;
  line = lines[li];
  while (li<lines.size()) {
    if (line.length() < 2) {
      break;
    }
    auto tokens = splitString(line, ',');
    std::vector<float> distanceRow;
    for (auto token : std::span(tokens).subspan(1)) {
      distanceRow.push_back(std::stof(token));
    }
    distanceMatrix.push_back(distanceRow);
    li++;
    if (li < lines.size()) {
      line = lines[li];
    }
  }
  Cvrpptpl problem = Cvrpptpl(depot, customers, lockers, mrtLines, vehicles, distanceMatrix);
  return problem;
}