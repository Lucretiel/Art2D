#include "Gradiant.h"

#include <algorithm>

#include "helpers.h"
#include "rand.h"

REGISTER_SIMULATION(Gradiant, "gradient");

void Gradiant::initialize()
{
	maxProgress(5);
	world().SetGravity(b2Vec2(0, 20));
	addProgress(1);
	setBottomEdge(false);
	addProgress(1);
	setLeftEdge(false);
	addProgress(1);
	setRightEdge(false);
	addProgress(1);
	setTopEdge(false);
	addProgress(1);
}

void Gradiant::think(unsigned long frame)
{
	for(auto it = boxes.begin(); it != boxes.end(); ++it)
	{
		if(it->body->GetPosition().y > 1.5*physicsDimensions().y)
			it->body = nullptr;
		else
			++it->age;
	}

	boxes.remove_if([this](ThrownBar& x)
	{
		return x.body == nullptr;
	});
	

	if(frame % 5 == 0)
	{
		polygonDef def;
		def.bodyDef.position = random_centered(b2Vec2(20, 40), 3);
		def.bodyDef.angle = to_radians(randuniform(0, 360));
		def.bodyDef.angularVelocity = to_radians(50);
		def.bodyDef.angularDamping = 0.1;
		def.bodyDef.type = b2_dynamicBody;
		def.bodyDef.linearVelocity = random_centered(b2Vec2(10, -30), 4);
		def.fixtureDef.density = 1;
		def.fixtureDef.filter.groupIndex = -1;
		def.shape.SetAsBox(1, 5);
		boxes.push_front(ThrownBar(world(), def));
	}
}

void Gradiant::render()
{
	for(auto it = boxes.begin(); it != boxes.end(); ++it)
	{
		drawBody(*it->body, it->color(), sf::Color::White, -.5);
	}
}