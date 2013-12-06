#pragma once

#include "Simulation.h"
#include "helpers.h"
#include "rand.h"

SIM(Implied, "implied")
{
	b2Body* circle;
	b2Body* circle2;
	std::list<b2Body*> bodies;

	b2Vec2 center;
	b2Vec2 dimensions;

	std::string title() override {return "Implied Line";}

	void initialize() override
	{
		center = physicsHalfDimensions();
		dimensions = physicsDimensions();
		world().SetGravity(b2Vec2(0, 20));

		circleDef def;
		def.fixtureDef.isSensor = true;
		def.bodyDef.position.Set(-5, 75);
		const float radius = 90;
		def.shape.m_radius = radius;
		def.shape.m_p.Set(-radius, -radius);
		circle = make_shape(world(), def);

		const float radius2 = 20;
		def.shape.m_radius = radius2;
		def.shape.m_p.Set(-radius2, -radius2);
		circle2 = make_shape(world(), def);
	}

	void think(unsigned long frame) override
	{
		auto pred = [this](b2Body* body)
		{
			return body->GetPosition().y > (dimensions.y * 1.5);
		};
		bodies.remove_if(pred);

		if(frame % 3 == 0)
		{
			polygonDef def;
			def.bodyDef.angle = to_radians(randuniform(0, 360));
			def.bodyDef.position.Set(center.x/2, -5);
			def.bodyDef.linearVelocity.Set(
				randcentered(20, 5),
				randcentered(-10, 5));
			def.bodyDef.angularVelocity = to_radians(randcentered(0, 10));
			def.bodyDef.type = b2_dynamicBody;
			def.fixtureDef.isSensor = true;
			def.fixtureDef.density = 1;
			def.shape.SetAsBox(1, 5);
			bodies.push_front(make_shape(world(), def));
		}
	}

	void render() override
	{
		drawBodies(bodies, sf::Color::Black, sf::Color::White, -1);
		drawBody(*circle, sf::Color::White, sf::Color::Black, 0);
		drawBody(*circle2);
	}

};
