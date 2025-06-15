#pragma once

#include "problem/locker.h"

struct MrtLine {
  const Locker startStation;
  const Locker endStation;
  const int startStationServiceTime;
  const double cost;
  const int freightCapacity;

  MrtLine(const Locker& startStation, const Locker& endStation,
    int startStationServiceTime, double cost, int freightCapacity);
};