#pragma once
#include <random>
#include <ctime>
#include <map>
#include "Mesh.h"
#include "Const.h"

class GrainGrowth
{
	Mesh* mesh;

	bool didSimulationEnd();

public:
	GrainGrowth(Mesh* _mesh);

	void setRandomInitialConditions(unsigned int noNucleons);
	void runSimulation(Config config);
};