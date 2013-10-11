#include "Height.h"

#include "helpers.h"
#include "rand.h"

REGISTER_SIMULATION(Height, "height");

InitParams Height::prepare()
{
	InitParams x;
	x.videoMode = sf::VideoMode(1000, 800 * 3);
	return x;
}

b2Vec2 random_region(int region)
{
	auto p = random_margin(b2Vec2(100, 80), 10);
	p.y += region * 80;
	return p;
}

void Height::initialize()
{

	for(int c = 0; c < clusters.size(); ++c)
	{
		int num_fragments;
		float rel_size;
		float density;

		switch(c)
		{
		case 0:
			num_fragments = 40;
			rel_size = 0.25;
			density = 1;
			break;
		case 1:
			num_fragments = 20;
			rel_size = 0.5;
			density = 0.5;
			break;
		case 2:
			num_fragments = 10;
			rel_size = 1;
			density = .25;
			break;
		}

		///////////////////////////////////////////////////////////////////

		gravities[c] = random_region(c);
		polygonDef def;
		def.bodyDef.position = gravities[c];
		def.bodyDef.type = b2_staticBody;
		def.fixtureDef.density = 1;
		def.shape.SetAsBox(.5*rel_size, .5*rel_size);
		make_shape(world(), def);

		//////////////////////////////////////////////////////////////////////

		for(int i = 0; i < num_fragments; ++i)
		{
			polygonDef def;
			def.bodyDef.angle = to_radians(randuniform(0, 360));
			def.bodyDef.position = random_region(c);
			def.bodyDef.type = b2_dynamicBody;
			def.shape.SetAsBox(1 * rel_size, 5 * rel_size);
			def.fixtureDef.density = density;
			def.fixtureDef.friction = .5;
			clusters[c].push_back(make_shape(world(), def));
		}
	}
}

const double gravity_force = 100;
b2Vec2 gravity(b2Vec2 center, b2Vec2 object)
{
	auto direction = center - object;
	auto length = 1/direction.Length();
	length *= gravity_force;
	direction.Normalize();
	direction *= length;
	return direction;
}

void Height::think(unsigned long frame)
{
	for(int c = 0; c < clusters.size(); ++c)
	{
		for(auto it = clusters[c].begin(); it != clusters[c].end(); ++it)
		{
			(*it)->ApplyForce(gravity(gravities[c], (*it)->GetPosition()), (*it)->GetPosition());
		}
	}
}

void Height::render()
{
	for(auto it = clusters.begin(); it != clusters.end(); ++it)
	{
		drawBodies(*it, sf::Color::Black, sf::Color::White, -.5);
	}
}