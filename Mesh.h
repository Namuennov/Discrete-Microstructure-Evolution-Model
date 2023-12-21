#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include "coordinates.cpp"

class Mesh
{
	std::unordered_map<coordinates, int> cells;
	int sizeX;
	int sizeY;
	int sizeZ;

public:
	Mesh(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ);
	Mesh(Mesh& mesh);

	int getSizeX() const;
	int getSizeY() const;
	int getSizeZ() const;
	int getCell(unsigned int x, unsigned int y, unsigned int z);
	std::unordered_map<coordinates, int> getAllCells();

	void setCell(unsigned int x, unsigned int y, unsigned int z, int state);

	void saveStateToCSV(std::string filename);
	void saveStateToVTK(std::string filename);
};