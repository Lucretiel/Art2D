#pragma once
#include "Simulation.h"

class ResizeSenario: public Simulation
{
	std::vector<b2Body*> boxes;

	void initialize();
	void render();

	std::string title() {return "Size Change";}
};

