#pragma once

#include <vector>
#include <array>
#include "Simulation.h"

class LayerSenario: public Simulation
{
	void initialize();
	void render();
	std::string title() {return "Layers";}

	typedef std::vector<b2Body*> BodyVec;
	typedef std::array<BodyVec, 3> Layers;
	Layers layers;
};

