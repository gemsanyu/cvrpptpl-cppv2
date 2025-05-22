#include "problem/cvrpptpl.h"
#include "heuristic/solution.h"


Solution::Solution(const Cvrpptpl& problem) {
  for (int vi = 0; vi < problem.numVehicles; vi++) {
    routes.push_back({});
    vehicleLoads.push_back(0);
    vehicleCharges.push_back(0);
  }
  for (int i = 0; i < problem.numNodes; i++) {
    packageDestinations.push_back(NO_DESTINATION);
    destinationVehicleAssignments.push_back(NO_VEHICLE);
    destinationTotalDemands.push_back(0);
    lockerLoads.push_back(0);
    lockerCharges.push_back(0);
  }
  for (int mi = 0; mi < problem.mrtLines.size(); mi++) {
    mrtLoads.push_back(0);
    mrtCharges.push_back(0);
  }
}

Solution::Solution(const Solution& other): routes(other.routes),
packageDestinations(other.packageDestinations),
destinationTotalDemands(other.destinationTotalDemands),
vehicleLoads(other.vehicleLoads),
lockerLoads(other.lockerLoads),
destinationVehicleAssignments(other.destinationVehicleAssignments),
mrtLoads(other.mrtLoads),
lockerCharges(other.lockerCharges),
vehicleCharges(other.vehicleCharges),
mrtCharges(other.mrtCharges),
totalCost(other.totalCost) {}