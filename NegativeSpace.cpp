#include "Simulation.h"

#include "helpers.h"
#include "rand.h"

#include <list>

class NegativeSpace : public Simulation
{
	b2Body* big1;
	b2Body* big2;
	std::list<b2Body*> bodies;
	std::string title() override {return "Negative Space";}

	void initialize() override
	{
		setAllEdges(true);
		setTopEdge(false);
		world().SetGravity(b2Vec2(0, 20));

		auto center = physicsHalfDimensions();

		polygonDef def;
		def.bodyDef.position = center;
		def.bodyDef.angle = to_radians(-30);
		def.bodyDef.type = b2_staticBody;
		def.bodyDef.fixedRotation = true;
		def.fixtureDef.density = 1;
		def.fixtureDef.isSensor = true;
		def.shape.SetAsBox(8, 40);
		big1 = make_shape(world(), def);

		def.bodyDef.position = center + b2Vec2(-3, 3);
		def.bodyDef.angle = to_radians(40);
		big2 = make_shape(world(), def);

		auto& xf = big1->GetTransform();
		
		shapeDef<b2EdgeShape> edgedef;
		edgedef.bodyDef.position.SetZero();
		edgedef.bodyDef.type = b2_staticBody;
		edgedef.bodyDef.fixedRotation = true;
		edgedef.fixtureDef.density = 1;
		edgedef.fixtureDef.friction = 0;

		for(int i = 0; i < 4; ++i)
		{
			if(i != 0)
			{
				edgedef.shape.Set(
					b2Mul(xf, def.shape.GetVertex(i)),
					b2Mul(xf, def.shape.GetVertex((i+1)%4)));
				make_shape(world(), edgedef);
			}
		}

	}

	void think(unsigned long frame) override
	{
		if(frame % 15 == 0)
		{
			polygonDef def;
			def.bodyDef.position.Set(
				randcentered(30, 5),
				randcentered(0, 5));
			def.bodyDef.angle = randuniform(0, 360);
			def.bodyDef.type = b2_dynamicBody;
			def.bodyDef.linearVelocity.Set(20, 20);
			def.fixtureDef.density = 1;
			def.fixtureDef.friction = 1;
			def.shape.SetAsBox(1, 5);
			bodies.push_back(make_shape(world(), def));
		}
	}

	void render() override
	{
		drawBody(*big1, sf::Color::Black, sf::Color::Transparent);
		drawBody(*big2, sf::Color::Black, sf::Color::Transparent);
		drawBodies(bodies, sf::Color::White, sf::Color::Transparent, -.5);
	}
};


REGISTER_SIMULATION(NegativeSpace, "negative");
