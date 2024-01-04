//Jakub Foryś   AGH   2023
#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <omp.h>
#include "enums.cpp"
#include "Config.h"
#include "Mesh.h"
#include "GrainGrowth.h"
#include "TestParallelism.h"

int main(int argc, char** argv)
{
    omp_set_num_threads(6);

    bool PERFORM_CELLULAR_AUTOMATA_GRAIN_GROWTH = false;
    bool PERFORM_MONTE_CARLO_GRAIN_GROWTH = false;

    //zaczynamy od 1, bo pierwszy argument to nazwa programu
    for (int i = 1; i < argc; i++) {
        
        Config config(static_cast<std::string>(argv[i]));
        if (config.CA_noNucleons > 0) {
            PERFORM_CELLULAR_AUTOMATA_GRAIN_GROWTH = true;
        }
        if (config.MC_noSteps > 0) {
            PERFORM_MONTE_CARLO_GRAIN_GROWTH = true;
        }

        std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
        std::chrono::microseconds elapsedTimeInitCA, elapsedTimeSimulationCA, elapsedTimeWritingResultToFileCA;
        std::chrono::microseconds elapsedTimeInitMC, elapsedTimeSimulationMC, elapsedTimeWritingResultToFileMC;

        if (PERFORM_CELLULAR_AUTOMATA_GRAIN_GROWTH) {
            startTime = std::chrono::system_clock::now();
            Mesh* meshCA = new Mesh(config.meshSizeX, config.meshSizeY, config.meshSizeZ);
            GrainGrowth* modelCA = new GrainGrowth(meshCA);
            modelCA->setRandomInitialConditions(config.CA_noNucleons);
            endTime = std::chrono::system_clock::now();
            elapsedTimeInitCA = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

            startTime = std::chrono::system_clock::now();
            modelCA->runSimulationCA(config.boundaryConditionType, config.neighbourhoodType, config.simulationName);
            endTime = std::chrono::system_clock::now();
            elapsedTimeSimulationCA = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

            startTime = std::chrono::system_clock::now();
            meshCA->saveStateToVTK("simulationResultCA_" + config.simulationName + ".vtk");
            endTime = std::chrono::system_clock::now();
            elapsedTimeWritingResultToFileCA = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

            delete modelCA;
            delete meshCA;
        }


        if (PERFORM_MONTE_CARLO_GRAIN_GROWTH) {
            startTime = std::chrono::system_clock::now();
            Mesh* meshMC = new Mesh(config.meshSizeX, config.meshSizeY, config.meshSizeZ);
            GrainGrowth* modelMC = new GrainGrowth(meshMC);
            modelMC->setRandomNotZeroStateInEveryCellInMesh();
            endTime = std::chrono::system_clock::now();
            elapsedTimeInitMC = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

            startTime = std::chrono::system_clock::now();
            modelMC->runSimulationMC(config.boundaryConditionType, config.neighbourhoodType, config.MC_noSteps, config.simulationName);
            endTime = std::chrono::system_clock::now();
            elapsedTimeSimulationMC = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

            startTime = std::chrono::system_clock::now();
            meshMC->saveStateToVTK("simulationResultMC_" + config.simulationName + ".vtk");
            endTime = std::chrono::system_clock::now();
            elapsedTimeWritingResultToFileMC = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

            delete modelMC;
            delete meshMC;
        }


        std::string timeMeasurementsFilename = "timeMeasurements_" + config.simulationName + ".txt";
        std::ofstream timeMeasurementsFile(timeMeasurementsFilename);
        if (PERFORM_CELLULAR_AUTOMATA_GRAIN_GROWTH) {
            timeMeasurementsFile << "Time of initialization CA: " << elapsedTimeInitCA.count() << " microseconds\n";
            timeMeasurementsFile << "Time of simulation CA: " << elapsedTimeSimulationCA.count() << " microseconds\n";
            timeMeasurementsFile << "Time of writing the result to a file CA: " << elapsedTimeWritingResultToFileCA.count() << " microseconds";
        }
        if (PERFORM_MONTE_CARLO_GRAIN_GROWTH) {
            timeMeasurementsFile << "Time of initialization MC: " << elapsedTimeInitMC.count() << " microseconds\n";
            timeMeasurementsFile << "Time of simulation MC: " << elapsedTimeSimulationMC.count() << " microseconds\n";
            timeMeasurementsFile << "Time of writing the result to a file MC: " << elapsedTimeWritingResultToFileMC.count() << " microseconds";
        }
        timeMeasurementsFile.close();

        PERFORM_CELLULAR_AUTOMATA_GRAIN_GROWTH = false;
        PERFORM_MONTE_CARLO_GRAIN_GROWTH = false;
    }
}