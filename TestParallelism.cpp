#include "TestParallelism.h"

void TestParallelism::CA()
{
    std::vector<int> noThreads = { 2, 3, 4, 5, 6, 8, 10, 12 };
    int noThreadsSize = noThreads.size();
    std::vector<sizes> meshSizes = {
        {1000, 1000, 1},
        {4000, 250, 1},
        {250, 4000, 1},
        {100, 100, 100},
        {400, 25, 100},
        {25, 400, 100}
    };
    int meshSizesSize = meshSizes.size();
    std::vector<neighbourhood> neighbourhoods = { neighbourhood::VON_NEUMANN, neighbourhood::MOORE };
    int neighbourhoodsSize = neighbourhoods.size();

    //|noSizes|noNoThreads|noNeighbourhood|noResult|
    std::map<int, std::map<int, std::map<int, std::map<int, int>>>> allResults;

    for (int t = 0; t < noThreadsSize; t++) {
        omp_set_num_threads(noThreads[t]);
        for (int m = 0; m < meshSizesSize; m++) {
            for (int n = 0; n < neighbourhoodsSize; n++) {
                std::string nString;
                if (neighbourhoods[n] == neighbourhood::VON_NEUMANN) nString = "vN";
                else if (neighbourhoods[n] == neighbourhood::MOORE) nString = "M";
                std::string simulationName = std::to_string(noThreads[t]) + "watki_100z_" + nString + "_" +
                    std::to_string(meshSizes[m].sX) + "x" + std::to_string(meshSizes[m].sY) + "x" + std::to_string(meshSizes[m].sZ);

                Config config(simulationName, meshSizes[m].sX, meshSizes[m].sY, meshSizes[m].sZ,
                    boundaryCondition::PERIODIC, neighbourhoods[n], 100, 0);

                //,,standard" v
                std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
                std::chrono::microseconds elapsedTimeInitCA, elapsedTimeSimulationCA, elapsedTimeWritingResultToFileCA;

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

                std::string timeMeasurementsFilename = "timeMeasurements_" + config.simulationName + ".txt";
                std::ofstream timeMeasurementsFile(timeMeasurementsFilename);
                timeMeasurementsFile << "Time of initialization CA: " << elapsedTimeInitCA.count() << " microseconds\n";
                timeMeasurementsFile << "Time of simulation CA: " << elapsedTimeSimulationCA.count() << " microseconds\n";
                timeMeasurementsFile << "Time of writing the result to a file CA: " << elapsedTimeWritingResultToFileCA.count() << " microseconds";
                //,,standard" ^

                allResults[m][t][n][0] = elapsedTimeInitCA.count();
                allResults[m][t][n][1] = elapsedTimeSimulationCA.count();
                allResults[m][t][n][2] = elapsedTimeWritingResultToFileCA.count();
            }
        }
    }

    std::string resultsFilename = "CA_results.csv";
    std::ofstream resultsFile(resultsFilename);
    for (int m = 0; m < meshSizesSize; m++) {
        for (int t = 0; t < noThreadsSize; t++)
            for (int n = 0; n < neighbourhoodsSize; n++)
                resultsFile << allResults[m][t][n][0] << ";";
        resultsFile << "\n";
        for (int t = 0; t < noThreadsSize; t++)
            for (int n = 0; n < neighbourhoodsSize; n++)
                resultsFile << allResults[m][t][n][1] << ";";
        resultsFile << "\n";
        for (int t = 0; t < noThreadsSize; t++)
            for (int n = 0; n < neighbourhoodsSize; n++)
                resultsFile << allResults[m][t][n][2] << ";";
        resultsFile << "\n";
    }
    resultsFile << "\n\n";
    for (int n = 0; n < neighbourhoodsSize; n++) {
        for (int m = 0; m < meshSizesSize; m++) {
            for (int t = 0; t < noThreadsSize; t++)
                resultsFile << allResults[m][t][n][1] << ";";
            resultsFile << "\n";
        }
    }
    resultsFile.close();
}

void TestParallelism::MC()
{
    std::vector<int> noThreads = { 2, 3, 4, 5, 6, 8, 10, 12 };
    int noThreadsSize = noThreads.size();
    std::vector<sizes> meshSizes = {
        {1000, 1000, 1},
        {4000, 250, 1},
        {250, 4000, 1},
        {100, 100, 100},
        {400, 25, 100},
        {25, 400, 100}
    };
    int meshSizesSize = meshSizes.size();
    std::vector<neighbourhood> neighbourhoods = { neighbourhood::VON_NEUMANN, neighbourhood::MOORE };
    int neighbourhoodsSize = neighbourhoods.size();

    //|noSizes|noNoThreads|noNeighbourhood|noResult|
    std::map<int, std::map<int, std::map<int, std::map<int, int>>>> allResults;

    for (int t = 0; t < noThreadsSize; t++) {
        omp_set_num_threads(noThreads[t]);
        for (int m = 0; m < meshSizesSize; m++) {
            for (int n = 0; n < neighbourhoodsSize; n++) {
                std::string nString;
                if (neighbourhoods[n] == neighbourhood::VON_NEUMANN) nString = "vN";
                else if (neighbourhoods[n] == neighbourhood::MOORE) nString = "M";
                std::string simulationName = std::to_string(noThreads[t]) + "watki_50k_" + nString + "_" +
                    std::to_string(meshSizes[m].sX) + "x" + std::to_string(meshSizes[m].sY) + "x" + std::to_string(meshSizes[m].sZ);

                Config config(simulationName, meshSizes[m].sX, meshSizes[m].sY, meshSizes[m].sZ,
                    boundaryCondition::PERIODIC, neighbourhoods[n], 0, 50);

                //,,standard" v
                std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
                std::chrono::microseconds elapsedTimeInitMC, elapsedTimeSimulationMC, elapsedTimeWritingResultToFileMC;

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

                std::string timeMeasurementsFilename = "timeMeasurements_" + config.simulationName + ".txt";
                std::ofstream timeMeasurementsFile(timeMeasurementsFilename);
                timeMeasurementsFile << "Time of initialization MC: " << elapsedTimeInitMC.count() << " microseconds\n";
                timeMeasurementsFile << "Time of simulation MC: " << elapsedTimeSimulationMC.count() << " microseconds\n";
                timeMeasurementsFile << "Time of writing the result to a file MC: " << elapsedTimeWritingResultToFileMC.count() << " microseconds";
                //,,standard" ^

                allResults[m][t][n][0] = elapsedTimeInitMC.count();
                allResults[m][t][n][1] = elapsedTimeSimulationMC.count();
                allResults[m][t][n][2] = elapsedTimeWritingResultToFileMC.count();
            }
        }
    }

    std::string resultsFilename = "MC_results.csv";
    std::ofstream resultsFile(resultsFilename);
    for (int m = 0; m < meshSizesSize; m++) {
        for (int t = 0; t < noThreadsSize; t++)
            for (int n = 0; n < neighbourhoodsSize; n++)
                resultsFile << allResults[m][t][n][0] << ";";
        resultsFile << "\n";
        for (int t = 0; t < noThreadsSize; t++)
            for (int n = 0; n < neighbourhoodsSize; n++)
                resultsFile << allResults[m][t][n][1] << ";";
        resultsFile << "\n";
        for (int t = 0; t < noThreadsSize; t++)
            for (int n = 0; n < neighbourhoodsSize; n++)
                resultsFile << allResults[m][t][n][2] << ";";
        resultsFile << "\n";
    }
    resultsFile << "\n\n";
    for (int n = 0; n < neighbourhoodsSize; n++) {
        for (int m = 0; m < meshSizesSize; m++) {
            for (int t = 0; t < noThreadsSize; t++)
                resultsFile << allResults[m][t][n][1] << ";";
            resultsFile << "\n";
        }
    }
    resultsFile.close();
}