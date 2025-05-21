#pragma once

#include "problem/locker.h"

class MrtLine {
  Locker startStation;
  Locker endStation;
  int startStationServiceTime;
  float cost;
  int freightCapacity;

public:
  MrtLine(const Locker& startStation, const Locker& endStation,
    int startStationServiceTime, float cost, int freightCapacity);
};