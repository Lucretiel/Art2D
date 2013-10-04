#pragma once
#include "Simulation.h"
#include "3DPlanes.h"
#include <list>


class Flat : public Simulation
{
	void initialize();
	void render();
	void think(unsigned long frame);
	std::string title() {return "Flat";}

	//std::list<b2Body*> bodies1;
	//std::list<b2Body*> bodies2;
	//std::list<b2Body*> bodies3;
	EnclosedWorld back1;
	EnclosedWorld back2;
	EnclosedWorld back3;

public:
	Flat()
		:back1(b2Vec2(0, 20), b2Vec2(100, 80))
		,back2(b2Vec2(0, 20), b2Vec2(100, 80))
		,back3(b2Vec2(0, 20), b2Vec2(100, 80))
	{}
};

