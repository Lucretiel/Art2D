#include "ExplosionSenario.h"
#include "rand.h"
#include "helpers.h"

#include "TypeRegistry.h"

const static b2Vec2 boom_center(50, 95);
const static double boom_factor = 1000000;

REGISTER_SIMULATION(ExplosionSenario, "explosion");

b2Vec2 boom_impulse(const b2Vec2& center)
{
	auto boom_vec = center - boom_center;
	auto distance = boom_vec.Normalize();
	boom_vec *= boom_factor;
	boom_vec *= 1/distance;
	return boom_vec;
};

void ExplosionSenario::initialize()
{
	boxes.reserve(30);
	b2Vec2 x;
	auto physicsCenter = physicsDimensions();
	physicsCenter *= 0.5;

	for(int i = 0; i < 10; ++i)
	{
		boxes.push_back(make_box(
			world(),
			b2Vec2(
				physicsCenter.x + randuniform(-20, 20),
				physicsCenter.y + randuniform(-20, 20)),
			b2Vec2(1, 5),
			to_radians(randuniform(0, 360))));
	}

	world().SetGravity(b2Vec2(0, 0));
}

void ExplosionSenario::handle_event(sf::Event& event)
{
	if(event.type == sf::Event::Closed)
		throw Quit();

	if(event.type == sf::Event::KeyPressed)
	{
		switch(event.key.code)
		{
		case sf::Keyboard::Q:
			throw Quit();
			break;
		case sf::Keyboard::S:
			//Need extra block because of case
			{
				auto image = window().capture();
				image.saveToFile("screenshot.png");
			}
			break;
		case sf::Keyboard::Space:
			for(auto it = boxes.begin(); it != boxes.end(); ++it)
				(*it)->ApplyLinearImpulse(boom_impulse((*it)->GetPosition()), (*it)->GetPosition());
			break;
		}
	}
}

void ExplosionSenario::render()
{
	for(auto it = boxes.begin(); it != boxes.end(); ++it)
		drawBody(**it, sf::Color::Black, sf::Color::White);
}