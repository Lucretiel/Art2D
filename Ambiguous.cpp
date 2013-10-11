#include "Ambiguous.h"

REGISTER_SIMULATION(Ambiguous, "ambiguous");

#include "helpers.h"
#include "rand.h"

void Ambiguous::initialize()
{
	const int num_sides = 2;
	const int num_floors = 2;
	const int num_bodies = 600;
	maxProgress(num_bodies + num_floors + num_sides);
	bodies.reserve(num_bodies);

	auto w_dim = windowDimensions();
	for(int i = 0; i < num_sides; ++i)
	{
		planes.push_back(VanishPlane::sidePlane(
			sf::Vector2f(
				randuniform(w_dim.x * -0.5, w_dim.x * 1.5),
				randuniform(w_dim.y * -0.5, w_dim.y * 1.5)),
			randuniform(0, w_dim.x)));
		addProgress(1);
	}

	for(int i = 0; i < num_floors; ++i)
	{
		planes.push_back(VanishPlane::floorPlane(
			sf::Vector2f(
				randuniform(0, w_dim.x),
				randuniform(0, w_dim.y)),
			randuniform(0, w_dim.y)));
		addProgress(1);
	}
	
	auto p_dim = physicsDimensions();
	for (int i = 0; i < num_bodies; ++i)
	{
		polygonDef def;
		def.bodyDef.position.Set(
			randuniform(0, p_dim.x * 2),
			randuniform(0, p_dim.y * 2));
		def.bodyDef.angle = to_radians(randuniform(0, 360));
		def.bodyDef.type = b2_staticBody;
		def.shape.SetAsBox(1.5, 7.5);
		def.fixtureDef.density = 1;
		def.bodyDef.userData = &planes[randint(0, planes.size()-1)];
		bodies.push_back(make_shape(world(), def));
		addProgress(1);
	}
}

void Ambiguous::render()
{
	for(auto it = bodies.begin(); it != bodies.end(); ++it)
		drawBody(**it, *static_cast<VanishPlane*>((*it)->GetUserData()), sf::Color::Black, sf::Color::White, -1);
}