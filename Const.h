#pragma once
#include <vector>
#include "enums.cpp"

#define PERFORM_CELLULAR_AUTOMATA_GRAIN_GROWTH
#define PERFORM_MONTE_CARLO_GRAIN_GROWTH
#define SAVE_EACH_MESH_STATE

struct NeighbourhoodSteps {
	int stepX;
	int stepY;
	int stepZ;
};

class Const {
public:
	static const unsigned int NUMBER_OF_CELL_STATES_BESIDES_ZERO;

	static const int CELL_NEIGHBOUR_DOES_NOT_EXIST_SPECIAL_VALUE;

	static const std::vector<NeighbourhoodSteps> getNeighbourhoodStepsForGivenNeighbourhood(neighbourhood _neighbourhood);
};