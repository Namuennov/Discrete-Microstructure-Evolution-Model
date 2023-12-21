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

int GrainGrowth::getNeighbourOfCell(Mesh* givenMesh, unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, int x, int y, int z,
	NeighbourhoodSteps steps, boundaryCondition boundaryConditionType)
{
	int checkedX = x + steps.stepX;
	int checkedY = y + steps.stepY;
	int checkedZ = z + steps.stepZ;
	if (boundaryConditionType == boundaryCondition::FIXED) {
		if (checkedX < 0 || checkedX >= sizeX || checkedY < 0 || checkedY >= sizeY || checkedZ < 0 || checkedZ >= sizeZ)
			return Const::CELL_NEIGHBOUR_DOES_NOT_EXIST_SPECIAL_VALUE;
	} else if (boundaryConditionType == boundaryCondition::PERIODIC) {
		if (checkedX < 0) checkedX = sizeX - 1;
		else if (checkedX >= sizeX) checkedX = 0;
		if (checkedY < 0) checkedY = sizeY - 1;
		else if (checkedY >= sizeY) checkedY = 0;
		if (checkedZ < 0) checkedZ = sizeZ - 1;
		else if (checkedZ >= sizeZ) checkedZ = 0;
	}
	return givenMesh->getCell(checkedX, checkedY, checkedZ);
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

void GrainGrowth::setRandomNotZeroStateInEveryCellInMesh()
{
	int sizeX = mesh->getSizeX();
	int sizeY = mesh->getSizeY();
	int sizeZ = mesh->getSizeZ();
	std::mt19937 randomGenerator = std::mt19937(time(0));
	std::uniform_int_distribution<> valueDistribution(1, Const::NUMBER_OF_CELL_STATES_BESIDES_ZERO);
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			for (int z = 0; z < sizeZ; z++)
				mesh->setCell(x, y, z, valueDistribution(randomGenerator));
}

void GrainGrowth::runSimulationCA(boundaryCondition boundaryConditionType, neighbourhood neighbourhoodType, std::string eachMeshStateDirectoryName)
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
	std::vector<NeighbourhoodSteps> neighbourhoodSteps = Const::getNeighbourhoodStepsForGivenNeighbourhood(neighbourhoodType);
	int nsSize = neighbourhoodSteps.size();
	int checkedCellState;

#ifdef SAVE_EACH_MESH_STATE
	std::string eachMeshStateDirectory = "wynikiCA_" + eachMeshStateDirectoryName;
	std::filesystem::create_directories(eachMeshStateDirectory);
	int noMeshState = 0;
	temporaryMesh1->saveStateToVTK(eachMeshStateDirectory + "/state" + std::to_string(noMeshState) + ".vtk");
#endif
	while (1) {
		for (int x = 0; x < sizeX; x++) {
			for (int y = 0; y < sizeY; y++) {
				for (int z = 0; z < sizeZ; z++) {

					if (temporaryMesh1->getCell(x, y, z) != 0) continue;

					for (int n = 0; n < nsSize; n++) {
						if (sizeZ == 1 && neighbourhoodSteps[n].stepZ != 0) continue;
						checkedCellState = getNeighbourOfCell(temporaryMesh1, sizeX, sizeY, sizeZ, x, y, z,
							neighbourhoodSteps[n], boundaryConditionType);
						if (checkedCellState == Const::CELL_NEIGHBOUR_DOES_NOT_EXIST_SPECIAL_VALUE) continue;
						else if (checkedCellState != 0) cellNeighbourhood[checkedCellState]++;
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
		temporaryMesh1->saveStateToVTK(eachMeshStateDirectory + "/state" + std::to_string(noMeshState) + ".vtk");
#endif
		if (!didSimulationEnd(temporaryMesh1)) temporaryMesh2 = new Mesh((*temporaryMesh1));
		else break;
	}
	for (int x = 0; x < sizeX; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++)
		mesh->setCell(x, y, z, temporaryMesh1->getCell(x, y, z));
	delete temporaryMesh1;
}

void GrainGrowth::runSimulationMC(boundaryCondition boundaryConditionType, neighbourhood neighbourhoodType, unsigned int noSteps, std::string eachMeshStateDirectoryName)
{
	unsigned int sizeX = mesh->getSizeX();
	unsigned int sizeY = mesh->getSizeY();
	unsigned int sizeZ = mesh->getSizeZ();
	Mesh* temporaryMesh1 = new Mesh((*mesh));
	Mesh* temporaryMesh2 = new Mesh((*mesh));
	std::vector<NeighbourhoodSteps> neighbourhoodSteps = Const::getNeighbourhoodStepsForGivenNeighbourhood(neighbourhoodType);
	int nsSize = neighbourhoodSteps.size();
	int currentCellState, randomCellState, checkedCellState;
	std::mt19937 randomGenerator = std::mt19937(time(0));
	std::uniform_int_distribution<> neighbourhoodDistribution(0, neighbourhoodSteps.size() - 1);
	NeighbourhoodSteps randomNeighbourhoodSteps;
	int currentEnergy = 0;
	int checkedEnergy = 0;

	int noCells = sizeX * sizeY * sizeZ;
	int** randPermIds = new int* [noCells];
	for (int i = 0; i < noCells; i++) randPermIds[i] = new int[3];
	int index = 0;
	for (int x = 0; x < sizeX; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++) {
		randPermIds[index][0] = x;
		randPermIds[index][1] = y;
		randPermIds[index][2] = z;
		index++;
	}
	std::shuffle(randPermIds, &randPermIds[noCells - 1], randomGenerator);
	int x, y, z;

#ifdef SAVE_EACH_MESH_STATE
	std::string eachMeshStateDirectory = "wynikiMC_" + eachMeshStateDirectoryName;
	std::filesystem::create_directories(eachMeshStateDirectory);
	int noMeshState = 0;
	temporaryMesh1->saveStateToVTK(eachMeshStateDirectory + "/state" + std::to_string(noMeshState) + ".vtk");
#endif
	for (int i = 0; i < noSteps; i++) {
		for (int j = 0; j < noCells; j++) {
			x = randPermIds[j][0];
			y = randPermIds[j][1];
			z = randPermIds[j][2];
			
			currentCellState = temporaryMesh1->getCell(x, y, z);
			
			if (sizeZ == 1) {
				do {
					randomNeighbourhoodSteps = neighbourhoodSteps[neighbourhoodDistribution(randomGenerator)];
				} while (randomNeighbourhoodSteps.stepZ != 0 && randomNeighbourhoodSteps.stepX == 0 && randomNeighbourhoodSteps.stepY == 0);
			}
			else randomNeighbourhoodSteps = neighbourhoodSteps[neighbourhoodDistribution(randomGenerator)];
			
			randomCellState = getNeighbourOfCell(temporaryMesh1, sizeX, sizeY, sizeZ, x, y, z,
				randomNeighbourhoodSteps, boundaryConditionType);
			if (randomCellState == Const::CELL_NEIGHBOUR_DOES_NOT_EXIST_SPECIAL_VALUE) continue;
			
			for (int n = 0; n < neighbourhoodSteps.size(); n++) {
				if (sizeZ == 1 && neighbourhoodSteps[n].stepZ != 0) continue;
				checkedCellState = getNeighbourOfCell(temporaryMesh1, sizeX, sizeY, sizeZ, x, y, z,
					neighbourhoodSteps[n], boundaryConditionType);
				if (checkedCellState == Const::CELL_NEIGHBOUR_DOES_NOT_EXIST_SPECIAL_VALUE) continue;
				if (checkedCellState != currentCellState) currentEnergy++;
				if (checkedCellState != randomCellState) checkedEnergy++;
			}
			
			if (checkedEnergy < currentEnergy) temporaryMesh2->setCell(x, y, z, randomCellState);
			checkedEnergy = 0;
			currentEnergy = 0;
		}

		delete temporaryMesh1;
		temporaryMesh1 = temporaryMesh2;
#ifdef SAVE_EACH_MESH_STATE
		noMeshState++;
		temporaryMesh1->saveStateToVTK(eachMeshStateDirectory + "/state" + std::to_string(noMeshState) + ".vtk");
#endif
		temporaryMesh2 = new Mesh((*temporaryMesh1));
		std::shuffle(randPermIds, &randPermIds[noCells], randomGenerator);
	}

	for (int x = 0; x < sizeX; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++)
		mesh->setCell(x, y, z, temporaryMesh1->getCell(x, y, z));
	delete temporaryMesh1;
	for (int i = 0; i < noCells; i++) delete[] randPermIds[i];
	delete[] randPermIds;
}