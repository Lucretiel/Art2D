#include "Simulation.h"
#include "rand.h"
#include "helpers.h"

class ClosedSpace : public Simulation
{
	std::list<b2Body*> bodies;
	b2Body* wall1;
	b2Body* floor;
	b2Body* wall2;

	std::string title() {return "Closed Space";}

	b2Body* makePlatform(b2Vec2 postion, float hx, float hy, float angle)
	{
		polygonDef wallDef;
		wallDef.bodyDef.position = postion;
		wallDef.bodyDef.angle = to_radians(angle);
		wallDef.bodyDef.type = b2_staticBody;
		wallDef.shape.SetAsBox(hx, hy);
		wallDef.fixtureDef.friction = 0.4;
		wallDef.fixtureDef.density = 1;
		return make_shape(world(), wallDef);
	}

	InitParams prepare()
	{
		InitParams params;
		params.videoMode = sf::VideoMode(800, 1000);
		return params;
	}

	void initialize()
	{
		setAllEdges(false);
		world().SetGravity(b2Vec2(0, 20));

		wall1 = makePlatform(b2Vec2(5, 50), 2, 80, -5);
		floor = makePlatform(b2Vec2(0, 100), 50, 2, -5);
		wall2 = makePlatform(b2Vec2(50, 100), 2, 35, 5);
	}

	void render()
	{
		drawBody(*wall1, sf::Color::Black);
		drawBody(*floor, sf::Color::Black);
		drawBody(*wall2, sf::Color::Black);

		drawBodies(bodies, sf::Color::Black, sf::Color::White, -2);
	}

	void think(unsigned long frame)
	{
		if(frame % 5 == 0)
		{
			polygonDef def;
			def.bodyDef.position.Set(randcentered(60, 15), randcentered(-20, 15));
			def.bodyDef.angle = to_radians(randcentered(10, 45));
			def.bodyDef.linearVelocity = random_centered(b2Vec2(-20, 20), 5);
			def.bodyDef.type = b2_dynamicBody;
			def.shape.SetAsBox(1, 5);
			def.fixtureDef.density = 1;
			def.fixtureDef.friction = 0.25;
			bodies.push_front(make_shape(world(), def));
		}

		bodies.remove_if([](b2Body* body)
		{
			return body->GetPosition().y > 110;
		});
	}

};

REGISTER_SIMULATION(ClosedSpace, "packed");