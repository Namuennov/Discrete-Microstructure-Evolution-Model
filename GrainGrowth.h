#pragma once
#include <random>
#include <ctime>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include "Mesh.h"
#include "Const.h"
#include "enums.cpp"

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

	void runSimulationCA(boundaryCondition boundaryConditionType, neighbourhood neighbourhoodType, std::string eachMeshStateDirectoryName = "");
	void runSimulationMC(boundaryCondition boundaryConditionType, neighbourhood neighbourhoodType, unsigned int noSteps, std::string eachMeshStateDirectoryName = "");
};