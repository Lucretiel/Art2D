#include "TreeSystem.h"

class Intense: public TreeSystem
{
	std::string title() override {return "Intense";}

	void initialize() override
	{

		//SUN
		circleDef def;
		def.bodyDef.active = false;
		def.bodyDef.type = b2_staticBody;
		def.bodyDef.position.Set(90, 10);
		def.shape.m_radius = 25;
		add_block(def, sf::Color::Green);

		//LEAVES
		std::vector<LeafSystem> leaves;

		LeafSystem leaf;
		leaf.color = sf::Color::Red;
		leaf.max_depth = -1;
		leaf.min_depth = 2;
		leaf.base_dangle = 90;
		leaf.delta_dangle = 10;
		leaf.size = .25;
		leaf.density = 3.5;
		leaves.push_back(leaf);

		//TREE
		std::vector<LeafSystem> overleaves;
		ColorTransform trans;
		make_tree(10, b2Vec2(75, 75),
			.9, .75,
			10, 35,
			0, 0,
			5, 45,
			10, 0,
			sf::Color(0, 100, 255), trans,
			overleaves, leaves);

		//Ground
		make_ground(5, 0, 5, sf::Color::Magenta, trans);

		//SKY
		background() = sf::Color::Yellow;
	}

};

REGISTER_SIMULATION(Intense, "intense")