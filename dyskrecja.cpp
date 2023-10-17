//Jakub Foryś   AGH   2023
#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include "enums.cpp"
#include "Mesh.h"
#include "GrainGrowth.h"

int main()
{
    unsigned int meshSizeX = 300;
    unsigned int meshSizeY = 300;
    unsigned int noNucleons = 100;
    boundaryCondition boundaryConditionType = boundaryCondition::FIXED;
    neighbourhood neighbourhoodType = neighbourhood::VON_NEUMANN;

    auto startTimeMeasurement = std::chrono::system_clock::now();
    Config config(boundaryConditionType, neighbourhoodType);
    Mesh* mesh = new Mesh(meshSizeX, meshSizeY);
    GrainGrowth* model = new GrainGrowth(mesh);
    model->setRandomInitialConditions(noNucleons);
    auto endTimeMeasurement = std::chrono::system_clock::now();
    auto elapsedTimeInitialization = std::chrono::duration_cast<std::chrono::microseconds>(endTimeMeasurement - startTimeMeasurement);

    startTimeMeasurement = std::chrono::system_clock::now();
    model->runSimulation(config);
    endTimeMeasurement = std::chrono::system_clock::now();
    auto elapsedTimeSimulation = std::chrono::duration_cast<std::chrono::microseconds>(endTimeMeasurement - startTimeMeasurement);

    startTimeMeasurement = std::chrono::system_clock::now();
    mesh->saveStateToCSV("simulationResult.csv");
    endTimeMeasurement = std::chrono::system_clock::now();
    auto elapsedTimeWritingResultToFile = std::chrono::duration_cast<std::chrono::microseconds>(endTimeMeasurement - startTimeMeasurement);

    std::string timeMeasurementsFilename = "timeMeasurements.txt";
    std::ofstream timeMeasurementsFile(timeMeasurementsFilename);
    timeMeasurementsFile << "Time of initialization: " << elapsedTimeInitialization.count() << " microseconds\n" <<
        "Time of simulation: " << elapsedTimeSimulation.count() << " microseconds\n" <<
        "Time of writing the result to a file: " << elapsedTimeWritingResultToFile.count() << " microseconds";
    timeMeasurementsFile.close();

    delete model;
    delete mesh;
}