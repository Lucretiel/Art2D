#pragma once
#include "Simulation.h"
#include "3DPlanes.h"

class Ambiguous : public Simulation
{
	std::string title() {return "Ambiguous";}
	void initialize();
	void render();

	std::vector<b2Body*> bodies;

	std::vector<VanishPlane> planes;
};

