#include "problem/cvrpptpl.h"
#include "problem/locker.h"
#include "problem/mrt_line.h"
#include "problem/node.h"

#include <ranges>

Cvrpptpl::Cvrpptpl(const Node& depot, const std::vector<Customer>& customers,
  const std::vector<Locker>& lockers, const std::vector<MrtLine>& mrtLines, 
  std::vector<std::vector<float>>& distanceMatrixOrig)
  : depot(depot), customers(customers), lockers(lockers), mrtLines(mrtLines) {
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

  for (int i = 0; i < nodes.size(); i++) {
    for (int j = 0; j < nodes.size(); j++) {
      distanceMatrix[i][j] = distanceMatrixOrig[i][j];
    }
    for (int j = nodes.size(); j < numNodes; j++) {
      int mrtStartStationIdx = mrtLines[j - nodes.size()].startStation.idx;
      distanceMatrix[i][j] = distanceMatrixOrig[i][mrtStartStationIdx];
    }
  }
  for (int i = nodes.size(); i < numNodes; i++) {
    int mrtStartStationIdx = mrtLines[i - nodes.size()].startStation.idx;
    for (int j = 0; j < numNodes; j++) {
      distanceMatrix[i][j] = distanceMatrix[mrtStartStationIdx][j];
    }
  }

  for (int i = 0; i<numNodes; i++) {
    demands.push_back(0);
    serviceTimes.push_back(0);
    lockerCapacities.push_back(0);
    lockerCosts.push_back(0);
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
    lockerCosts[locker.idx] = locker.cost;
  }

  for (int mi = 0; mi < mrtLines.size(); mi++) {
    auto& mrtLine = mrtLines[mi];
    int i = mi + nodes.size();
    isMrtLine[i] = true;
    mrtLineCosts[i] = mrtLine.cost;
    mrtLineCapacities[i] = mrtLine.freightCapacity;
    int endStationIdx = mrtLine.endStation.idx;
    incomingMrtLineIdxs[endStationIdx] = mi;
  }
}
