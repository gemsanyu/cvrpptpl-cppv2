// cvrpptpl.cpp : Defines the entry point for the application.
//
#include <iostream>
#include "problem/cvrpptpl.h"
#include "heuristic/solution.h"
#include "heuristic/greedy_initialization.h"
#include "heuristic/destroy_operator_l1.h"

int main(int argc, char* argv[])
{
	/*if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <input_filepath>" << std::endl;
		return 1;
	}*/
	//std::string filepath = argv[1];
	std::string filepath = "instances/A-n11-k3-m1-b3.txt";
	auto problem = readProblem(filepath);
	auto solution = greedyInitialization(problem);
	for (auto& route : solution.routes) {
		for (int nodeIdx : route) {
			std::cout << nodeIdx << " ";
		}
		std::cout << "\n";
	}
	std::cout << solution.isFeasible(problem) << "\n";

	auto destroyOp = RandomDestinationRemoval(2, 5);
	auto status = destroyOp.apply(problem, solution);
	return 0;
}
