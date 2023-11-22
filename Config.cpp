#include "Config.h"

const std::string Config::defaultSimulationName = "";
const unsigned int Config::defaultMeshSizeX = 300;
const unsigned int Config::defaultMeshSizeY = 300;
const unsigned int Config::defaultMeshSizeZ = 1;
const boundaryCondition Config::defaultBoundaryConditionType = boundaryCondition::PERIODIC;
const neighbourhood Config::defaultNeighbourhoodType = neighbourhood::VON_NEUMANN;
const unsigned int Config::defaultCA_noNucleons = 0;
const unsigned int Config::defaultMC_noSteps = 0;

Config::Config(std::string _simulationName, unsigned int _meshSizeX, unsigned int _meshSizeY, unsigned int _meshSizeZ,
	boundaryCondition _boundaryConditionType, neighbourhood _neighbourhoodType, unsigned int _CA_noNucleons, unsigned int _MC_noSteps)
{
	simulationName = _simulationName;
	meshSizeX = _meshSizeX;
	meshSizeY = _meshSizeY;
	meshSizeZ = _meshSizeZ;
	boundaryConditionType = _boundaryConditionType;
	neighbourhoodType = _neighbourhoodType;
	CA_noNucleons = _CA_noNucleons;
	MC_noSteps = _MC_noSteps;
}

Config::Config(std::string filename)
{
	simulationName = defaultSimulationName;
	meshSizeX = defaultMeshSizeX;
	meshSizeY = defaultMeshSizeY;
	meshSizeZ = defaultMeshSizeZ;
	boundaryConditionType = defaultBoundaryConditionType;
	neighbourhoodType = defaultNeighbourhoodType;
	CA_noNucleons = defaultCA_noNucleons;
	MC_noSteps = defaultMC_noSteps;

	std::ifstream file(filename);
	if (!file.good()) {
		std::cout << "Failed to open file " << filename << " - the default values will be used\n";
	}
	else {
		std::string line;
		std::getline(file, line);
		char delimiter;
		if (line.find(',') != std::string::npos) delimiter = ',';
		else if (line.find(';') != std::string::npos) delimiter = ';';

		while (1) {
			if (line.find("Simulation Name") != std::string::npos) {
				simulationName = (line.substr(line.find(delimiter) + 1));
				if (simulationName[simulationName.size() - 1] == delimiter) simulationName = simulationName.substr(0, simulationName.size() - 1);
			} else if (line.find("Mesh Size X") != std::string::npos) meshSizeX = std::stoi(line.substr(line.find(delimiter) + 1));
			else if (line.find("Mesh Size Y") != std::string::npos) meshSizeY = std::stoi(line.substr(line.find(delimiter) + 1));
			else if (line.find("Mesh Size Z") != std::string::npos) meshSizeZ = std::stoi(line.substr(line.find(delimiter) + 1));
			else if (line.find("Boundary Condition") != std::string::npos) {
				if (line.find("Fixed") != std::string::npos) boundaryConditionType = boundaryCondition::FIXED;
				else if (line.find("Periodic") != std::string::npos) boundaryConditionType = boundaryCondition::PERIODIC;
			}
			else if (line.find("Neighbourhood") != std::string::npos) {
				if (line.find("Von Neumann") != std::string::npos) neighbourhoodType = neighbourhood::VON_NEUMANN;
				else if (line.find("Moore") != std::string::npos) neighbourhoodType = neighbourhood::MOORE;
			}
			else if (line.find("CA - Nucleons") != std::string::npos) CA_noNucleons = std::stoi(line.substr(line.find(delimiter) + 1));
			else if (line.find("MC - Steps") != std::string::npos) MC_noSteps = std::stoi(line.substr(line.find(delimiter) + 1));
			if (file.eof()) break;
			std::getline(file, line);
		}

		file.close();
	}
}