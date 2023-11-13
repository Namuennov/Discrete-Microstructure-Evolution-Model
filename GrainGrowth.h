#pragma once
#include <random>
#include <ctime>
#include <map>
#include <algorithm>
#include "Mesh.h"
#include "Const.h"

class GrainGrowth
{
	Mesh* mesh;

	bool didSimulationEnd(Mesh* _mesh);
	int getNeighbourOfCell(Mesh* givenMesh, unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, int x, int y, int z,
		NeighbourhoodSteps steps, boundaryCondition boundaryConditionType);

public:
	GrainGrowth(Mesh* _mesh);

	void setRandomInitialConditions(unsigned int noNucleons);
	void setRandomNotZeroStateInEveryCellInMesh();

	void runSimulationCA(Config config);
	void runSimulationMC(Config config, unsigned int noSteps);
};