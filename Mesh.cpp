#include "Mesh.h"

Mesh::Mesh(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ)
{
	cells.resize(sizeX);
	for (int x = 0; x < sizeX; x++) cells[x].resize(sizeY);
	for (int x = 0; x < sizeX; x++) for (int y = 0; y < sizeY; y++) cells[x][y].resize(sizeZ);
	for (int x = 0; x < sizeX; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++) cells[x][y][z] = 0;
}

Mesh::Mesh(Mesh& mesh)
{
	unsigned int sizeX = mesh.getSizeX();
	unsigned int sizeY = mesh.getSizeY();
	unsigned int sizeZ = mesh.getSizeZ();
	cells.resize(sizeX);
	for (int x = 0; x < sizeX; x++) cells[x].resize(sizeY);
	for (int x = 0; x < sizeX; x++) for (int y = 0; y < sizeY; y++) cells[x][y].resize(sizeZ);
	for (int x = 0; x < sizeX; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++) cells[x][y][z] = mesh.getCell(x, y, z);
}

int Mesh::getSizeX() const { return cells.size(); }

int Mesh::getSizeY() const { return cells[0].size(); }

int Mesh::getSizeZ() const { return cells[0][0].size(); }

int Mesh::getCell(unsigned int x, unsigned int y, unsigned int z) const { return cells[x][y][z]; }

void Mesh::setCell(unsigned int x, unsigned int y, unsigned int z, int state) { cells[x][y][z] = state; }

void Mesh::saveStateToCSV(std::string filename)
{
	unsigned int sizeX = cells.size();
	unsigned int sizeY = cells[0].size();
	unsigned int sizeZ = cells[0][0].size();

	std::ofstream file(filename);
	file << "x;y;z;state\n";
	for (int x = 0; x < sizeX; x++) {
		for (int y = 0; y < sizeY; y++) {
			for (int z = 0; z < sizeZ; z++) {
				file << x << ";" << y << ";" << z << ";" << cells[x][y][z] << "\n";
			}
		}
	}
	file.close();
}

void Mesh::saveStateToVTK(std::string filename)
{
	unsigned int sizeX = cells.size();
	unsigned int sizeY = cells[0].size();
	unsigned int sizeZ = cells[0][0].size();

	std::ofstream file(filename);
	file << "# vtk DataFile Version 2.0\n" << "3D-ADE data file\n" << "ASCII\n" << "DATASET RECTILINEAR_GRID\n" <<
		"DIMENSIONS " << sizeX << " " << sizeY << " " << sizeZ << "\n" << "X_COORDINATES " << sizeX << " int\n";
	for (int x = 0; x < sizeX; x++) file << x << " ";
	file << "\nY_COORDINATES " << sizeY << " int\n";
	for (int y = 0; y < sizeY; y++) file << y << " ";
	file << "\nZ_COORDINATES " << sizeZ << " int\n";
	for (int z = 0; z < sizeZ; z++) file << z << " ";
	file << "\n" << "POINT_DATA " << (sizeX * sizeY * sizeZ) << "\nSCALARS cells double 1\n" <<
		"LOOKUP_TABLE default\n";
	for (int x = 0; x < sizeX; x++) for (int y = 0; y < sizeY; y++) for (int z = 0; z < sizeZ; z++) file << cells[x][y][z] << " \n";
	
	file.close();
}