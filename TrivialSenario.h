#pragma once
#include "simulation.h"

class TrivialSenario: public Simulation
{
private:
	b2Body* box;

	void initialize();
	void render();
	std::string title() {return "Trivial";}
};

