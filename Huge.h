#include "Simulation.h"
#include "helpers.h"
#include "rand.h"

class Huge: public Simulation
{
	InitParams prepare() override
	{
		InitParams params;
		params.videoMode = sf::VideoMode(3000, 3000);
		params.settings.antialiasingLevel = 16;
		return params;
	}
};