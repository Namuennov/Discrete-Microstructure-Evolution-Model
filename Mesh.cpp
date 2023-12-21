#include "Mesh.h"

Mesh::Mesh(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->sizeZ = sizeZ;
	for (unsigned int x = 0; x < sizeX; x++)
		for (unsigned int y = 0; y < sizeY; y++)
			for (unsigned int z = 0; z < sizeZ; z++)
				cells[{x, y, z}] = 0;
}

Mesh::Mesh(Mesh& mesh)
{
	sizeX = mesh.getSizeX();
	sizeY = mesh.getSizeY();
	sizeZ = mesh.getSizeZ();
	cells = mesh.getAllCells();
}

int Mesh::getSizeX() const { return sizeX; }

int Mesh::getSizeY() const { return sizeY; }

int Mesh::getSizeZ() const { return sizeZ; }

int Mesh::getCell(unsigned int x, unsigned int y, unsigned int z) {
	return cells[{x, y, z}];
}

std::unordered_map<coordinates, int> Mesh::getAllCells() { return cells; }

void Mesh::setCell(unsigned int x, unsigned int y, unsigned int z, int state) {
	cells[{x, y, z}] = state;
}

void Mesh::saveStateToCSV(std::string filename)
{
	unsigned int sizeX = this->getSizeX();
	unsigned int sizeY = this->getSizeY();
	unsigned int sizeZ = this->getSizeZ();

	std::ofstream file(filename);
	file << "x;y;z;state\n";
	for (unsigned int x = 0; x < sizeX; x++)
		for (unsigned int y = 0; y < sizeY; y++)
			for (unsigned int z = 0; z < sizeZ; z++)
				file << x << ";" << y << ";" << z << ";" << cells[{x, y, z}] << "\n";
	file.close();
}

void Mesh::saveStateToVTK(std::string filename)
{
	unsigned int sizeX = this->getSizeX();
	unsigned int sizeY = this->getSizeY();
	unsigned int sizeZ = this->getSizeZ();

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
	for (unsigned int x = 0; x < sizeX; x++)
		for (unsigned int y = 0; y < sizeY; y++)
			for (unsigned int z = 0; z < sizeZ; z++)
				file << cells[{x, y, z}] << " \n";
	
	file.close();
}