#include "TrivialSenario.h"

#include "helpers.h"


REGISTER_SIMULATION(TrivialSenario, "trivial");

void TrivialSenario::initialize()
{
	auto physicsCenter = physicsDimensions();
	physicsCenter *= 0.5;

	box = make_box(
		world(),
		b2Vec2(
			physicsCenter.x,
			physicsCenter.y),
		b2Vec2(4, 4));

	world().SetGravity(b2Vec2(0, 10));
}

void TrivialSenario::render()
{
	drawBody(*box, sf::Color::Transparent, sf::Color::Black);
}