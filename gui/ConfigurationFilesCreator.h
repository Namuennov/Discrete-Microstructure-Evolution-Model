#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

class ConfigurationFilesCreator {
public:
	static std::vector<std::string> createdConfigurationFileNames;

	static unsigned int noCreatedConfigurationFiles;

	static std::string createConfigurationFile(unsigned int meshSizeX, unsigned int meshSizeY, unsigned int meshSizeZ,
		unsigned int boundaryConditionTypeId, unsigned int neighbourhoodTypeId, unsigned int CA_noNucleons,
		unsigned int MC_noSteps);
};