#pragma once
#include "Simulation.h"
#include "3DPlanes.h"

class Trivial3DSenario : public Simulation
{
public:
	Trivial3DSenario()
		:floorWorld(b2Vec2(0, 0))
		,sideWorld(b2Vec2(0, 0))
		,backWorld(b2Vec2(0, 0))
	{}

private:
	InitParams prepare()
	{
		InitParams params;
		params.videoMode = sf::VideoMode(1000, 1000);
		return params;
	}
	void initialize();
	void think(unsigned long frame);
	void render();
	std::string title() {return "Trivial 3D";}

	b2World floorWorld;
	b2World sideWorld;
	b2World backWorld;

	VanishPlane floorPlane;
	VanishPlane sidePlane;
	VanishPlane backPlane;

	std::vector<b2Body*> floorBodies;
	std::vector<b2Body*> sideBodies;
	std::vector<b2Body*> backBodies;
};

