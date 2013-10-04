#pragma once
#include "Simulation.h"

class Isometric: public Simulation
{
	void initialize();
	void render();

	std::string title() {return "Isometric";}

	std::vector<sf::ConvexShape> shapes;
};

