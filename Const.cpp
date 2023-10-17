#include "Const.h"

Config::Config(boundaryCondition _boundaryConditionType, neighbourhood _neighbourhoodType)
{
	boundaryConditionType = _boundaryConditionType;
	neighbourhoodType = _neighbourhoodType;
}

const unsigned int Const::NUMBER_OF_CELL_STATES_BESIDES_ZERO = 10;

const std::vector<NeighbourhoodSteps> Const::getNeighbourhoodStepsForGivenNeighbourhood(neighbourhood _neighbourhood)
{
	switch (_neighbourhood) {
	case(neighbourhood::VON_NEUMANN):
		return std::vector<NeighbourhoodSteps> {
			{ -1, 0 },
			{1, 0},
			{0, -1},
			{0, 1} };
	case(neighbourhood::MOORE):
		return std::vector<NeighbourhoodSteps> {
			{ -1, 0 },
			{ 1, 0 },
			{ 0, -1 },
			{ 0, 1 },
			{ -1, -1 },
			{ 1, -1 },
			{ -1, 1 },
			{ 1, 1 }};
	}
}