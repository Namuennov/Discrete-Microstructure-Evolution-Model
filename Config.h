#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "enums.cpp"

struct Config {
private:
	static const std::string defaultSimulationName;
	static const unsigned int defaultMeshSizeX;
	static const unsigned int defaultMeshSizeY;
	static const unsigned int defaultMeshSizeZ;
	static const boundaryCondition defaultBoundaryConditionType;
	static const neighbourhood defaultNeighbourhoodType;
	static const unsigned int defaultCA_noNucleons;
	static const unsigned int defaultMC_noSteps;

public:
	std::string simulationName;
	unsigned int meshSizeX;
	unsigned int meshSizeY;
	unsigned int meshSizeZ;
	boundaryCondition boundaryConditionType;
	neighbourhood neighbourhoodType;
	unsigned int CA_noNucleons;
	unsigned int MC_noSteps;

	Config(std::string _simulationName, unsigned int _meshSizeX, unsigned int _meshSizeY, unsigned int _meshSizeZ,
		boundaryCondition _boundaryConditionType, neighbourhood _neighbourhoodType, unsigned int _CA_noNucleons, unsigned int _MC_noSteps);
	Config(std::string filename);
};