// cvrpptpl.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "problem/cvrpptpl.h"

int main(int argc, char* argv[])
{
	/*if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <input_filepath>" << std::endl;
		return 1;
	}*/
	//std::string filepath = argv[1];
	std::string filepath = "instances/A-n11-k3-m1-b3.txt";
	auto problem = readProblem(filepath);
	return 0;
}
