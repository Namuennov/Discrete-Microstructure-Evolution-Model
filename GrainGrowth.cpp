#include "GrainGrowth.h"

int** GrainGrowth::alloc_2d_int(int rows, int cols)
{
	int* data = (int*)malloc(rows * cols * sizeof(int));
	int** array = (int**)malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; i++)
		array[i] = &(data[cols * i]);

	return array;
}

int*** GrainGrowth::alloc_3d_int(int l, int m, int n) {
	int* data = new int[l * m * n];
	int*** array = new int** [l];
	for (int i = 0; i < l; i++) {
		array[i] = new int* [m];
		for (int j = 0; j < m; j++) {
			array[i][j] = &(data[(i * m + j) * n]);
		}
	}
	return array;
}

int** GrainGrowth::makePrevBoundary(Mesh* _mesh)
{
	int sizeY = _mesh->getSizeY();
	int sizeZ = _mesh->getSizeZ();
	int** prevBoundary = alloc_2d_int(sizeY, sizeZ);
	for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++) prevBoundary[y][z] = _mesh->getCell(1, y, z);
	return prevBoundary;
}

int** GrainGrowth::makeNextBoundary(Mesh* _mesh)
{
	int sizeX_m2 = _mesh->getSizeX() - 2;
	int sizeY = _mesh->getSizeY();
	int sizeZ = _mesh->getSizeZ();
	int** nextBoundary = alloc_2d_int(sizeY, sizeZ);
	for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++) nextBoundary[y][z] = _mesh->getCell(sizeX_m2, y, z);
	return nextBoundary;
}

void GrainGrowth::assimilateBoundaries(Mesh* _mesh, int** prevBoundary, int** nextBoundary)
{
	int sizeX_m1 = _mesh->getSizeX() - 1;
	int sizeY = _mesh->getSizeY();
	int sizeZ = _mesh->getSizeZ();
	for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++) {
		_mesh->setCell(0, y, z, prevBoundary[y][z]);
		_mesh->setCell(sizeX_m1, y, z, nextBoundary[y][z]);
	}
	free(prevBoundary[0]);
	free(prevBoundary);
	free(nextBoundary[0]);
	free(nextBoundary);
}

int*** GrainGrowth::makeFinalMeshToPass(Mesh* _mesh)
{
	int sizeX_m1 = _mesh->getSizeX() - 1;
	int sizeY = _mesh->getSizeY();
	int sizeZ = _mesh->getSizeZ();
	int*** finalMeshToPass = alloc_3d_int(sizeX_m1 - 1, sizeY, sizeZ);
	for (int x = 1; x < sizeX_m1; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++)
		finalMeshToPass[x - 1][y][z] = _mesh->getCell(x, y, z);
	return finalMeshToPass;

}

bool GrainGrowth::didSimulationEnd(Mesh* _mesh)
{
	int sizeX_m1 = _mesh->getSizeX() - 1;
	int sizeY = _mesh->getSizeY();
	int sizeZ = _mesh->getSizeZ();
	for (int x = 1; x < sizeX_m1; x++)
		for (int y = 0; y < sizeY; y++)
			for (int z = 0; z < sizeZ; z++)
				if (_mesh->getCell(x, y, z) == 0) return false;
	return true;
}

int GrainGrowth::getNeighbourOfCell(Mesh* givenMesh, unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, int x, int y, int z,
	NeighbourhoodSteps steps, boundaryCondition boundaryConditionType, int mpiRank, int mpiSize)
{
	int checkedX = x + steps.stepX;
	int checkedY = y + steps.stepY;
	int checkedZ = z + steps.stepZ;
	if (boundaryConditionType == boundaryCondition::FIXED) {
		if ((checkedX == 0 && mpiRank == 0) || (checkedX == (sizeX - 1) && mpiRank == (mpiSize - 1)) ||
			checkedY < 0 || checkedY >= sizeY || checkedZ < 0 || checkedZ >= sizeZ)
			return Const::CELL_NEIGHBOUR_DOES_NOT_EXIST_SPECIAL_VALUE;
	}
	else if (boundaryConditionType == boundaryCondition::PERIODIC) {
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

Mesh* GrainGrowth::runSimulationCA(boundaryCondition boundaryConditionType, neighbourhood neighbourhoodType, int argc, char** argv)
{
	MPI_Request request;
	MPI_Status status;
	int mpiRank, mpiSize;
	int prevBoundaryTag = 1;
	int nextBoundaryTag = 2;
	int prevEndTag = 3;
	int nextEndTag = 4;
	int finalMeshPassTag = 5;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

	int **prevBoundary, **nextBoundary;
	int ***finalMesh;
	int prevRank, nextRank;
	prevRank = mpiRank == 0 ? mpiSize - 1 : mpiRank - 1;
	nextRank = mpiRank == (mpiSize - 1) ? 0 : mpiRank + 1;
	bool didPrevEnd = false;
	bool didNextEnd = false;
	bool didMeEnd = false;

	Mesh* temporaryMesh1 = new Mesh((*mesh), mpiRank, mpiSize);
	Mesh* temporaryMesh2 = new Mesh((*temporaryMesh1));
	unsigned int sizeX = temporaryMesh1->getSizeX();
	unsigned int sizeX_m1 = sizeX - 1;
	unsigned int sizeY = temporaryMesh1->getSizeY();
	unsigned int sizeZ = temporaryMesh1->getSizeZ();
	std::map<int, unsigned int> cellNeighbourhood;
	for (int i = 1; i <= Const::NUMBER_OF_CELL_STATES_BESIDES_ZERO; i++) cellNeighbourhood[i] = 0;
	int dominantState, dominantStateCount;
	std::map<int, unsigned int>::iterator iter;
	std::vector<NeighbourhoodSteps> neighbourhoodSteps = Const::getNeighbourhoodStepsForGivenNeighbourhood(neighbourhoodType);
	int checkedCellState;

	while (1) {
		for (int x = 1; x < sizeX_m1; x++) {
			for (int y = 0; y < sizeY; y++) {
				for (int z = 0; z < sizeZ; z++) {

					if (temporaryMesh1->getCell(x, y, z) != 0) continue;

					for (int n = 0; n < neighbourhoodSteps.size(); n++) {
						if (sizeZ == 1 && neighbourhoodSteps[n].stepZ != 0) continue;
						checkedCellState = getNeighbourOfCell(temporaryMesh1, sizeX, sizeY, sizeZ, x, y, z,
							neighbourhoodSteps[n], boundaryConditionType, mpiRank, mpiSize);
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

		prevBoundary = makePrevBoundary(temporaryMesh1);
		nextBoundary = makeNextBoundary(temporaryMesh1);
		MPI_Isend(&(prevBoundary[0][0]), sizeY * sizeZ, MPI_INT, prevRank, prevBoundaryTag, MPI_COMM_WORLD, &request);
		MPI_Isend(&(nextBoundary[0][0]), sizeY * sizeZ, MPI_INT, nextRank, nextBoundaryTag, MPI_COMM_WORLD, &request);
		if (!didPrevEnd) MPI_Recv(&(prevBoundary[0][0]), sizeY * sizeZ, MPI_INT, prevRank, nextBoundaryTag, MPI_COMM_WORLD, &status);
		if (!didNextEnd) MPI_Recv(&(nextBoundary[0][0]), sizeY * sizeZ, MPI_INT, nextRank, prevBoundaryTag, MPI_COMM_WORLD, &status);
		assimilateBoundaries(temporaryMesh1, prevBoundary, nextBoundary);

		if (didSimulationEnd(temporaryMesh1)) didMeEnd = true;
		MPI_Isend(&didMeEnd, 1, MPI_C_BOOL, prevRank, prevEndTag, MPI_COMM_WORLD, &request);
		MPI_Isend(&didMeEnd, 1, MPI_C_BOOL, nextRank, nextEndTag, MPI_COMM_WORLD, &request);
		if (!didPrevEnd) MPI_Recv(&didPrevEnd, 1, MPI_C_BOOL, prevRank, nextEndTag, MPI_COMM_WORLD, &status);
		if (!didNextEnd) MPI_Recv(&didNextEnd, 1, MPI_C_BOOL, nextRank, prevEndTag, MPI_COMM_WORLD, &status);

		if (!didMeEnd) temporaryMesh2 = new Mesh((*temporaryMesh1));
		else break;
	}

	if (mpiRank != 0) {
		finalMesh = makeFinalMeshToPass(temporaryMesh1);
		MPI_Send(&(finalMesh[0][0][0]), (sizeX - 2) * sizeY * sizeZ, MPI_INT, 0, finalMeshPassTag, MPI_COMM_WORLD);
		free(finalMesh[0][0]);
		free(finalMesh[0]);
		free(finalMesh);
		delete temporaryMesh1;
		MPI_Finalize();
	}
	if (mpiRank == 0) {
		unsigned int sizeX_m2 = sizeX - 2;
		int baseSizeX = mesh->getSizeX();
		int lastRankSizeX = (baseSizeX / mpiSize) + (baseSizeX % mpiSize);
		int*** mergedMeshes = alloc_3d_int(baseSizeX, sizeY, sizeZ);
		for (int i = 1; i < (mpiSize - 1); i++) MPI_Recv(&(mergedMeshes[i * sizeX_m2][0][0]), (sizeX - 2) * sizeY * sizeZ,
			MPI_INT, i, finalMeshPassTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&(mergedMeshes[(mpiSize - 1) * sizeX_m2][0][0]), lastRankSizeX * sizeY * sizeZ,
			MPI_INT, mpiSize - 1, finalMeshPassTag, MPI_COMM_WORLD, &status);

		Mesh* result = new Mesh(mesh->getSizeX(), sizeY, sizeZ);
		for (int x = 1; x < sizeX_m1; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++)
			result->setCell(x - 1, y, z, temporaryMesh1->getCell(x, y, z));
		for (int x = sizeX_m2; x < baseSizeX; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++)
			result->setCell(x, y, z, mergedMeshes[x][y][z]);
		free(mergedMeshes[0][0]);
		free(mergedMeshes[0]);
		free(mergedMeshes);
		delete temporaryMesh1;
		MPI_Finalize();
		return result;
	}
}

Mesh* GrainGrowth::runSimulationMC(boundaryCondition boundaryConditionType, neighbourhood neighbourhoodType, unsigned int noSteps, int argc, char** argv)
{
	MPI_Request request;
	MPI_Status status;
	int mpiRank, mpiSize;
	int prevBoundaryTag = 1;
	int nextBoundaryTag = 2;
	int prevEndTag = 3;
	int nextEndTag = 4;
	int finalMeshPassTag = 5;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

	int** prevBoundary, ** nextBoundary;
	int*** finalMesh;
	int prevRank, nextRank;
	prevRank = mpiRank == 0 ? mpiSize - 1 : mpiRank - 1;
	nextRank = mpiRank == (mpiSize - 1) ? 0 : mpiRank + 1;

	Mesh* temporaryMesh1 = new Mesh((*mesh), mpiRank, mpiSize);
	Mesh* temporaryMesh2 = new Mesh((*temporaryMesh1));
	unsigned int sizeX = temporaryMesh1->getSizeX();
	unsigned int sizeX_m1 = sizeX - 1;
	unsigned int sizeY = temporaryMesh1->getSizeY();
	unsigned int sizeZ = temporaryMesh1->getSizeZ();
	std::vector<NeighbourhoodSteps> neighbourhoodSteps = Const::getNeighbourhoodStepsForGivenNeighbourhood(neighbourhoodType);
	int currentCellState, randomCellState, checkedCellState;
	std::mt19937 randomGenerator = std::mt19937(time(0));
	std::uniform_int_distribution<> neighbourhoodDistribution(0, neighbourhoodSteps.size() - 1);
	NeighbourhoodSteps randomNeighbourhoodSteps;
	int currentEnergy = 0;
	int checkedEnergy = 0;

	int noCells = (sizeX-2) * sizeY * sizeZ;
	int** randPermIds = new int* [noCells];
	for (int i = 0; i < noCells; i++) randPermIds[i] = new int[3];
	int index = 0;
	for (int x = 1; x < sizeX_m1; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++) {
		randPermIds[index][0] = x;
		randPermIds[index][1] = y;
		randPermIds[index][2] = z;
		index++;
	}
	std::shuffle(randPermIds, &randPermIds[noCells - 1], randomGenerator);
	int x, y, z;

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
				randomNeighbourhoodSteps, boundaryConditionType, mpiRank, mpiSize);
			if (randomCellState == Const::CELL_NEIGHBOUR_DOES_NOT_EXIST_SPECIAL_VALUE) continue;

			for (int n = 0; n < neighbourhoodSteps.size(); n++) {
				if (sizeZ == 1 && neighbourhoodSteps[n].stepZ != 0) continue;
				checkedCellState = getNeighbourOfCell(temporaryMesh1, sizeX, sizeY, sizeZ, x, y, z,
					neighbourhoodSteps[n], boundaryConditionType, mpiRank, mpiSize);
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

		prevBoundary = makePrevBoundary(temporaryMesh1);
		nextBoundary = makeNextBoundary(temporaryMesh1);
		MPI_Isend(&(prevBoundary[0][0]), sizeY * sizeZ, MPI_INT, prevRank, prevBoundaryTag, MPI_COMM_WORLD, &request);
		MPI_Isend(&(nextBoundary[0][0]), sizeY * sizeZ, MPI_INT, nextRank, nextBoundaryTag, MPI_COMM_WORLD, &request);
		MPI_Recv(&(prevBoundary[0][0]), sizeY * sizeZ, MPI_INT, prevRank, nextBoundaryTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&(nextBoundary[0][0]), sizeY * sizeZ, MPI_INT, nextRank, prevBoundaryTag, MPI_COMM_WORLD, &status);
		assimilateBoundaries(temporaryMesh1, prevBoundary, nextBoundary);

		temporaryMesh2 = new Mesh((*temporaryMesh1));
		std::shuffle(randPermIds, &randPermIds[noCells - 1], randomGenerator);
	}

	if (mpiRank != 0) {
		finalMesh = makeFinalMeshToPass(temporaryMesh1);
		MPI_Send(&(finalMesh[0][0][0]), (sizeX - 2) * sizeY * sizeZ, MPI_INT, 0, finalMeshPassTag, MPI_COMM_WORLD);
		free(finalMesh[0][0]);
		free(finalMesh[0]);
		free(finalMesh);
		delete temporaryMesh1;
		for (int i = 0; i < noCells; i++) delete[] randPermIds[i];
		delete[] randPermIds;
		MPI_Finalize();
	}
	if (mpiRank == 0) {
		unsigned int sizeX_m2 = sizeX - 2;
		int baseSizeX = mesh->getSizeX();
		int lastRankSizeX = (baseSizeX / mpiSize) + (baseSizeX % mpiSize);
		int*** mergedMeshes = alloc_3d_int(baseSizeX, sizeY, sizeZ);
		for (int i = 1; i < (mpiSize - 1); i++) MPI_Recv(&(mergedMeshes[i * sizeX_m2][0][0]), (sizeX - 2) * sizeY * sizeZ,
			MPI_INT, i, finalMeshPassTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&(mergedMeshes[(mpiSize - 1) * sizeX_m2][0][0]), lastRankSizeX * sizeY * sizeZ,
			MPI_INT, mpiSize - 1, finalMeshPassTag, MPI_COMM_WORLD, &status);

		Mesh* result = new Mesh(mesh->getSizeX(), sizeY, sizeZ);
		for (int x = 1; x < sizeX_m1; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++)
			result->setCell(x - 1, y, z, temporaryMesh1->getCell(x, y, z));
		for (int x = sizeX_m2; x < baseSizeX; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++)
			result->setCell(x, y, z, mergedMeshes[x][y][z]);
		free(mergedMeshes[0][0]);
		free(mergedMeshes[0]);
		free(mergedMeshes);
		delete temporaryMesh1;
		for (int i = 0; i < noCells; i++) delete[] randPermIds[i];
		delete[] randPermIds;
		MPI_Finalize();
		return result;
	}
}