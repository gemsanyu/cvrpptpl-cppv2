#include "problem/cvrpptpl.h"
#include "problem/locker.h"
#include "problem/mrt_line.h"
#include "problem/node.h"

Cvrpptpl::Cvrpptpl(const Node& depot, const std::vector<Customer>& customers,
  const std::vector<Locker>& lockers, const std::vector<MrtLine>& mrtLines)
  : depot(depot), customers(customers), lockers(lockers), mrtLines(mrtLines) {
  nodes.push_back(depot);
  for (auto& customer : customers) {
    nodes.push_back(customer);
  }
  for (auto& locker: lockers) {
    nodes.push_back(locker);
  }
  numNodes = nodes.size()+mrtLines.size();



}
