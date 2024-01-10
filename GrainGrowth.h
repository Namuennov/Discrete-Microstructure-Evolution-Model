#pragma once
#include <random>
#include <ctime>
#include <map>
#include <algorithm>
#include <filesystem>
#include <mpi.h>
#include "Mesh.h"
#include "Const.h"
#include "enums.cpp"

class GrainGrowth
{
	Mesh* mesh;

	int** alloc_2d_int(int rows, int cols);
	int*** alloc_3d_int(int l, int m, int n);
	int** makePrevBoundary(Mesh* _mesh);
	int** makeNextBoundary(Mesh* _mesh);
	void assimilateBoundaries(Mesh* _mesh, int** prevBoundary, int** nextBoundary);
	int*** makeFinalMeshToPass(Mesh* _mesh);

	bool didSimulationEnd(Mesh* _mesh);
	int getNeighbourOfCell(Mesh* givenMesh, unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, int x, int y, int z,
		NeighbourhoodSteps steps, boundaryCondition boundaryConditionType, int mpiRank, int mpiSize);

public:
	GrainGrowth(Mesh* _mesh);

	void setRandomInitialConditions(unsigned int noNucleons);
	void setRandomNotZeroStateInEveryCellInMesh();

	Mesh* runSimulationCA(boundaryCondition boundaryConditionType, neighbourhood neighbourhoodType, int argc, char** argv);
	Mesh* runSimulationMC(boundaryCondition boundaryConditionType, neighbourhood neighbourhoodType, unsigned int noSteps, int argc, char** argv);
};