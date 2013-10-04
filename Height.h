#pragma once
#include "Simulation.h"

#include <array>
class Height: public Simulation
{
	typedef std::vector<b2Body*> BodyVec;
	typedef std::array<BodyVec, 3> Clusters;

	Clusters clusters;
	std::array<b2Vec2, 3> gravities;

	InitParams prepare();
	void initialize();
	void render();
	void think(unsigned long frame);
	std::string title() {return "height";}
};

