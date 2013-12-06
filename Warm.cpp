#include "TreeSystem.h"

class Warm: public TreeSystem
{
	std::string title() override {return "Warm";}

	void initialize() override
	{
		ColorTransform trans;
		trans.set_vs(sf::Vector2f(20, 20));

		std::vector<LeafSystem> leaves(1);
		LeafSystem& leaf = leaves.front();
		leaf.color = sf::Color(255, 128, 0);
		leaf.transform.set_rgb(sf::Vector3f(40, 70, 20));
		leaf.transform.set_vs(sf::Vector2f(20, 20));
		leaf.max_depth = -1;
		leaf.min_depth = 3;
		leaf.base_dangle = 90;
		leaf.delta_dangle = 20;
		leaf.size = .2;
		leaf.density = .5;
		make_tree(10, b2Vec2(75, 75),
			.9, .75,
			10, 35,
			0, 0,
			5, 45,
			10, 0,
			sf::Color(100, 55, 44), trans,
			leaves);
		make_ground(5, 0, 0, leaf.color, leaf.transform);
		background() = sf::Color(240, 230, 220);
	}

};

REGISTER_SIMULATION(Warm, "warm")
