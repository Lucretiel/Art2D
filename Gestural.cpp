//Hoo boy.
#pragma once

#include <chrono>
#include <random>
#include "Simulation.h"
#include "helpers.h"
#include "rand.h"

bool sort_body(const b2Body* lhs, const b2Body* rhs)
{
	b2Vec2 middle(50, -40);
	return (lhs->GetPosition()-middle).Length() < (rhs->GetPosition()-middle).Length();
}



SIM(Gestural, "gestural")
{
	std::string title() override {return "Gestural";}

	std::list<b2Body*> bodies;
	std::list<b2Body*> bodies2;
	std::vector<sf::Vector2f> vertecies;
	std::vector<sf::CircleShape> dots;

	bool fancy_draw;
	bool produce;

	b2Vec2 center;
	b2Vec2 dimensions;

	void initialize() override
	{
		fancy_draw = false;
		produce = false;
		center = physicsHalfDimensions();
		dimensions = physicsDimensions();
		world().SetGravity(b2Vec2(0, 20));
		setAllEdges(false);

		for(int i = 0; i < 10; ++i)
		{
			circleDef def;
			def.bodyDef.position = random_margin(dimensions, 10);
			def.fixtureDef.density = 1;
			def.shape.m_radius = 1;
			bodies2.push_back(make_shape(world(), def));
		}

	}

	void think(unsigned long frame) override
	{
		auto pred = [this](b2Body* body)
		{
			auto p = body->GetPosition();
			return p.y > dimensions.y + 5 || p.x < -5 || p.x > dimensions.x;
		};
		bodies.remove_if(pred);
		bodies2.remove_if(pred);

		if(produce && frame % 5 == 0)
		{
			polygonDef def;
			def.bodyDef.position.Set(
				randcentered(center.x, 20),
				-20);
			def.bodyDef.angle = to_radians(randuniform(0, 360));
			def.bodyDef.type = b2_dynamicBody;
			def.shape.SetAsBox(1, 5);
			def.bodyDef.angularVelocity = to_radians(randcentered(0, 360));
			def.fixtureDef.density = 1;
			def.fixtureDef.restitution = .1;
			bodies.push_front(make_shape(world(), def));
		}
	}

	void render() override
	{
		drawBodies(bodies2, sf::Color::Black, sf::Color::White, -1);
		if(!fancy_draw)
			drawBodies(bodies, sf::Color::Blue, sf::Color::White, -1);
		else
			for(auto& circ : dots)
				window().draw(circ);
	}

	void handle_event(sf::Event& event)
	{
		default_event(event);
		switch(event.type)
		{
		case sf::Event::KeyPressed:
			switch(event.key.code)
			{
			case sf::Keyboard::F:
				if(is_paused())
					begin_fancy_draw();
				break;
			case sf::Keyboard::Space:
				produce = true;
				break;
			default:
				break;
			}
			break;
		case sf::Event::KeyReleased:
			switch(event.key.code)
			{
			case sf::Keyboard::Space:
				produce = false;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		if(!is_paused())
			end_fancy_draw();
	}

	void add_fancy_line(const sf::Vector2f& point1, const sf::Vector2f& point2)
	{
		auto p1 = random_centered(point1, 3);
		auto p2 = random_centered(point2, 3);
		auto line_vec = p2 - p1;
		auto normal_vec = to_length(rot90(line_vec), 1);
		float len = length(line_vec);
		unsigned num_steps = len / 1.5;

		float h_max = len/10;
		float rad_max = std::min<float>(2, h_max/2);
		float h = randcentered(0, h_max);

		linear_scale<unsigned, float> normal_scale(0, num_steps, -1, 1);
		linear_scale<unsigned, float> length_scale(0, num_steps, 0, len);

		for(unsigned step = 0; step <= num_steps; ++step)
		{
			float x = normal_scale(step);
			float y = (-h*((x*x) - 1));
			float r = (-rad_max*((x*x)-1)) + .5;
			auto real_vec = 
				p1 + (normal_vec * y) +
				(to_length(line_vec, length_scale(step)));

			dots.emplace_back(r);
			dots.back().setOrigin(r,r);
			dots.back().setPosition(real_vec);
			dots.back().setFillColor(sf::Color::Black);
		}
	}

	void begin_fancy_draw()
	{
		if(vertecies.empty())
		{
			std::vector<b2Body*> sorted_bodies(bodies.begin(), bodies.end());
			std::sort(sorted_bodies.begin(), sorted_bodies.end(), &sort_body);

			vertecies.reserve(sorted_bodies.size() * 4);
			const auto factor = simParams().factor;
			const auto w_dim = windowDimensions();
			for(b2Body* body : sorted_bodies)
			{
				bool add = true;
				const auto shape = static_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape());
				const auto xf = body->GetTransform();
				unsigned i_begin = randint(0,3);
				for(unsigned i = i_begin; i < i_begin+5; ++i)
				{
					vertecies.push_back(convertVector(shape->m_vertices[i%4], xf, factor));
				
				}

			}
		}
		dots.clear();
		for(auto it = vertecies.begin()+1; it != vertecies.end(); ++it)
		{
			add_fancy_line(*(it-1), *it);
		}

		fancy_draw = true;
	}

	void end_fancy_draw()
	{
		fancy_draw = false;
		vertecies.clear();
		dots.clear();
	}


};
