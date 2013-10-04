#include "ResizeSenario.h"

#include <algorithm>
#include "TypeRegistry.h"
#include "helpers.h"
#include "rand.h"

REGISTER_SIMULATION(ResizeSenario, "resize");

const static double factor = 1;

void ResizeSenario::initialize()
{
	auto pd = physicsDimensions();
	int num_objects = 80;
	maxProgress(80);
	b2Vec2 vanish(50, 20);
	for(int i = 0; i < num_objects; ++i)
	{
		polygonDef def;
		def.bodyDef.position = b2Vec2(randuniform(10, 90), randuniform(30, 70));
		def.bodyDef.angle = 0;
		def.fixtureDef.filter.groupIndex = -1;
		def.fixtureDef.density = 1;
		
		auto x = (vanish - def.bodyDef.position).Length() / 100;
		def.shape.SetAsBox(1*x, 5*x);
		boxes.push_back(make_shape(world(), def));
		setProgress(i);
	}
	std::sort(boxes.begin(), boxes.end(), [](b2Body* lhs, b2Body* rhs)
	{
		return lhs->GetPosition().y < rhs->GetPosition().y;
	});
}

void ResizeSenario::render()
{
	drawBodies(boxes, sf::Color::Black, sf::Color::White, -2);
}