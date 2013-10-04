#include <vector>
#include <algorithm>

#include "Simulation.h"
#include "TypeRegistry.h"
#include "helpers.h"
#include "rand.h"

class PackedSpace: public Simulation
{
	std::string title() {return "Packed Space";}

	std::vector<b2Body*> bodies;
	b2Body* leftBricks;
	b2Body* rightBricks;

	InitParams prepare()
	{
		InitParams params;
		params.videoMode = sf::VideoMode(800, 1000);
		return params;
	}

	void initialize()
	{
		setTopEdge(false);
		setBottomEdge(false);
		world().SetGravity(b2Vec2(0, 20));

		std::vector<b2Vec2> bricks;
		bricks.push_back(b2Vec2(0, 0));
		bricks.push_back(b2Vec2(10, 0));
		bricks.push_back(b2Vec2(36.5, 80));
		bricks.push_back(b2Vec2(30, 80));
		bricks.push_back(b2Vec2(0, 25));

		polygonDef def1;
		def1.bodyDef.position.SetZero();
		def1.bodyDef.type = b2_staticBody;
		def1.shape.Set(bricks.data(), bricks.size());
		def1.fixtureDef.density = 1;
		
		leftBricks = make_shape(world(), def1);

		//Mirror
		for(auto it = bricks.begin(); it != bricks.end(); ++it)
			it->x = 80 - it->x;
		//Reverse for CCW
		std::reverse(bricks.begin(), bricks.end());

		polygonDef def2;
		def2.bodyDef.position.SetZero();
		def2.bodyDef.type = b2_staticBody;
		def2.shape.Set(bricks.data(), bricks.size());
		def2.fixtureDef.density = 1;
		
		rightBricks = make_shape(world(), def2);
	}

	void render()
	{
		drawBody(*leftBricks, sf::Color::Black);
		drawBody(*rightBricks, sf::Color::Black);
		drawBodies(bodies, sf::Color::Black, sf::Color::White, -2);
	}

	void think(unsigned long frame)
	{
		if(frame % 5 == 0)
		{
			make_bar(b2Vec2(randcentered(40, 20), -10));
		}
	}

	void make_bar(const b2Vec2& position)
	{
		polygonDef def;
		def.bodyDef.position = position;
		def.bodyDef.angle = to_radians(randcentered(0, 20));
		def.bodyDef.linearVelocity.Set(randcentered(0, 10), 10);
		def.bodyDef.type = b2_dynamicBody;
		def.shape.SetAsBox(1, 5);
		def.fixtureDef.density = 1;
		def.fixtureDef.friction = .5;
		bodies.push_back(make_shape(world(), def));
	}

	void handle_event(sf::Event& event)
	{
		default_event(event);

		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			make_bar(b2Vec2(40, 80));
	}
};

REGISTER_SIMULATION(PackedSpace, "packed");