#pragma once
#include <list>
#include "Simulation.h"

class OpenSpace: public Simulation
{
	InitParams prepare();
	void initialize();
	void render();
	std::string title() {return "Open Space";}

	std::list<b2Body*> bodies;

	b2Body* upper;
	b2Body* middle;

	b2Body* makePlatform(float ycenter, float angle);
};

