#include "TreeSystem.h"

#include <list>
#include <cmath>

#include "rand.h"
#include "helpers.h"

void TreeSystem::make_ground(
	float thickness,
	float base_angle,
	float delta_angle,
	const sf::Color& base_color,
	const ColorTransform& deltas)
{
	const float density_factor = 1.5;
	const float block_w = 5;
	const float block_h = 1;
	const float block_area = block_w * block_h;
	const float underground_depth = 5;

	auto phys = physicsDimensions();
	auto phys2 = physicsHalfDimensions();
	const float area = (thickness + underground_depth) * phys.x;
	const unsigned density = (area * density_factor) / block_area;

	blocks.reserve(blocks.size() + density + 1);

	polygonDef def;
	def.bodyDef.active = false;
	def.bodyDef.fixedRotation = true;
	def.bodyDef.type = b2_staticBody;
	def.shape.SetAsBox(phys2.x, thickness/2);
	def.bodyDef.angle = 0;
	def.bodyDef.position.Set(phys2.x, phys.y - (thickness/2));

	blocks.emplace_back(
		make_shape(world(), def),
		base_color);
	
	def.shape.SetAsBox(block_w, block_h);
	addProgress(1);

	const b2Vec2 topLeft(0, phys.y - thickness);
	const b2Vec2 bottomRight(phys.x, phys.y + underground_depth);

	maxProgress(blocks.size() + density + 1);
	setProgress(blocks.size());

	for(int i = 0; i < density; ++i)
	{
		def.bodyDef.position = random_in(topLeft, bottomRight);
		def.bodyDef.angle = to_radians(randcentered(base_angle, delta_angle));
		add_block(def, deltas.apply(base_color));
		addProgress(1);
	}
}

class TreeNode
{
private:
	static b2Vec2 root;
	const TreeNode* parent;
	b2Vec2 self;
	float length;
	float angle;
	bool trunk;
	bool inverse;
	unsigned id;

public:
	TreeNode(TreeNode& p, bool first):
		parent(&p),
		trunk(first && p.trunk),
		inverse(!p.inverse),
		id(p.id + 1)
	{
		if(first)
			if(p.trunk)
				set_dimensions(p.length * ratio1, 0);
			else
				set_dimensions(p.length * ratio1, randuniform(min_ba1, max_ba1));
		else
			if(p.trunk)
				set_dimensions(p.length * ratio2, randuniform(min_ca, max_ca));
			else
				set_dimensions(p.length * ratio2, -randuniform(min_ba2, max_ba2));
	}

	TreeNode(b2Vec2 new_root, float length):
		parent(nullptr),
		trunk(true),
		inverse(false),
		id(0)
	{
		root = new_root;
		set_dimensions(length, 90);
	}

	b2Vec2 top() const
	{
		return base() + self;
	}

	b2Vec2 base() const
	{
		if(parent)
			return parent->top();
		else
			return root;
	}
	
	b2Vec2 center() const
	{
		return base() + b2Vec2(self.x / 2, self.y / 2);
	}

	float get_angle() const
	{
		return angle;
	}

	float get_length() const
	{
		return length;
	}

	unsigned get_id() const
	{
		return id;
	}




	void set_dimensions(float l, float a)
	{
		angle = (parent ? parent->angle : 0) + (to_radians(a) * (inverse ? - 1 : 1));
		set_length(l);
	}

	void set_length(float l)
	{
		length = l;
		self.Set(l * std::cos(angle), -l * std::sin(angle));

	}

public:
	static float ratio1, ratio2;
	static float min_ca, max_ca;
	static float min_ba1, max_ba1;
	static float min_ba2, max_ba2;
};

float TreeNode::ratio1, TreeNode::ratio2,
	TreeNode::min_ca, TreeNode::max_ca,
	TreeNode::min_ba1, TreeNode::min_ba2,
	TreeNode::max_ba1, TreeNode::max_ba2;

b2Vec2 TreeNode::root;


void TreeSystem::make_tree(
	int iterations, b2Vec2 base,
	float ratio1, float ratio2,
	float min_core_angle, float max_core_angle,
	float min_branch_angle_1, float max_branch_angle_1,
	float min_branch_angle_2, float max_branch_angle_2,
	float initial_size, float leaf_size,
	const sf::Color& bark, const ColorTransform& bark_trans,
	const std::vector<LeafSystem>& over_leaves,
	const std::vector<LeafSystem>& under_leaves)
{
	TreeNode::ratio1 = ratio1;
	TreeNode::ratio2 = ratio2;
	TreeNode::min_ca = min_core_angle;
	TreeNode::max_ca = max_core_angle;
	TreeNode::min_ba1 = min_branch_angle_1;
	TreeNode::max_ba1 = max_branch_angle_1;
	TreeNode::min_ba2 = min_branch_angle_2;
	TreeNode::max_ba2 = max_branch_angle_2;

	//Generate Tree
	std::list<TreeNode> grown;
	std::list<TreeNode> growing;
	std::list<TreeNode> upcoming;

	grown.emplace_back(base, initial_size);
	growing.emplace_back(grown.back(), true);

	for(unsigned i = 0; i < iterations; ++i)
	{
		for(TreeNode& node: growing)
		{
			upcoming.emplace_back(node, true);
			upcoming.emplace_back(node, false);
		}
		grown.splice(grown.end(), growing);
		growing.splice(growing.end(), upcoming);
	}
	grown.splice(grown.end(), growing);

	auto make_leaves = [&](const std::vector<LeafSystem>& leaves)->void
	{
		
		polygonDef leafDef;
		leafDef.bodyDef.active = false;
		leafDef.bodyDef.type = b2_staticBody;
		leafDef.bodyDef.fixedRotation = true;
		for(const LeafSystem& system : leaves)
		{
			leafDef.shape.SetAsBox(5 * system.size, system.size);
			for(const TreeNode& node : grown)
			{
				if(system.min_depth > node.get_id() && system.min_depth >= 0)
					continue;
				if(system.max_depth < node.get_id() && system.max_depth >= 0)
					break;

				const float length = node.get_length();
				float num_leaves = (length * length * system.density / 2.5);
				if(system.density < 0 || (num_leaves < 1 && randuniform(0, 1) < num_leaves))
					num_leaves = 1.5;
				const unsigned count_leaves = num_leaves;
				const auto center = node.center();

				for(unsigned i = 0; i < count_leaves; ++i)
				{
					leafDef.bodyDef.position = random_centered(center, system.density < 0 ? 0 : length / 2);
					leafDef.bodyDef.angle = to_radians(randcentered(system.base_dangle, system.delta_dangle));
					add_block(leafDef, system.transform.apply(system.color));
				}
			}
		}
	};

	make_leaves(under_leaves);

	polygonDef def;
	def.bodyDef.active = false;
	def.bodyDef.fixedRotation = true;
	def.bodyDef.type = b2_staticBody;

	for(TreeNode& node : grown)
	{
		def.bodyDef.position = node.center();
		def.bodyDef.angle = -node.get_angle();
		def.shape.SetAsBox(node.get_length() / 2, node.get_length() / 10);
		add_block(def, bark_trans.apply(bark));
	}

	make_leaves(over_leaves);

	
}