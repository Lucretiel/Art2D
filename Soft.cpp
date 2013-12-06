#include "TreeSystem.h"

class Soft: public TreeSystem
{
	std::string title() override {return "Soft";}

	void initialize() override
	{
		ColorTransform trans;
		trans.set_vs(sf::Vector2f(20, 20));
		make_tree(10, b2Vec2(75, 75),
			.9, .75,
			10, 35,
			0, 0,
			5, 45,
			10, 0,
			sf::Color(88, 66, 44), trans);
		make_ground(5, 0, 0, sf::Color(250, 250, 250), trans);
		background() = to_val_sat(sf::Color(200, 200, 240), sf::Vector2f(140, 44));
	}

};

REGISTER_SIMULATION(Soft, "soft")
