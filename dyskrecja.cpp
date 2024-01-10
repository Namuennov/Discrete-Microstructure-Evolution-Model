//Jakub Foryś   AGH   2023
#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include "enums.cpp"
#include "Config.h"
#include "Mesh.h"
#include "GrainGrowth.h"

int main(int argc, char** argv)
{
    //test dla CA v
    //Config config("test2D", 1000, 1000, 1, boundaryCondition::PERIODIC, neighbourhood::VON_NEUMANN, 100, 50);
    ////Config config("test2D", 1000, 1000, 1, boundaryCondition::PERIODIC, neighbourhood::MOORE, 100, 50);
    ////Config config("test3D", 100, 100, 100, boundaryCondition::PERIODIC, neighbourhood::VON_NEUMANN, 100, 50);
    ////Config config("test3D", 100, 100, 100, boundaryCondition::PERIODIC, neighbourhood::MOORE, 100, 50);
    //std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
    //std::chrono::microseconds elapsedTimeInitCA, elapsedTimeSimulationCA, elapsedTimeWritingResultToFileCA;

    //startTime = std::chrono::system_clock::now();
    //Mesh* meshInit = new Mesh(config.meshSizeX, config.meshSizeY, config.meshSizeZ);
    //GrainGrowth* modelCA = new GrainGrowth(meshInit);
    //modelCA->setRandomInitialConditions(config.CA_noNucleons);
    //endTime = std::chrono::system_clock::now();
    //elapsedTimeInitCA = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    //startTime = std::chrono::system_clock::now();
    //Mesh* resultMesh = modelCA->runSimulationCA(config.boundaryConditionType, config.neighbourhoodType, argc, argv);
    //endTime = std::chrono::system_clock::now();
    //elapsedTimeSimulationCA = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    //startTime = std::chrono::system_clock::now();
    //resultMesh->saveStateToVTK("simulationResultCA_" + config.simulationName + ".vtk");
    //endTime = std::chrono::system_clock::now();
    //elapsedTimeWritingResultToFileCA = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    //delete modelCA;
    //delete meshInit;
    //delete resultMesh;

    //std::string timeMeasurementsFilename = "timeMeasurements_" + config.simulationName + ".txt";
    //std::ofstream timeMeasurementsFile(timeMeasurementsFilename);
    //timeMeasurementsFile << "Time of initialization CA: " << elapsedTimeInitCA.count() << " microseconds\n";
    //timeMeasurementsFile << "Time of simulation CA: " << elapsedTimeSimulationCA.count() << " microseconds\n";
    //timeMeasurementsFile << "Time of writing the result to a file CA: " << elapsedTimeWritingResultToFileCA.count() << " microseconds";
    //timeMeasurementsFile.close();
    //test dla CA ^



    //test dla MC v
    Config config("test2D", 1000, 1000, 1, boundaryCondition::PERIODIC, neighbourhood::VON_NEUMANN, 100, 50);
    //Config config("test2D", 1000, 1000, 1, boundaryCondition::PERIODIC, neighbourhood::MOORE, 100, 50);
    //Config config("test3D", 100, 100, 100, boundaryCondition::PERIODIC, neighbourhood::VON_NEUMANN, 100, 50);
    //Config config("test3D", 100, 100, 100, boundaryCondition::PERIODIC, neighbourhood::MOORE, 100, 50);
    std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
    std::chrono::microseconds elapsedTimeInitMC, elapsedTimeSimulationMC, elapsedTimeWritingResultToFileMC;

    startTime = std::chrono::system_clock::now();
    Mesh* meshInit = new Mesh(config.meshSizeX, config.meshSizeY, config.meshSizeZ);
    GrainGrowth* modelMC = new GrainGrowth(meshInit);
    modelMC->setRandomNotZeroStateInEveryCellInMesh();
    endTime = std::chrono::system_clock::now();
    elapsedTimeInitMC = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    startTime = std::chrono::system_clock::now();
    Mesh* resultMesh = modelMC->runSimulationMC(config.boundaryConditionType, config.neighbourhoodType, config.MC_noSteps, argc, argv);
    endTime = std::chrono::system_clock::now();
    elapsedTimeSimulationMC = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    startTime = std::chrono::system_clock::now();
    resultMesh->saveStateToVTK("simulationResultMC_" + config.simulationName + ".vtk");
    endTime = std::chrono::system_clock::now();
    elapsedTimeWritingResultToFileMC = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    delete modelMC;
    delete meshInit;
    delete resultMesh;

    std::string timeMeasurementsFilename = "timeMeasurements_" + config.simulationName + ".txt";
    std::ofstream timeMeasurementsFile(timeMeasurementsFilename);
    timeMeasurementsFile << "Time of initialization MC: " << elapsedTimeInitMC.count() << " microseconds\n";
    timeMeasurementsFile << "Time of simulation MC: " << elapsedTimeSimulationMC.count() << " microseconds\n";
    timeMeasurementsFile << "Time of writing the result to a file MC: " << elapsedTimeWritingResultToFileMC.count() << " microseconds";
    timeMeasurementsFile.close();
    //test dla MC ^
}