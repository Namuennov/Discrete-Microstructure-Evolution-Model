#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <omp.h>
#include "enums.cpp"
#include "Config.h"
#include "Mesh.h"
#include "GrainGrowth.h"

struct sizes
{
    int sX;
    int sY;
    int sZ;
};

class TestParallelism
{
public:
	static void CA();
	static void MC();
};