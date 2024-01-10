#pragma once
#include <vector>
#include "enums.cpp"

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