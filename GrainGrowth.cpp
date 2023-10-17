#include "GrainGrowth.h"

bool GrainGrowth::didSimulationEnd()
{
	//TODO - mo¿e lepiej zamiast tego dekrementowaæ licznik ,,pustych" komórek i koñczyæ symulacjê jak siê skoñcz¹? (to raczej musia³oby byæ w klasie Mesh)
	for (int i = 0; i < mesh->getSizeX(); i++)
		for (int j = 0; j < mesh->getSizeY(); j++)
			if (mesh->getCell(i, j) == 0) return false;
	return true;
}

GrainGrowth::GrainGrowth(Mesh* _mesh)
{
	mesh = _mesh;
}

void GrainGrowth::setRandomInitialConditions(unsigned int noNucleons)
{
	std::mt19937 randomGenerator = std::mt19937(time(0));
	std::uniform_int_distribution<> xDistribution(0, mesh->getSizeX() - 1);
	std::uniform_int_distribution<> yDistribution(0, mesh->getSizeY() - 1);
	std::uniform_int_distribution<> valueDistribution(1, Const::NUMBER_OF_CELL_STATES_BESIDES_ZERO);
	for (int i = 0; i < noNucleons; i++) {
		int x = xDistribution(randomGenerator);
		int y = yDistribution(randomGenerator);
		int value = valueDistribution(randomGenerator);
		mesh->setCell(x, y, value);
	}
}

void GrainGrowth::runSimulation(Config config)
{
	unsigned int sizeX = mesh->getSizeX();
	unsigned int sizeY = mesh->getSizeY();
	std::map<int, unsigned int> cellNeighbourhood;
	for (int i = 1; i <= Const::NUMBER_OF_CELL_STATES_BESIDES_ZERO; i++) cellNeighbourhood[i] = 0;
	int dominantState, dominantStateCount;
	std::map<int, unsigned int>::iterator iter;
	std::vector<NeighbourhoodSteps> neighbourhoodSteps = Const::getNeighbourhoodStepsForGivenNeighbourhood(config.neighbourhoodType);

	int checkedX, checkedY, checkedCellState;
	while (!didSimulationEnd()) {
		for (int x = 0; x < sizeX; x++) {
			for (int y = 0; y < sizeY; y++) {

				if (mesh->getCell(x, y) != 0) continue;

				for (int n = 0; n < neighbourhoodSteps.size(); n++) {
					checkedX = x + neighbourhoodSteps[n].stepX;
					checkedY = y + neighbourhoodSteps[n].stepY;
					if (config.boundaryConditionType == boundaryCondition::FIXED) {
						if (checkedX < 0 || checkedX >= sizeX || checkedY < 0 || checkedY >= sizeY) continue;
					} else if (config.boundaryConditionType == boundaryCondition::PERIODIC) {
						if (checkedX < 0) checkedX = sizeX - 1;
						else if (checkedX >= sizeX) checkedX = 0;
						if (checkedY < 0) checkedY = sizeY - 1;
						else if (checkedY >= sizeY) checkedY = 0;
					}
					checkedCellState = mesh->getCell(checkedX, checkedY);
					if(checkedCellState != 0) cellNeighbourhood[checkedCellState]++;
				}

				dominantStateCount = 0;
				dominantState = 0;
				for (iter = cellNeighbourhood.begin(); iter != cellNeighbourhood.end(); iter++) {
					if (iter->second > dominantStateCount) {
						dominantStateCount = iter->second;
						dominantState = iter->first;
					}
				}

				if (dominantState != 0) {
					mesh->setCell(x, y, dominantState);
					for (int i = 1; i <= Const::NUMBER_OF_CELL_STATES_BESIDES_ZERO; i++) cellNeighbourhood[i] = 0;
				}
			
			}
		}
	}
}