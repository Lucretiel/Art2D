#pragma once

#include <cmath>
#include "Simulation.h"
#include "helpers.h"
#include "rand.h"

SIM(Impersonal, "impersonal")
{
	std::string title() override {return "Impersonal";}
	std::vector<b2Body*> bodies;

	void initialize() override
	{	
		const unsigned numBodies = 1000;
		maxProgress(numBodies);
		auto dimensions = physicsDimensions();
		auto center = physicsHalfDimensions();
		bodies.reserve(numBodies);
		for(int i = 0; i < numBodies; ++i)
		{
			polygonDef def;
			def.bodyDef.position = random_margin(dimensions, -5);
			def.bodyDef.fixedRotation = true;
			def.bodyDef.userData = randint(0,2) ? this : nullptr;
			auto distance = std::abs(center.x - def.bodyDef.position.x);
			float amount = distance / center.x * 100;
			def.bodyDef.angle = to_radians(randcentered(0, amount));
			def.shape.SetAsBox(1, 5);
			def.fixtureDef.isSensor = true;
			bodies.push_back(make_shape(world(), def));
			addProgress(1);
		}
		background() = sf::Color::Black;
	}

	void render() override
	{
		for(b2Body* body : bodies)
		{
			if(body->GetUserData())
				drawBody(*body, sf::Color::Black, sf::Color::White, 1);
			else
				drawBody(*body, sf::Color::White, sf::Color::Black, 1);
		}

	}

};