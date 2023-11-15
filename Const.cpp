#include "Const.h"

const unsigned int Const::NUMBER_OF_CELL_STATES_BESIDES_ZERO = 10;

const int Const::CELL_NEIGHBOUR_DOES_NOT_EXIST_SPECIAL_VALUE = -1;

const std::vector<NeighbourhoodSteps> Const::getNeighbourhoodStepsForGivenNeighbourhood(neighbourhood _neighbourhood)
{
	switch (_neighbourhood) {
	case(neighbourhood::VON_NEUMANN):
		return std::vector<NeighbourhoodSteps> {
			{ -1, 0, 0 },
			{ 1, 0, 0 },
			{ 0, -1, 0 },
			{ 0, 1, 0 },
			{ 0, 0, -1 },
			{ 0, 0, 1 }};
	case(neighbourhood::MOORE):
		return std::vector<NeighbourhoodSteps> {
			{ -1, 0, 0 },
			{ 1, 0, 0 },
			{ 0, -1, 0 },
			{ 0, 1, 0 },
			{ -1, -1, 0 },
			{ 1, -1, 0 },
			{ -1, 1, 0 },
			{ 1, 1, 0 },
			{ 0, 0, -1 },
			{ -1, 0, -1 },
			{ 1, 0, -1 },
			{ 0, -1, -1 },
			{ 0, 1, -1 },
			{ -1, -1, -1 },
			{ 1, -1, -1 },
			{ -1, 1, -1 },
			{ 1, 1, -1 },
			{ 0, 0, 1 },
			{ -1, 0, 1 },
			{ 1, 0, 1 },
			{ 0, -1, 1 },
			{ 0, 1, 1 },
			{ -1, -1, 1 },
			{ 1, -1, 1 },
			{ -1, 1, 1 },
			{ 1, 1, 1 }};
	}
}