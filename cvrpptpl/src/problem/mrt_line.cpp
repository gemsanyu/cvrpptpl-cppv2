#include "problem/locker.h"
#include "problem/mrt_line.h"


MrtLine::MrtLine(const Locker& startStation, const Locker& endStation,
  int startStationServiceTime, float cost, int freightCapacity)
  : startStation(startStation),
    endStation(endStation),
    startStationServiceTime(startStationServiceTime),
    cost(cost),
    freightCapacity(freightCapacity){ }