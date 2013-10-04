#pragma once
#include "Simulation.h"
#include "3DPlanes.h"

class VanishingPoint : public Simulation
{
public:
	VanishingPoint();

private:
	InitParams prepare();
	void initialize();
	void think(unsigned long frame);
	void render();

	std::string title() {return "Vanishing Point";}

	EnclosedWorld goodStack; //Center front
	EnclosedWorld badStack; //Center back
	EnclosedWorld fallen; //floor
	b2World verticals;
};

