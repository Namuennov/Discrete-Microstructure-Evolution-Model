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
    unsigned int meshSizeZ = 1;
#ifdef PERFORM_CELLULAR_AUTOMATA_GRAIN_GROWTH
    unsigned int noNucleons = 100;
#endif
#ifdef PERFORM_MONTE_CARLO_GRAIN_GROWTH
    unsigned int noSteps = 300;
#endif
    boundaryCondition boundaryConditionType = boundaryCondition::PERIODIC;
    neighbourhood neighbourhoodType = neighbourhood::MOORE;

    Config config(boundaryConditionType, neighbourhoodType);
    std::chrono::time_point<std::chrono::system_clock> startTime, endTime;


#ifdef PERFORM_CELLULAR_AUTOMATA_GRAIN_GROWTH
    startTime = std::chrono::system_clock::now();
    Mesh* meshCA = new Mesh(meshSizeX, meshSizeY, meshSizeZ);
    GrainGrowth* modelCA = new GrainGrowth(meshCA);
    modelCA->setRandomInitialConditions(noNucleons);
    endTime = std::chrono::system_clock::now();
    auto elapsedTimeInitCA = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    startTime = std::chrono::system_clock::now();
    modelCA->runSimulationCA(config);
    endTime = std::chrono::system_clock::now();
    auto elapsedTimeSimulationCA = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    startTime = std::chrono::system_clock::now();
    meshCA->saveStateToVTK("simulationResultCA.vtk");
    endTime = std::chrono::system_clock::now();
    auto elapsedTimeWritingResultToFileCA = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    delete modelCA;
    delete meshCA;
#endif


#ifdef PERFORM_MONTE_CARLO_GRAIN_GROWTH
    startTime = std::chrono::system_clock::now();
    Mesh* meshMC = new Mesh(meshSizeX, meshSizeY, meshSizeZ);
    GrainGrowth* modelMC = new GrainGrowth(meshMC);
    modelMC->setRandomNotZeroStateInEveryCellInMesh();
    endTime = std::chrono::system_clock::now();
    auto elapsedTimeInitMC = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    startTime = std::chrono::system_clock::now();
    modelMC->runSimulationMC(config, noSteps);
    endTime = std::chrono::system_clock::now();
    auto elapsedTimeSimulationMC = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    startTime = std::chrono::system_clock::now();
    meshMC->saveStateToVTK("simulationResultMC.vtk");
    endTime = std::chrono::system_clock::now();
    auto elapsedTimeWritingResultToFileMC = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    delete modelMC;
    delete meshMC;
#endif


    std::string timeMeasurementsFilename = "timeMeasurements.txt";
    std::ofstream timeMeasurementsFile(timeMeasurementsFilename);
#ifdef PERFORM_CELLULAR_AUTOMATA_GRAIN_GROWTH
    timeMeasurementsFile << "Time of initialization CA: " << elapsedTimeInitCA.count() << " microseconds\n";
    timeMeasurementsFile << "Time of simulation CA: " << elapsedTimeSimulationCA.count() << " microseconds\n";
    timeMeasurementsFile << "Time of writing the result to a file CA: " << elapsedTimeWritingResultToFileCA.count() << " microseconds";
#endif
#ifdef PERFORM_MONTE_CARLO_GRAIN_GROWTH
    timeMeasurementsFile << "Time of initialization MC: " << elapsedTimeInitMC.count() << " microseconds\n";
    timeMeasurementsFile << "Time of simulation MC: " << elapsedTimeSimulationMC.count() << " microseconds\n";
    timeMeasurementsFile << "Time of writing the result to a file MC: " << elapsedTimeWritingResultToFileMC.count() << " microseconds";
#endif
    timeMeasurementsFile.close();
}