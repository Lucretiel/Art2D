#pragma once

#include "Simulation.h"
#include "color_helpers.h"
#include "helpers.h"

#include <vector>

struct LeafSystem
{
	sf::Color color;
	ColorTransform transform;
	float base_dangle;
	float delta_dangle;
	unsigned min_depth;
	unsigned max_depth;
	float density;
	float size;
};


class TreeSystem: public Simulation
{
private:
	struct block
	{
		b2Body* body;
		sf::Color color;

		block(b2Body* body, const sf::Color& color):
			body(body),
			color(color)
		{}
	};

	std::vector<block> blocks;

protected:
	void make_tree(
		int iterations, b2Vec2 base,
		float ratio1, float ratio2,
		float min_core_angle, float max_core_angle,
		float min_branch_angle_1, float max_branch_angle_1,
		float min_branch_angle_2, float max_branch_angle_2,
		float initial_size, float leaf_size,
		const sf::Color& bark, const ColorTransform& bark_trans,
		const std::vector<LeafSystem>& over_leaves=std::vector<LeafSystem>(),
		const std::vector<LeafSystem>& under_leaves=std::vector<LeafSystem>());

	void make_ground(
		float thickness,
		float base_angle,
		float delta_angle,
		const sf::Color& baseColor,
		const ColorTransform& deltas);

	template<class T>
	void add_block(
		const shapeDef<T, 0>& shape,
		const sf::Color& color)
	{
		blocks.emplace_back(
			make_shape(world(), shape),
			color);
	}


private:
	void render() override
	{
		for(const block& b: blocks)
			drawBody(*b.body, b.color);
	}
};