#include "rand.h"

#include <random>
#include <ctime>

std::default_random_engine engine;

int seed()
{
	engine.seed(std::time(0));
	return 0;
}

int randint(int min, int max)
{
	return std::uniform_int_distribution<int>(min, max)(engine);
}

float randuniform(float min, float max)
{
	return std::uniform_real_distribution<float>(min, max)(engine);
}

float randcentered(float center, float extent)
{
	return center + randuniform(-extent, extent);
}

const static int initial_seed(seed());