#include "Mesh.h"

Mesh::Mesh(unsigned int sizeX, unsigned int sizeY)
{
	cells.resize(sizeX);
	for (int i = 0; i < sizeX; i++) cells[i].resize(sizeY);
	for (int i = 0; i < sizeX; i++) for (int j = 0; j < sizeY; j++) cells[i][j] = 0;
}

int Mesh::getSizeX() const { return cells.size(); }

int Mesh::getSizeY() const { return cells[0].size(); }

int Mesh::getCell(unsigned int x, unsigned int y) const { return cells[x][y]; }

void Mesh::setCell(unsigned int x, unsigned int y, int state) { cells[x][y] = state; }

void Mesh::saveStateToCSV(std::string filename)
{
	unsigned int sizeX = cells.size();
	unsigned int sizeY = cells[0].size();

	std::ofstream file(filename);
	file << "x;y;state\n";
	for (int x = 0; x < sizeX; x++) {
		for (int y = 0; y < sizeY; y++) {
			file << x << ";" << y << ";" << cells[x][y] << "\n";
		}
	}
	file.close();
}