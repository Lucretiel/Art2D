#include "TreeSystem.h"

class Cool: public TreeSystem
{
	std::string title() override {return "Cool";}

	void initialize() override
	{
		ColorTransform trans;
		trans.set_vs(sf::Vector2f(20, 20));

		std::vector<LeafSystem> leaves;

		LeafSystem leaf;
		leaf.color = sf::Color(31, 192, 64);
		leaf.transform.set_vs(sf::Vector2f(50, 20));
		leaf.max_depth = -1;
		leaf.min_depth = 2;
		leaf.base_dangle = 90;
		leaf.delta_dangle = 10;
		leaf.size = .25;
		leaf.density = 3.5;
		leaves.push_back(leaf);

		LeafSystem flower;
		flower.color = sf::Color(147, 113, 250);
		flower.transform.set_vs(sf::Vector2f(70, 20));
		flower.max_depth = -1;
		flower.min_depth = 4;
		flower.delta_dangle = 5;
		flower.size = .2;
		flower.density = .05;
		for(unsigned a = 0; a < 360; a += 120)
		{
			flower.base_dangle = a;
			leaves.push_back(flower);
		}


		make_tree(10, b2Vec2(75, 75),
			.9, .75,
			10, 35,
			0, 0,
			5, 45,
			10, 0,
			sf::Color(34, 44, 41), trans,
			leaves);
		make_ground(5, 0, 0, leaf.color, trans);
		background() = sf::Color(220, 230, 240);
	}

};

REGISTER_SIMULATION(Cool, "cool")
