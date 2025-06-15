#pragma once

#include "problem/cvrpptpl.h"
#include "heuristic/solution.h"

void insertNodeToRoute(int nodeIdx, int vi, int pos, const Cvrpptpl& problem, Solution& solution);