#include "ConfigurationFilesCreator.h"

std::vector<std::string> ConfigurationFilesCreator::createdConfigurationFileNames;

unsigned int ConfigurationFilesCreator::noCreatedConfigurationFiles = 0;

std::string ConfigurationFilesCreator::createConfigurationFile(unsigned int meshSizeX, unsigned int meshSizeY, unsigned int meshSizeZ,
	unsigned int boundaryConditionTypeId, unsigned int neighbourhoodTypeId, unsigned int CA_noNucleons,
	unsigned int MC_noSteps)
{
	auto currentTime = std::time(nullptr);
	auto currentLocalTime = *std::localtime(&currentTime);
	std::stringstream ss;
	ss << std::put_time(&currentLocalTime, "%F-%H-%M-%S");
	std::string currentTimeFormatted = ss.str();

	std::string boundaryConditionType;
	if (boundaryConditionTypeId == 0) boundaryConditionType = "Fixed";
	else if (boundaryConditionTypeId == 1) boundaryConditionType = "Periodic";
	std::string neighbourhoodType;
	if (neighbourhoodTypeId == 0) neighbourhoodType = "Von Neumann";
	else if (neighbourhoodTypeId == 1) neighbourhoodType = "Moore";

	std::string configurationFileName = "configurationFile_" + currentTimeFormatted + ".csv";
	std::ofstream configurationFile(configurationFileName);

	configurationFile <<
		"Simulation Name;" << currentTimeFormatted << ";\n" <<
		"Mesh Size X;" << meshSizeX << ";\n" <<
		"Mesh Size Y;" << meshSizeY << ";\n" <<
		"Mesh Size Z;" << meshSizeZ << ";\n" <<
		"Boundary Condition;" << boundaryConditionType << ";\n" <<
		"Neighbourhood;" << neighbourhoodType << ";\n" <<
		"CA - Nucleons;" << CA_noNucleons << ";\n" <<
		"MC - Steps;" << MC_noSteps << ";";

	configurationFile.close();
	return configurationFileName;
}