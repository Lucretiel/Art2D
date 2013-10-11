#include "EmptySpace.h"

#include "helpers.h"
#include "rand.h"

#include <iostream>
REGISTER_SIMULATION(EmptySpace, "empty");

void EmptySpace::initialize()
{
	setTopEdge(false);
	setRightEdge(false);
	setBottomEdge(false);
	setLeftEdge(false);

	world().SetGravity(b2Vec2(0, 20));

	auto pd = physicsDimensions();

	initial_position.Set(pd.x * 2 / 3, -20);
	initial_velocity.Set(0, 0);
	position_noise = 10;
	velocity_noise = 10;
}

void EmptySpace::render()
{
	drawBodies(bodies, sf::Color::Black, sf::Color::White, -2);
}

void EmptySpace::handle_event(sf::Event& event)
{
	default_event(event);
	auto pd = physicsDimensions();

	switch(event.type)
	{
	case sf::Event::KeyPressed:
		switch(event.key.code)
		{
		case sf::Keyboard::Space:
			{
				polygonDef def;
				def.bodyDef.position.Set(randuniform(-10, pd.x + 10), -10);
				def.bodyDef.angle = to_radians(randuniform(-30, 30));
				def.bodyDef.type = b2_dynamicBody;
				def.shape.SetAsBox(1, 5);
				def.fixtureDef.density = 1;
			
				bodies.push_back(make_shape(world(), def));
			}
			break;
		case sf::Keyboard::K:
			bodies.pop_front();
			break;
		}
	}
}

void EmptySpace::think(unsigned long frame)
{
	filter_bodies();
}

void EmptySpace::filter_bodies()
{
	auto height = physicsDimensions().y+5;

	for(auto it = bodies.begin(); it != bodies.end(); ++it)
	{
		float dist = (height - (*it)->GetPosition().y)/10;
		if(dist <= 1)
			if(randuniform(0, 1) > dist)
				(*it)->SetActive(false);
	}
}