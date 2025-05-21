#pragma once

#include "problem/locker.h"

struct MrtLine {
  const Locker startStation;
  const Locker endStation;
  const int startStationServiceTime;
  const float cost;
  const int freightCapacity;

  MrtLine(const Locker& startStation, const Locker& endStation,
    int startStationServiceTime, float cost, int freightCapacity);
};