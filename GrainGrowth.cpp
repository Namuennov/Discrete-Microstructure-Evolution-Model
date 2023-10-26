#include "GrainGrowth.h"

bool GrainGrowth::didSimulationEnd(Mesh* _mesh)
{
	//TODO - mo¿e lepiej zamiast tego dekrementowaæ licznik ,,pustych" komórek i koñczyæ symulacjê jak siê skoñcz¹?
	int sizeX = _mesh->getSizeX();
	int sizeY = _mesh->getSizeY();
	int sizeZ = _mesh->getSizeZ();
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			for (int z = 0; z < sizeZ; z++)
				if (_mesh->getCell(x, y, z) == 0) return false;
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
	std::uniform_int_distribution<> zDistribution(0, mesh->getSizeZ() - 1);
	std::uniform_int_distribution<> valueDistribution(1, Const::NUMBER_OF_CELL_STATES_BESIDES_ZERO);
	int x, y, z, value;
	for (int i = 0; i < noNucleons; i++) {
		do {
			x = xDistribution(randomGenerator);
			y = yDistribution(randomGenerator);
			z = zDistribution(randomGenerator);
		} while (mesh->getCell(x, y, z) != 0);
		value = valueDistribution(randomGenerator);
		mesh->setCell(x, y, z, value);
	}
}

void GrainGrowth::runSimulation(Config config)
{
	unsigned int sizeX = mesh->getSizeX();
	unsigned int sizeY = mesh->getSizeY();
	unsigned int sizeZ = mesh->getSizeZ();
	Mesh* temporaryMesh1 = new Mesh((*mesh));
	Mesh* temporaryMesh2 = new Mesh((*mesh));
	std::map<int, unsigned int> cellNeighbourhood;
	for (int i = 1; i <= Const::NUMBER_OF_CELL_STATES_BESIDES_ZERO; i++) cellNeighbourhood[i] = 0;
	int dominantState, dominantStateCount;
	std::map<int, unsigned int>::iterator iter;
	std::vector<NeighbourhoodSteps> neighbourhoodSteps = Const::getNeighbourhoodStepsForGivenNeighbourhood(config.neighbourhoodType);
	int checkedX, checkedY, checkedZ, checkedCellState;

#ifdef SAVE_EACH_MESH_STATE
	int noMeshState = 0;
	temporaryMesh1->saveStateToVTK("wyniki/state" + std::to_string(noMeshState) + ".vtk");
#endif
	while (1) {
		for (int x = 0; x < sizeX; x++) {
			for (int y = 0; y < sizeY; y++) {
				for (int z = 0; z < sizeZ; z++) {

					if (temporaryMesh1->getCell(x, y, z) != 0) continue;

					for (int n = 0; n < neighbourhoodSteps.size(); n++) {

						if (sizeZ == 1 && neighbourhoodSteps[n].stepZ != 0) continue;

						checkedX = x + neighbourhoodSteps[n].stepX;
						checkedY = y + neighbourhoodSteps[n].stepY;
						checkedZ = z + neighbourhoodSteps[n].stepZ;
						if (config.boundaryConditionType == boundaryCondition::FIXED) {
							if (checkedX < 0 || checkedX >= sizeX || checkedY < 0 || checkedY >= sizeY || checkedZ < 0 || checkedZ >= sizeZ)
								continue;
						}
						else if (config.boundaryConditionType == boundaryCondition::PERIODIC) {
							if (checkedX < 0) checkedX = sizeX - 1;
							else if (checkedX >= sizeX) checkedX = 0;
							if (checkedY < 0) checkedY = sizeY - 1;
							else if (checkedY >= sizeY) checkedY = 0;
							if (checkedZ < 0) checkedZ = sizeZ - 1;
							else if (checkedZ >= sizeZ) checkedZ = 0;
						}
						checkedCellState = temporaryMesh1->getCell(checkedX, checkedY, checkedZ);
						if (checkedCellState != 0) cellNeighbourhood[checkedCellState]++;
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
						temporaryMesh2->setCell(x, y, z, dominantState);
						for (int i = 1; i <= Const::NUMBER_OF_CELL_STATES_BESIDES_ZERO; i++) cellNeighbourhood[i] = 0;
					}

				}
			}
		}

		delete temporaryMesh1;
		temporaryMesh1 = temporaryMesh2;
#ifdef SAVE_EACH_MESH_STATE
		noMeshState++;
		temporaryMesh1->saveStateToVTK("wyniki/state" + std::to_string(noMeshState) + ".vtk");
#endif
		if (!didSimulationEnd(temporaryMesh1)) temporaryMesh2 = new Mesh((*temporaryMesh1));
		else break;
	}
	for (int x = 0; x < sizeX; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++)
		mesh->setCell(x, y, z, temporaryMesh1->getCell(x, y, z));
	delete temporaryMesh1;
}