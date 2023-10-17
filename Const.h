#pragma once
#include <vector>
#include "enums.cpp"

struct Config {
	boundaryCondition boundaryConditionType;
	neighbourhood neighbourhoodType;

	Config(boundaryCondition _boundaryConditionType, neighbourhood _neighbourhoodType);
};

struct NeighbourhoodSteps {
	int stepX;
	int stepY;
};

class Const {
public:
	static const unsigned int NUMBER_OF_CELL_STATES_BESIDES_ZERO;

	static const std::vector<NeighbourhoodSteps> getNeighbourhoodStepsForGivenNeighbourhood(neighbourhood _neighbourhood);
};