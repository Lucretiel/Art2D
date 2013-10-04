#pragma once

#include <list>
#include "Simulation.h"
#include "helpers.h"

const float age_range = 200;

float convert(int age)
{
	return (age > age_range ? 0 : age_range - age)/age_range;
}

struct ThrownBar
{
	b2Body* body;
	unsigned long age;

	ThrownBar(b2World& world, const polygonDef& def)
		:body(make_shape(world, def))
		,age(0)
	{}

	sf::Color color()
	{
		int x = convert(age)*255;
		return sf::Color(x, x, x);
	}
};

class Gradiant: public Simulation
{
	std::list<ThrownBar> boxes;
	
	void initialize();
	void render();
	void think(unsigned long fram);
	std::string title() {return "gradient";}
};