#pragma once
#include <vector>
#include <string>
#include <fstream>

class Mesh
{
	std::vector<std::vector<int>> cells;

public:
	Mesh(unsigned int sizeX, unsigned int sizeY);

	int getSizeX() const;
	int getSizeY() const;
	int getCell(unsigned int x, unsigned int y) const;

	void setCell(unsigned int x, unsigned int y, int state);

	void saveStateToCSV(std::string filename);
};