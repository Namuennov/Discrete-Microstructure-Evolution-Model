#pragma once
#include <vector>
#include <string>
#include <fstream>

class Mesh
{
	std::vector<std::vector<std::vector<int>>> cells;

public:
	Mesh(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ);
	Mesh(Mesh& mesh);

	int getSizeX() const;
	int getSizeY() const;
	int getSizeZ() const;
	int getCell(unsigned int x, unsigned int y, unsigned int z) const;
	std::vector<std::vector<std::vector<int>>> getAllCells();

	void setCell(unsigned int x, unsigned int y, unsigned int z, int state);

	void saveStateToCSV(std::string filename);
	void saveStateToVTK(std::string filename);
};