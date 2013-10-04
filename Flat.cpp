#include "Flat.h"

#include "helpers.h"
#include "rand.h"

#include "TypeRegistry.h"
REGISTER_SIMULATION(Flat, "flat");

sf::Vector2f noShift(const sf::Vector2f& p)
{
	return p;
}
sf::Vector2f shiftUpLeft(const sf::Vector2f& p)
{
	return sf::Vector2f(p.x - 100, p.y - 100);
}

sf::Vector2f shiftUpLeft2(const sf::Vector2f& p)
{
	return shiftUpLeft(shiftUpLeft(p));
}

void Flat::initialize()
{
	const int num_things = 75;

	maxProgress(num_things * 3);

	for(int i = 0; i < num_things; ++i)
	{
		polygonDef def;
		def.bodyDef.position.Set(randuniform(50, 100), randuniform(0, 80));
		def.bodyDef.angle = 0;
		def.bodyDef.fixedRotation = true;
		def.bodyDef.type = b2_dynamicBody;
		def.shape.SetAsBox(1*1.2, 5*1.2);
		def.fixtureDef.density = 1;
		(make_shape(back1, def));
		addProgress(1);
	}
	for(int i = 0; i < num_things; ++i)
	{
		polygonDef def;
		def.bodyDef.position.Set(randuniform(50, 100), randuniform(0, 80));
		def.bodyDef.angle = to_radians(randuniform(0, 360));
		def.bodyDef.type = b2_dynamicBody;
		def.shape.SetAsBox(1, 5);
		def.fixtureDef.density = 1;
		(make_shape(back2, def));
		addProgress(1);
	}
	for(int i = 0; i < num_things; ++i)
	{
		polygonDef def;
		def.bodyDef.position.Set(randuniform(50, 100), randuniform(0, 80));
		def.bodyDef.angle = 0;
		def.bodyDef.fixedRotation = true;
		def.bodyDef.type = b2_dynamicBody;
		def.shape.SetAsBox(1*0.8, 5*0.8);
		def.fixtureDef.density = 1;
		(make_shape(back3, def));
		addProgress(1);
	}
}

void Flat::think(unsigned long frame)
{
	back1.Step(
		simParams().inv_frame_rate,
		simParams().velocityIterations,
		simParams().positionIterations);
	back2.Step(
		simParams().inv_frame_rate,
		simParams().velocityIterations,
		simParams().positionIterations);
	back3.Step(
		simParams().inv_frame_rate,
		simParams().velocityIterations,
		simParams().positionIterations);
}

void Flat::render()
{
	drawWorld(back3, &shiftUpLeft2, sf::Color::Black, sf::Color::White, -1);
	drawWorld(back2, &shiftUpLeft, sf::Color::Black, sf::Color::White, -1);
	drawWorld(back1, &noShift, sf::Color::Black, sf::Color::White, -1);
}
